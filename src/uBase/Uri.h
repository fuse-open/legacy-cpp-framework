#pragma once
#include <uBase/String.h>

namespace uBase
{
    /** \ingroup Http */
    class Uri
    {
    public:
        static String Encode(const String& uri);
        static String AutoEncodeUri(const String& uri);
        static String Decode(const String& uri);
    };
}