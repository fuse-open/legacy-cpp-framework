#include <uBase/Unicode.h>
#include <uBase/Console.h>
#include "../../3rdparty/ConvertUTF/ConvertUTF.h"
#include <cstring>

namespace uBase
{
    static const ConversionFlags Flags = lenientConversion;

    bool Unicode::IsLegalUtf8(const char* str, int len, int flags)
    {
        Array<char> utf8;

        // Check and handle MUTF-8 (FIXME: Simple implementation can be optimized)
        if (flags & UnicodeFlagsModifiedUtf8)
        {
            for (int i = 0; i < len - 1; i++)
            {
                if (str[i + 0] == (char)(unsigned char)0xC0 &&
                    str[i + 1] == (char)(unsigned char)0x80)
                {
                    utf8.Reserve(len * 2);
                    utf8.AddRange(str, i);
                    utf8.Add(0);

                    for (int j = i + 2; j < len; j++)
                    {
                        if (j < len - 1 &&
                            str[j + 0] == (char)(unsigned char)0xC0 &&
                            str[j + 1] == (char)(unsigned char)0x80)
                        {
                            utf8.Add(0);
                            j++;
                        }
                        else
                            utf8.Add(str[j]);
                    }

                    str = utf8.Ptr();
                    len = utf8.Length();
                    break;
                }
            }
        }

        const UTF8* src_start = (const UTF8*)str;
        const UTF8* src_end = src_start + len;

        return isLegalUTF8Sequence(src_start, src_end) != 0;
    }

    bool Unicode::IsLegalUtf8(const String& str, int flags)
    {
        return IsLegalUtf8(str.Ptr(), str.Length(), flags);
    }

    StringU16 Unicode::Utf8To16(const char* str, int len, int flags)
    {
        Array<char> utf8;

        // Check and handle MUTF-8 (FIXME: Simple implementation can be optimized)
        if (flags & UnicodeFlagsModifiedUtf8)
        {
            for (int i = 0; i < len - 1; i++)
            {
                if (str[i + 0] == (char)(unsigned char)0xC0 &&
                    str[i + 1] == (char)(unsigned char)0x80)
                {
                    utf8.Reserve(len * 2);
                    utf8.AddRange(str, i);
                    utf8.Add(0);

                    for (int j = i + 2; j < len; j++)
                    {
                        if (j < len - 1 &&
                            str[j + 0] == (char)(unsigned char)0xC0 &&
                            str[j + 1] == (char)(unsigned char)0x80)
                        {
                            utf8.Add(0);
                            j++;
                        }
                        else
                            utf8.Add(str[j]);
                    }

                    str = utf8.Ptr();
                    len = utf8.Length();
                    break;
                }
            }
        }

        const UTF8* src_start = (const UTF8*)str;
        const UTF8* src_end = src_start + len;

        StringU16 result = StringU16::Create(len);
        UTF16* dst_start = (UTF16*)result._data;
        UTF16* dst_end = dst_start + result._length;

        ConversionResult res = ConvertUTF8toUTF16(&src_start, src_end, &dst_start, dst_end, Flags);

        if (res != conversionOK && !(flags & UnicodeFlagsIgnoreError))
            throw Exception("Unicode conversion failed");

        // Adjust length
        result._length = (int)(dst_start - (UTF16*)result._data);
        result._data[result._length] = 0;

        return result;
    }

    StringU16 Unicode::Utf8To16(const String& str, int flags)
    {
        return Utf8To16(str.Ptr(), str.Length(), flags);
    }

    String Unicode::Utf16To8(const Utf16* str, int len, int flags)
    {
        const UTF16* src_start = (const UTF16*)str;
        const UTF16* src_end = src_start + len;

        String result = String::Create(len * 4);
        UTF8* dst_start = (UTF8*)result._data;
        UTF8* dst_end = dst_start + result._length;

        ConversionResult res = ConvertUTF16toUTF8(&src_start, src_end, &dst_start, dst_end, Flags);

        if (res != conversionOK && !(flags & UnicodeFlagsIgnoreError))
            throw Exception("Unicode conversion failed");

        // Adjust length
        result._length = (int)(dst_start - (UTF8*)result._data);
        result._data[result._length] = 0;

        // Check and handle MUTF-8 (FIXME: Simple implementation can be optimized)
        if (flags & UnicodeFlagsModifiedUtf8)
        {
            for (int i = 0; i < result._length; i++)
            {
                if (result[i] == 0)
                {
                    Array<char> mutf8;
                    mutf8.Reserve(result._length * 2);
                    mutf8.AddRange(result._data, i);
                    mutf8.Add((char)(unsigned char)0xC0);
                    mutf8.Add((char)(unsigned char)0x80);

                    for (int j = i + 1; j < result._length; j++)
                    {
                        if (result._data[j] == 0)
                        {
                            mutf8.Add((char)(unsigned char)0xC0);
                            mutf8.Add((char)(unsigned char)0x80);
                        }
                        else
                            mutf8.Add(result._data[j]);
                    }

                    return String(mutf8.Ptr(), mutf8.Length());
                }
            }
        }

        return result;
    }

    String Unicode::Utf16To8(const StringU16& str, int flags)
    {
        return Utf16To8(str.Ptr(), str.Length(), flags);
    }

    #include "UnicodeUpperLower.inc"
}
