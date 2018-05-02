//{TODO} IFDEFS HERE
#include <XliHttpClient/HttpClient.h>
#include <XliPlatform/Window.h>
#include <curl/curl.h>

namespace Xli
{
    class CurlHttpRequest;

    class CurlHttpClient : public HttpClient
    {
    private:
        Auto<HttpEventHandler> eventHandler;
        int keyCounter;
        CURLM* multiSession;
    public:
        static HttpClient* Create();
        CurlHttpClient();
        virtual ~CurlHttpClient();
        virtual HttpRequest* CreateRequest(const String& method, const String& url);
        virtual void SetEventHandler(HttpEventHandler* eventHandler);
        virtual void AddSession(CURL* session, CurlHttpRequest* request);
        virtual void RemoveSession(CURL* session);
        virtual HttpEventHandler* GetEventHandler();
        virtual void Update();
    };
};


Xli::String CurlErrorToString(CURLcode errorCode)
{
    switch (errorCode)
    {
    case CURLE_OK:
        return "All fine. Proceed as usual.";
    case CURLE_UNSUPPORTED_PROTOCOL:
        return "The URL you passed to libcurl used a protocol that this libcurl does not support. The support might be a compile-time option that you didn't use, it can be a misspelled protocol string or just a protocol libcurl has no code for.";
    case CURLE_FAILED_INIT:
        return "Very early initialization code failed. This is likely to be an internal error or problem, or a resource problem where something fundamental couldn't get done at init time.";
    case CURLE_URL_MALFORMAT:
        return "The URL was not properly formatted.";
    case CURLE_NOT_BUILT_IN:
        return "A requested feature, protocol or option was not found built-in in this libcurl due to a build-time decision. This means that a feature or option was not enabled or explicitly disabled when libcurl was built and in order to get it to function you have to get a rebuilt libcurl.";
    case CURLE_COULDNT_RESOLVE_PROXY:
        return "Couldn't resolve proxy. The given proxy host could not be resolved.";
    case CURLE_COULDNT_RESOLVE_HOST:
        return "Couldn't resolve host. The given remote host was not resolved.";
    case CURLE_COULDNT_CONNECT:
        return "Failed to connect() to host or proxy.";
    case CURLE_FTP_WEIRD_SERVER_REPLY:
        return "After connecting to a FTP server, libcurl expects to get a certain reply back. This error code implies that it got a strange or bad reply. The given remote server is probably not an OK FTP server.";
    case CURLE_REMOTE_ACCESS_DENIED:
        return "We were denied access to the resource given in the URL. For FTP, this occurs while trying to change to the remote directory.";
    case CURLE_FTP_ACCEPT_FAILED:
        return "While waiting for the server to connect back when an active FTP session is used, an error code was sent over the control connection or similar.";
    case CURLE_FTP_WEIRD_PASS_REPLY:
        return "After having sent the FTP password to the server, libcurl expects a proper reply. This error code indicates that an unexpected code was returned.";
    case CURLE_FTP_ACCEPT_TIMEOUT:
        return "During an active FTP session while waiting for the server to connect, the CURLOPT_ACCEPTTIMOUT_MS (or the internal default) timeout expired.";
    case CURLE_FTP_WEIRD_PASV_REPLY:
        return "libcurl failed to get a sensible result back from the server as a response to either a PASV or a EPSV command. The server is flawed.";
    case CURLE_FTP_WEIRD_227_FORMAT:
        return "FTP servers return a 227-line as a response to a PASV command. If libcurl fails to parse that line, this return code is passed back.";
    case CURLE_FTP_CANT_GET_HOST:
        return "An internal failure to lookup the host used for the new connection.";
    case CURLE_FTP_COULDNT_SET_TYPE:
        return "Received an error when trying to set the transfer mode to binary or ASCII.";
    case CURLE_PARTIAL_FILE:
        return "A file transfer was shorter or larger than expected. This happens when the server first reports an expected transfer size, and then delivers data that doesn't match the previously given size.";
    case CURLE_FTP_COULDNT_RETR_FILE:
        return "This was either a weird reply to a 'RETR' command or a zero byte transfer complete.";
    case CURLE_QUOTE_ERROR:
        return "When sending custom \"QUOTE\" commands to the remote server, one of the commands returned an error code that was 400 or higher (for FTP) or otherwise indicated unsuccessful completion of the command.";
    case CURLE_HTTP_RETURNED_ERROR:
        return "This is returned if CURLOPT_FAILONERROR is set TRUE and the HTTP server returns an error code that is >= 400.";
    case CURLE_WRITE_ERROR:
        return "An error occurred when writing received data to a local file, or an error was returned to libcurl from a write callback.";
    case CURLE_UPLOAD_FAILED:
        return "Failed starting the upload. For FTP, the server typically denied the STOR command. The error buffer usually contains the server's explanation for this.";
    case CURLE_READ_ERROR:
        return "There was a problem reading a local file or an error returned by the read callback.";
    case CURLE_OUT_OF_MEMORY:
        return "A memory allocation request failed. This is serious badness and things are severely screwed up if this ever occurs.";
    case CURLE_OPERATION_TIMEDOUT:
        return "Operation timeout. The specified time-out period was reached according to the conditions.";
    case CURLE_FTP_PORT_FAILED:
        return "The FTP PORT command returned error. This mostly happens when you haven't specified a good enough address for libcurl to use. See CURLOPT_FTPPORT.";
    case CURLE_FTP_COULDNT_USE_REST:
        return "The FTP REST command returned error. This should never happen if the server is sane.";
    case CURLE_RANGE_ERROR:
        return "The server does not support or accept range requests.";
    case CURLE_HTTP_POST_ERROR:
        return "This is an odd error that mainly occurs due to internal confusion.";
    case CURLE_SSL_CONNECT_ERROR:
        return "A problem occurred somewhere in the SSL/TLS handshake. You really want the error buffer and read the message there as it pinpoints the problem slightly more. Could be certificates (file formats, paths, permissions), passwords, and others.";
    case CURLE_BAD_DOWNLOAD_RESUME:
        return "The download could not be resumed because the specified offset was out of the file boundary.";
    case CURLE_FILE_COULDNT_READ_FILE:
        return "A file given with FILE:// couldn't be opened. Most likely because the file path doesn't identify an existing file. Did you check file permissions?";
    case CURLE_LDAP_CANNOT_BIND:
        return "LDAP cannot bind. LDAP bind operation failed.";
    case CURLE_LDAP_SEARCH_FAILED:
        return "LDAP search failed.";
    case CURLE_FUNCTION_NOT_FOUND:
        return "Function not found. A required zlib function was not found.";
    case CURLE_ABORTED_BY_CALLBACK:
        return "Aborted by callback. A callback returned \"abort\" to libcurl.";
    case CURLE_BAD_FUNCTION_ARGUMENT:
        return "Internal error. A function was called with a bad parameter.";
    case CURLE_INTERFACE_FAILED:
        return "Interface error. A specified outgoing interface could not be used. Set which interface to use for outgoing connections' source IP address with CURLOPT_INTERFACE.";
    case CURLE_TOO_MANY_REDIRECTS:
        return "Too many redirects. When following redirects, libcurl hit the maximum amount. Set your limit with CURLOPT_MAXREDIRS.";
    case CURLE_UNKNOWN_OPTION:
        return "An option passed to libcurl is not recognized/known. Refer to the appropriate documentation. This is most likely a problem in the program that uses libcurl. The error buffer might contain more specific information about which exact option it concerns.";
    case CURLE_TELNET_OPTION_SYNTAX:
        return "A telnet option string was Illegally formatted.";
    case CURLE_PEER_FAILED_VERIFICATION:
        return "The remote server's SSL certificate or SSH md5 fingerprint was deemed not OK.";
    case CURLE_GOT_NOTHING:
        return "Nothing was returned from the server, and under the circumstances, getting nothing is considered an error.";
    case CURLE_SSL_ENGINE_NOTFOUND:
        return "The specified crypto engine wasn't found.";
    case CURLE_SSL_ENGINE_SETFAILED:
        return "Failed setting the selected SSL crypto engine as default!";
    case CURLE_SEND_ERROR:
        return "Failed sending network data.";
    case CURLE_RECV_ERROR:
        return "Failure with receiving network data.";
    case CURLE_SSL_CERTPROBLEM:
        return "problem with the local client certificate.";
    case CURLE_SSL_CIPHER:
        return "Couldn't use specified cipher.";
    case CURLE_SSL_CACERT:
        return "Peer certificate cannot be authenticated with known CA certificates.";
    case CURLE_BAD_CONTENT_ENCODING:
        return "Unrecognized transfer encoding.";
    case CURLE_LDAP_INVALID_URL:
        return "Invalid LDAP URL.";
    case CURLE_FILESIZE_EXCEEDED:
        return "Maximum file size exceeded.";
    case CURLE_USE_SSL_FAILED:
        return "Requested FTP SSL level failed.";
    case CURLE_SEND_FAIL_REWIND:
        return "When doing a send operation curl had to rewind the data to retransmit, but the rewinding operation failed.";
    case CURLE_SSL_ENGINE_INITFAILED:
        return "Initiating the SSL Engine failed.";
    case CURLE_LOGIN_DENIED:
        return "The remote server denied curl to login (Added in 7.13.1)";
    case CURLE_TFTP_NOTFOUND:
        return "File not found on TFTP server.";
    case CURLE_TFTP_PERM:
        return "Permission problem on TFTP server.";
    case CURLE_REMOTE_DISK_FULL:
        return "Out of disk space on the server.";
    case CURLE_TFTP_ILLEGAL:
        return "Illegal TFTP operation.";
    case CURLE_TFTP_UNKNOWNID:
        return "Unknown TFTP transfer ID.";
    case CURLE_REMOTE_FILE_EXISTS:
        return "File already exists and will not be overwritten.";
    case CURLE_TFTP_NOSUCHUSER:
        return "This error should never be returned by a properly functioning TFTP server.";
    case CURLE_CONV_FAILED:
        return "Character conversion failed.";
    case CURLE_CONV_REQD:
        return "Caller must register conversion callbacks.";
    case CURLE_SSL_CACERT_BADFILE:
        return "Problem with reading the SSL CA cert (path? access rights?)";
    case CURLE_REMOTE_FILE_NOT_FOUND:
        return "The resource referenced in the URL does not exist.";
    case CURLE_SSH:
        return "An unspecified error occurred during the SSH session.";
    case CURLE_SSL_SHUTDOWN_FAILED:
        return "Failed to shut down the SSL connection.";
    case CURLE_AGAIN:
        return "Socket is not ready for send/recv wait till it's ready and try again. This return code is only returned from curl_easy_recv(3) and curl_easy_send(3) (Added in 7.18.2)";
    case CURLE_SSL_CRL_BADFILE:
        return "Failed to load CRL file (Added in 7.19.0)";
    case CURLE_SSL_ISSUER_ERROR:
        return "Issuer check failed (Added in 7.19.0)";
    case CURLE_FTP_PRET_FAILED:
        return "The FTP server does not understand the PRET command at all or does not support the given argument. Be careful when using CURLOPT_CUSTOMREQUEST, a custom LIST command will be sent with PRET CMD before PASV as well. (Added in 7.20.0)";
    case CURLE_RTSP_CSEQ_ERROR:
        return "Mismatch of RTSP CSeq numbers.";
    case CURLE_RTSP_SESSION_ERROR:
        return "Mismatch of RTSP Session Identifiers.";
    case CURLE_FTP_BAD_FILE_LIST:
        return "Unable to parse FTP file list (during FTP wildcard downloading).";
    case CURLE_CHUNK_FAILED:
        return "Chunk callback reported error.";
    case CURLE_NO_CONNECTION_AVAILABLE:
        return "(For internal use only, will never be returned by libcurl) No connection available, the session will be queued. (added in 7.30.0)";
    default:
        return "Unknown Error Code";
    }
}
