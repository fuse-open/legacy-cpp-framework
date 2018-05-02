#pragma once
#include <uBase/Buffer.h>
#include <uBase/String.h>

namespace Xli
{
    using namespace uBase;

    /** \ingroup XliHttpClient */
    enum HttpRequestState
    {
        HttpRequestStateUnsent=0,
        HttpRequestStateOpened=1,
        HttpRequestStateSent=2,
        HttpRequestStateHeadersReceived=3,
        HttpRequestStateLoading=4,
        HttpRequestStateDone=5,
    };

    /** \ingroup XliHttpClient */
    class HttpRequest: public Object
    {
        void *userData;

    public:
        void SetUserData(void *data) { userData = data; }
        void *GetUserData() const { return userData; }

        virtual String GetMethod() const = 0;
        virtual String GetUrl() const = 0;

        virtual HttpRequestState GetState() const = 0;

        virtual void SetTimeout(int timeout) = 0;
        virtual int GetTimeout() const = 0;

        virtual void SetHeader(const String& key, const String& value) = 0;
        virtual void RemoveHeader(const String& key) = 0;

        virtual void SetVerifyHost(bool verify) = 0;
        virtual bool GetVerifyHost() const = 0;

        virtual int HeadersBegin() const = 0;
        virtual int HeadersEnd() const = 0;
        virtual int HeadersNext(int iterator) const = 0;
        virtual String GetHeaderKey(int iterator) const = 0;
        virtual String GetHeaderValue(int iterator) const = 0;

        virtual void SendAsync(const void* content, int byteLength) = 0;
        virtual void SendAsync(const String& content) = 0;
        virtual void SendAsync() = 0;
        virtual void Abort() = 0;

        virtual int GetResponseHeaderCount() const = 0;
        virtual int ResponseHeadersBegin() const = 0;
        virtual int ResponseHeadersEnd() const = 0;
        virtual int ResponseHeadersNext(int iterator) const = 0;
        virtual String GetResponseHeaderKey(int iterator) const = 0;
        virtual String GetResponseHeaderValue(int iterator) const = 0;

        virtual bool TryGetResponseHeader(const String& key, String& result) const = 0;

        virtual int GetResponseStatus() const = 0;
        virtual DataAccessor* GetResponseBody() const = 0;
    };

    /** \ingroup XliHttpClient */
    class HttpEventHandler: public Object
    {
    public:
        virtual void OnRequestStateChanged(HttpRequest* request) { }
        virtual void OnRequestProgress(HttpRequest* request,int position, int total, bool totalKnown) { }
        virtual void OnRequestAborted(HttpRequest* request) { }
        virtual void OnRequestTimeout(HttpRequest* request) { }
        virtual void OnRequestError(HttpRequest* request,String message) { }
    };

    /** \ingroup XliHttpClient */
    class HttpAction : public Object
    {
    public:
        virtual void Execute() = 0;
    };

    /** \ingroup XliHttpClient */
    class HttpClient: public Object
    {
    public:
        static HttpClient* Create();

        virtual HttpRequest* CreateRequest(const String& method, const String& url) = 0;

        virtual void SetEventHandler(HttpEventHandler* eventHandler) = 0;
        virtual HttpEventHandler* GetEventHandler() = 0;

        virtual void Update() = 0;
    };
}