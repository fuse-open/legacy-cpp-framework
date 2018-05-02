#include <uBase/Uri.h>
#include <uBase/StringBuilder.h>
#include <cstdio>
#include <cctype>
#include <cstring>

namespace uBase
{
    static const char* HexLits = "0123456789ABCDEF";
    static const char* DoNotEncode = "!*'();:@&=+$,/?#[]-_.~";

    bool ShouldEncode(int c, bool encodeParenthesis);
    String EncodeInternal(const String& uri, bool encodeParenthesis);

    String Uri::Encode(const String& uri)
    {
        return EncodeInternal(uri, true);
    }

    String Uri::AutoEncodeUri(const String& uri)
    {
        return EncodeInternal(uri, false);
    }

    String EncodeInternal(const String& uri, bool encodeParenthesis)
    {
        StringBuilder sb;

        for (int i = 0; i < uri.Length(); i++)
        {
            int c = (int)(unsigned char)uri[i];

            if (ShouldEncode(c, encodeParenthesis))
            {
                sb.Append('%');
                sb.Append(HexLits[(c >> 4) & 0xf]);
                sb.Append(HexLits[(c >> 0) & 0xf]);
            }
            else
            {
                sb.Append(uri[i]);
            }
        }

        return sb.ToString();
    }

    String Uri::Decode(const String& uri)
    {
        // TODO
        return uri;
    }

    bool ShouldEncode(int c, bool encodeParenthesis)
    {
        if ((c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))
            return false;

        for (int i = 0; i < std::strlen(DoNotEncode); ++i)
            if (c == (int)DoNotEncode[i])
                return false;

        if (!encodeParenthesis && c == '%')
            return false;

        return true;
    }
}
