#include <uBase/ArrayStream.h>
#include <uBase/BufferStream.h>
#include <uBase/Buffer.h>
#include <uBase/HashMap.h>

#include <XliHttpClient/HttpClient.h>
#include "CurlHttpClient.h"

#include <curl/curl.h>
#include <stdlib.h>
#include <cstring>
#include <stdlib.h>

namespace Xli
{
    static int HttpInitialized = 0;
    static CURLcode CurlGlobalCode;
    static void ShutdownCurlHttp()
    {
        // Assumes clients have cleaned up up the handles they own
        curl_global_cleanup();
    }
    static void InitCurlHttp()
    {
        if (!HttpInitialized)
        {
            CurlGlobalCode = curl_global_init(CURL_GLOBAL_ALL);
            atexit(Xli::ShutdownCurlHttp);
            HttpInitialized = 1;
        }
    }

    class CurlHttpRequest : public HttpRequest
    {
    private:
        CurlHttpClient* client;

        HttpRequestState state;

        String url;
        String method;
        int timeout;
        bool verifyHost;

        HashMap<String,String> headers;
        struct curl_slist* curlUploadHeaders;
        CURL* curlSession;
        bool requestOwnsUploadData;

        Auto<BufferStream> uploadBuffer;

        HashMap<String,String> responseHeaders;
        Auto< ArrayStream > responseBody;
        Auto< BufferRef > responseBodyRef;
        int responseStatus;

    public:
        bool aborted;

        CurlHttpRequest(CurlHttpClient* client, String url, String method)
        {
            this->client = client;
            this->state = HttpRequestStateOpened;
            this->url = url;
            this->method = method;
            this->timeout = 0;
            this->verifyHost = true;
            this->curlUploadHeaders=NULL;
            this->responseBody = new ArrayStream(1);
            this->responseStatus = -1;
            this->aborted = false;
            HttpEventHandler* eh = client->GetEventHandler();
            if (eh!=0) eh->OnRequestStateChanged(this);
        }

        virtual ~CurlHttpRequest()
        {
            Abort();
            curl_slist_free_all(curlUploadHeaders);
            // {TODO} free upload headers
        }

        virtual String GetMethod() const { return method; }
        virtual String GetUrl() const { return url; }
        virtual HttpRequestState GetState() const { return state; }
        virtual int GetTimeout() const { return timeout; }
        virtual void SetTimeout(int timeout)
        {
            if (state <= HttpRequestStateOpened)
            {
                this->timeout = timeout;
            } else {
                throw Exception("HttpRequest->SetTimeout(): Not in a valid state to set the timeout");
            }
        }
        virtual bool GetVerifyHost() const { return verifyHost; }
        virtual void SetVerifyHost(bool verify)
        {
            if (state <= HttpRequestStateOpened)
            {
                verifyHost = verify;
            } else {
                throw Exception("HttpRequest->SetTimeout(): Not in a valid state to set the verify host flag");
            }
        }
        virtual void SetHeader(const String& key, const String& value)
        {
            if (state == HttpRequestStateOpened)
            {
                headers.Add(key,value);
            } else {
                throw Exception("HttpRequest->SetHeader(): Not in a valid state to set a header");
            }
        }
        virtual void RemoveHeader(const String& key)
        {
            if (state == HttpRequestStateOpened)
            {
                headers.Remove(key);
            } else {
                throw Exception("HttpRequest->SetHeader(): Not in a valid state to set a header");
            }
        }
        virtual int HeadersBegin() const { return headers.Begin(); }
        virtual int HeadersEnd() const { return headers.End(); }
        virtual int HeadersNext(int n) const { return headers.Next(n); }
        virtual String GetHeaderKey(int n) const { return headers.GetKey(n); }
        virtual String GetHeaderValue(int n) const { return headers.GetValue(n); }

        virtual CURLcode SetCurlRequestOptions(CURL* session, const void* content, int byteLength)
        {
            CURLcode result = CURLE_OK;

            // set tpref
            if (result == CURLE_OK) result = curl_easy_setopt(session, CURLOPT_PRIVATE, (void*)this);

            // set method option
            if (method == "GET")
            {
                result = curl_easy_setopt(session, CURLOPT_HTTPGET, 1);
            } else if (method == "POST") {
                result = curl_easy_setopt(session, CURLOPT_POST, 1);
            } else if (method == "PUT") {
                result = curl_easy_setopt(session, CURLOPT_UPLOAD, 1);
            } else if (method == "OPTIONS" || method == "HEAD" || method == "TRACE" || method == "DELETE") {
                curl_easy_setopt(session, CURLOPT_CUSTOMREQUEST, method.Ptr());
            } else {
                return CURLE_FAILED_INIT;
            }
            if (result == CURLE_OK) result = curl_easy_setopt(session, CURLOPT_URL, url.Ptr());
            if (result == CURLE_OK) result = curl_easy_setopt(session, CURLOPT_CONNECTTIMEOUT, timeout);
            if (result == CURLE_OK) result = curl_easy_setopt(session, CURLOPT_AUTOREFERER, 1);
            if (result == CURLE_OK) result = curl_easy_setopt(session, CURLOPT_USERAGENT, "libcurl-agent/1.0");
            if (result == CURLE_OK) result = curl_easy_setopt(session, CURLOPT_HEADERFUNCTION, onHeaderRecieved);
            if (result == CURLE_OK) result = curl_easy_setopt(session, CURLOPT_HEADERDATA, (void*)this);
            if (result == CURLE_OK) result = curl_easy_setopt(session, CURLOPT_WRITEFUNCTION, onDataRecieved);
            if (result == CURLE_OK) result = curl_easy_setopt(session, CURLOPT_WRITEDATA, (void*)this);
            if (result == CURLE_OK) result = curl_easy_setopt(session, CURLOPT_NOPROGRESS, 0);
            if (result == CURLE_OK) result = curl_easy_setopt(session, CURLOPT_PROGRESSDATA, (void*)this);
            if (result == CURLE_OK) result = curl_easy_setopt(session, CURLOPT_PROGRESSFUNCTION, onProgress);

           //{TODO} Add and test CURLOPT_USE_SSL
            if (!verifyHost)
            {
                if (result == CURLE_OK) result = curl_easy_setopt(session, CURLOPT_SSL_VERIFYPEER, 0);
                if (result == CURLE_OK) result = curl_easy_setopt(session, CURLOPT_SSL_VERIFYHOST, 0);
            }

            //Method specific options
            if (method ==  "GET")
            {
                if (content!=0 && byteLength>0) result = CURLE_FAILED_INIT;
            } else if (method == "HEAD") {
                if (content!=0 && byteLength>0) result = CURLE_FAILED_INIT;
                if (result == CURLE_OK) result = curl_easy_setopt(session, CURLOPT_NOBODY, 1);
            } else if (method == "POST") {
                if (byteLength <= 0)
                {
                    if (result == CURLE_OK) result = curl_easy_setopt(session, CURLOPT_POSTFIELDSIZE, -1);
                } else {
                    if (result == CURLE_OK) result = curl_easy_setopt(session, CURLOPT_POSTFIELDS, (void*)content);
                    if (result == CURLE_OK) result = curl_easy_setopt(session, CURLOPT_POSTFIELDSIZE_LARGE, byteLength);
                }
            } else if (method == "PUT") {
                if (content==0 || byteLength <= 0) result = CURLE_FAILED_INIT;
                if (result == CURLE_OK) result = curl_easy_setopt(session, CURLOPT_READFUNCTION, onDataUpload);
                if (result == CURLE_OK) result = curl_easy_setopt(session, CURLOPT_READDATA, (void*)this);
            };

            return result;
        }

        virtual void SendAsync(const void* content, int byteLength)
        {
            if (state != HttpRequestStateOpened)
            {
                //throw Exception("HttpRequest->SetArrayPulledCallback(): Not in a valid state to send");
                HttpEventHandler* eh = client->GetEventHandler();
                if (eh!=0 && !aborted) eh->OnRequestError(this,"HttpRequest->SendAsync(): Not in a valid state to send");
                return;
            }

            if (content!=0 && byteLength>0)
            {
                // const_cast is safe here are bufferstream is set readonly
                uploadBuffer = new BufferStream(new BufferPtr(const_cast<void*>(content), byteLength, requestOwnsUploadData), true, false);
            }

            //Create session
            CURL* session = curl_easy_init();
            if (!session)
            {
                HttpEventHandler* eh = client->GetEventHandler();
                if (eh!=0 && !aborted) eh->OnRequestError(this, "HttpRequest->SendAsync(): could not create session");
                return;
            }

            // Set curl options
            CURLcode result = SetCurlRequestOptions(session, content, byteLength);

            //Add headers
            int i = HeadersBegin();
            String header;
            String key;
            while (i != HeadersEnd())
            {
                header = "";
                key = GetHeaderKey(i);
                if (key.Length() > 0)
                {
                    header += key;
                    header += ":";
                    header += GetHeaderValue(i);
                    curlUploadHeaders = curl_slist_append(curlUploadHeaders, header.Ptr());
                }
                i = HeadersNext(i);
            }
            curl_easy_setopt(session, CURLOPT_HTTPHEADER, curlUploadHeaders);

            if (result == CURLE_OK)
            {
                state = HttpRequestStateSent;

                HttpEventHandler* eh = client->GetEventHandler();
                if (eh!=0 && !aborted) eh->OnRequestStateChanged(this);

                curlSession = session;
                client->AddSession(session, this);
            } else {
                HttpEventHandler* eh = client->GetEventHandler();
                if (eh!=0 && !aborted) eh->OnRequestError(this, "HttpRequest->SendAsync(): Failed to set curl options" );
            }
        }
        virtual void SendAsync(const String& content)
        {
            requestOwnsUploadData = true;
            void* raw = malloc(content.Length());
            memcpy(raw, content.Ptr(), content.Length());
            SendAsync(raw, content.Length());
        }
        virtual void SendAsync()
        {
            SendAsync((void*)0, -1);
        }

        virtual void Abort()
        {
            if (!aborted)
            {
                client->RemoveSession(curlSession);
                curlSession = 0;
                aborted = true;

                HttpEventHandler* eh = client->GetEventHandler();
                if (eh!=0) eh->OnRequestAborted(this);
            }
        }

        virtual int GetResponseHeaderCount() const
        {
            if (state >= HttpRequestStateHeadersReceived)
            {
                return responseHeaders.Count();
            } else {
                throw Exception("HttpRequest->GetResponseHeaderCount(): Not in a valid state to get the response header count");
            }
        }
        virtual int ResponseHeadersBegin() const
        {
            if (state >= HttpRequestStateHeadersReceived)
            {
                return responseHeaders.Begin();
            } else {
                throw Exception("HttpRequest->ResponseHeaderBegin(): Not in a valid state to get the response header iterator");
            }
        }
        virtual int ResponseHeadersNext(int n) const
        {
            if (state >= HttpRequestStateHeadersReceived)
            {
                return responseHeaders.Next(n);
            } else {
                throw Exception("HttpRequest->ResponseHeaderNext(): Not in a valid state to get the next response header");
            }
        }
        virtual int ResponseHeadersEnd() const
        {
            if (state >= HttpRequestStateHeadersReceived)
            {
                return responseHeaders.End();
            } else {
                throw Exception("HttpRequest->ResponseHeaderEnd(): Not in a valid state to get the response header");
            }
        }
        virtual String GetResponseHeaderKey(int n) const
        {
            if (state >= HttpRequestStateHeadersReceived)
            {
                return responseHeaders.GetKey(n);
            } else {
                throw Exception("HttpRequest->GetResponseHeaderN(): Not in a valid state to get the response header");
            }
        }
        virtual String GetResponseHeaderValue(int n) const
        {
            if (state >= HttpRequestStateHeadersReceived)
            {
                return responseHeaders.GetValue(n);
            } else {
                throw Exception("HttpRequest->GetResponseHeaderN(): Not in a valid state to get the response header");
            }
        }

        virtual int GetResponseStatus() const
        {
            if (state >= HttpRequestStateHeadersReceived)
            {
                return responseStatus;
            } else {
                throw Exception("HttpRequest->GetResponseStatus(): Not in a valid state to get the response status");
            }
        }

        virtual bool TryGetResponseHeader(const String& key, String& result) const
        {
            if (state >= HttpRequestStateHeadersReceived)
            {
                return responseHeaders.TryGetValue(key, result);
            } else {
                throw Exception("HttpRequest->GetResponseHeader(): Not in a valid state to get the response header");
            }
        }

        virtual DataAccessor* GetResponseBody() const
        {
            if (state == HttpRequestStateDone)
            {
                return (DataAccessor*)responseBodyRef.Get();
            } else {
                throw Exception("HttpRequest->GetResponseBody(): Not in a valid state to get the response body");
            }
        }

        virtual void onDone( int code )
        {
            state = HttpRequestStateDone;
            responseStatus = code;

            if (!aborted)
            {
                curl_easy_cleanup(curlSession);
            }

            responseBodyRef = new BufferRef((void*)responseBody->GetPtr(), responseBody->GetLength(), (Object*)responseBody.Get());

            HttpEventHandler* eh = client->GetEventHandler();
            if (eh!=0 && !aborted) eh->OnRequestStateChanged(this);
        }

        virtual void onError(int errorCode, String errorMessage)
        {
            state = HttpRequestStateDone;

            if (!aborted)
            {
                curl_easy_cleanup(curlSession);
            }

            HttpEventHandler* eh = client->GetEventHandler();
            if (eh!=0 && !aborted) eh->OnRequestError(this, errorMessage);
        }

    private:
        static size_t onDataUpload( void *ptr, size_t size, size_t nmemb, void *userdata)
        {
            CurlHttpRequest* request = (CurlHttpRequest*)userdata;

            // if the session is aborted then we want to skip this. We cant say
            // we did the job though so we pass back 0 and accept this may indicate
            // that this method caused the problem
            if (request->aborted) return 0;

            size_t maxCopyLength = (size * nmemb);
            size_t copyLength = 0;
            if (maxCopyLength > 0 && !request->uploadBuffer->AtEnd())
            {
                copyLength = request->uploadBuffer->Read(ptr, 1, maxCopyLength);
            }
            return copyLength;
        }

        static size_t onHeaderRecieved(void *ptr, size_t size, size_t nmemb, void *userdata)
        {
            CurlHttpRequest* request = (CurlHttpRequest*)userdata;

            // if the session is aborted then we want to skip this but not
            // cause curl to think this method caused the problem.
            if (request->aborted) return (size * nmemb);

            size_t bytesPulled = (size * nmemb);
            if (bytesPulled > 0)
            {
                String fullHeader((char*)ptr, bytesPulled);
                if (fullHeader.Length() > 0) {
                    int end = fullHeader.Length();
                    if (fullHeader[end-1]=='\n') end-=1;
                    if (fullHeader[end-1]=='\r') end-=1;
                    fullHeader = fullHeader.Substring(0, end);
                    if (fullHeader.Length() > 0) {
                        int splitPos = fullHeader.IndexOf(':');
                        // curl can include the status-line in the headers which is
                        // daft. If there isnt a ':' then it's not valid and we drop
                        // it.
                        if (splitPos > 0)
                        {
                            String key = fullHeader.Substring(0, splitPos);
                            String val = fullHeader.Substring(splitPos+1);
                            if (!request->responseHeaders.ContainsKey(key)) {
                                request->responseHeaders.Add(key, val);
                            }
                        }
                    }
                }
            }
            return bytesPulled;
        }

        static size_t onDataRecieved(char* ptr, size_t size, size_t nmemb, void* userdata)
        {
            CurlHttpRequest* request = (CurlHttpRequest*)userdata;

            // if the session is aborted then we want to skip this but not
            // cause curl to think this method caused the problem.
            if (request->aborted) return (size * nmemb);

            size_t bytesRead = 0;

            // {TODO} saying we have the headers at this point. Check the validity
            request->state = HttpRequestStateHeadersReceived;

            long http_code = 0;
            CURLcode res = curl_easy_getinfo(request->curlSession, CURLINFO_RESPONSE_CODE, &http_code);
            if (res == CURLE_OK) {
                request->responseStatus = (int)http_code;
            }

            HttpEventHandler* eh = request->client->GetEventHandler();
            if (eh!=0) eh->OnRequestStateChanged(request);

            bytesRead = (size * nmemb);
            request->responseBody->Write((uint8_t*)ptr, 1, bytesRead);

            return bytesRead;
        }

        static int onProgress(void *clientp, curl_off_t dltotal, curl_off_t dlnow, curl_off_t ultotal, curl_off_t ulnow)
        {
            CurlHttpRequest* request = (CurlHttpRequest*)clientp;

            if (request->aborted)
            {
                // curl_easy_cleanup(session);
                return CURLE_ABORTED_BY_CALLBACK;
            }
            if (dlnow && (request->state == HttpRequestStateLoading))
            {
                HttpEventHandler* eh = request->client->GetEventHandler();
                if (eh!=0) eh->OnRequestProgress(request, dlnow, dltotal, (dltotal > 0));
            }
            if (ulnow && (request->state == HttpRequestStateSent))
            {
                HttpEventHandler* eh = request->client->GetEventHandler();
                if (eh!=0) eh->OnRequestProgress(request, ulnow, ultotal, (ultotal > 0));
            }
            return 0;
        }
        static void onTimeout(CurlHttpRequest* request) // {TODO} this is never used...the fuck?
        {

            HttpEventHandler* eh = request->client->GetEventHandler();
            if (eh!=0) eh->OnRequestTimeout(request);
        }

        //{TODO} What happens if there is an error? Do we get a callback? We need this to clean up
    };

    //------------------------------------------------------------


    CurlHttpClient::CurlHttpClient()
    {
        multiSession = curl_multi_init();
    }

    CurlHttpClient::~CurlHttpClient()
    {
        //{TODO} find all curlcodes in this file and add checks for them
        CURLMcode result = curl_multi_cleanup(multiSession);
    }

    HttpRequest* CurlHttpClient::CreateRequest(const String& method, const String& url)
    {
        return new CurlHttpRequest(this, url, method);
    }

    void CurlHttpClient::SetEventHandler(HttpEventHandler* eventHandler)
    {
        this->eventHandler = eventHandler;
    }

    HttpEventHandler* CurlHttpClient::GetEventHandler()
    {
        return eventHandler;
    }

    void CurlHttpClient::AddSession(CURL* session, CurlHttpRequest* request)
    {
        CURLMcode result = curl_multi_add_handle(multiSession, session);
    }

    void CurlHttpClient::RemoveSession(CURL* session)
    {
        curl_multi_remove_handle(multiSession, session);
    }

    void CurlHttpClient::Update()
    {
        int msgRemaining;
        int runningHandles;
        CURLMsg* msg;
        CURL* session = 0;
        CurlHttpRequest* request = 0;
        CURLMSG messageType;
        CURLcode resultState;

        CURLMcode pCode = curl_multi_perform(multiSession, &runningHandles);

        while (NULL != (msg = curl_multi_info_read(multiSession, &msgRemaining)))
        {
            session = msg->easy_handle;
            char* tmp = (char*)&request;
            CURLcode ss = curl_easy_getinfo (session, CURLINFO_PRIVATE, tmp);

            messageType = msg->msg;
            if (messageType == CURLMSG_DONE)
            {
                resultState = msg->data.result;
                if (resultState == CURLE_OK)
                {
                    long http_code = 0;
                    CURLcode res = curl_easy_getinfo (session, CURLINFO_RESPONSE_CODE, &http_code);
                    request->onDone((int)http_code);
                } else {
                    request->onError((int)resultState, CurlErrorToString(resultState));
                }
            } else {
                HttpEventHandler* eh = this->GetEventHandler();
                if (eh!=0) eh->OnRequestError(request, "Client expected CURLMSG_DONE");
            }
        }
    }

    HttpClient* HttpClient::Create()
    {
        if (!HttpInitialized) InitCurlHttp();
        return new CurlHttpClient();
    }
}
