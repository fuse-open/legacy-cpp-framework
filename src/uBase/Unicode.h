#pragma once
#include <uBase/String.h>
#include <uBase/StringU16.h>

namespace uBase
{
    /** \ingroup uText */
    enum UnicodeFlags
    {
        UnicodeFlagsModifiedUtf8 = 1 << 0,
        UnicodeFlagsIgnoreError = 1 << 1,
    };

    /** \ingroup uText */
    class Unicode
    {
    public:
        static bool IsLegalUtf8(const char* str, int len, int flags = 0);
        static bool IsLegalUtf8(const String& str, int flags = 0);

        static StringU16 Utf8To16(const char* str, int len, int flags = 0);
        static StringU16 Utf8To16(const String& str, int flags = 0);

        static String Utf16To8(const Utf16* str, int len, int flags = 0);
        static String Utf16To8(const StringU16& str, int flags = 0);

        static Utf16 ToUpper(Utf16 chr);
        static Utf16 ToLower(Utf16 chr);
    };
}
