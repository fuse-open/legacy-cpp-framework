#pragma once
#include <uBase/String.h>
#include <uBase/Array.h>

namespace uBase
{
    /** \ingroup uText */
    class StringBuilder
    {
        Array<char> _buffer;

    public:
        StringBuilder(int cap = 1024);

        StringBuilder& operator +=(const String& text);
        StringBuilder& operator +=(const char* str);
        StringBuilder& operator +=(char c);

        int GetLength() const;

        void Append(const char* str);
        void Append(const String& text);
        void Append(char c);

        void AppendLine(const char* str);
        void AppendLine(const String& text);
        void AppendLine();

        void AppendFormat(const char* format, ...);

        void Clear();

        String ToString() const;
    };
}
