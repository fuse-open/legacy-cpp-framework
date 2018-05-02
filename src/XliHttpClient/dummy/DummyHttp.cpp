//
// Copyright (C) 2010-2014 Outracks chnologies
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
// associated documentation files (the "Software"), to deal in the Software without restriction,
// including without limitation the rights to use, copy, modify, merge, publish, distribute,
// sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or substantial
// portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
// NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES
// OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
// CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//

#include <uBase/ArrayStream.h>
#include <uBase/BufferStream.h>
#include <uBase/Buffer.h>
#include <uBase/HashMap.h>
#include <XliHttpClient/HttpClient.h>

namespace Xli
{
    class AHttpRequest : public HttpRequest
    {
    public:
        virtual String GetMethod() const { return ""; }
        virtual String GetUrl() const { return ""; }
        virtual HttpRequestState GetState() const { return (HttpRequestState)0; }
        virtual void SetTimeout(int timeout) { }
        virtual int GetTimeout() const { return 0; }
        virtual void SetHeader(const String& key, const String& value) {}
        virtual void RemoveHeader(const String& key) {}
        virtual void SetVerifyHost(bool verify) {}
        virtual bool GetVerifyHost() const { return false; }
        virtual int HeadersBegin() const { return 0; }
        virtual int HeadersEnd() const { return 0; }
        virtual int HeadersNext(int n) const { return 0; }
        virtual String GetHeaderKey(int n) const { return ""; }
        virtual String GetHeaderValue(int n) const { return ""; }
        virtual void SendAsync(const void* content, int byteLength) {}
        virtual void SendAsync(const String& content) {}
        virtual void SendAsync() {}
        virtual void Abort() {}
        virtual int GetResponseHeaderCount() const { return 0; }
        virtual int ResponseHeadersBegin() const { return 0; }
        virtual int ResponseHeadersEnd() const { return 0; }
        virtual int ResponseHeadersNext(int n) const { return 0; }
        virtual String GetResponseHeaderKey(int n) const { return ""; }
        virtual String GetResponseHeaderValue(int n) const { return ""; }
        virtual bool TryGetResponseHeader(const String& key, String& result) const { return false; }
        virtual int GetResponseStatus() const { return 0; }
        virtual DataAccessor* GetResponseBody() const { return NULL; }
    };

    //------- Client -------//
    class AHttpClient : public HttpClient
    {
    public:
        static HttpClient* Create() { return NULL; }
        virtual AHttpRequest* CreateRequest(const String& method, const String& url) { return NULL; }
        virtual void SetEventHandler(HttpEventHandler* eventHandler) {}
        virtual HttpEventHandler* GetEventHandler() { return NULL; }
        virtual void EnqueueAction(HttpAction* action) {}
        virtual void Update() {}
    };
    HttpClient* HttpClient::Create() { return new AHttpClient(); }
}
