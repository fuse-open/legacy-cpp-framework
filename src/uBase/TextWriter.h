#pragma once
#include <uBase/Stream.h>
#include <uBase/String.h>
#include <uBase/File.h>
#include <uBase/Traits.h>

namespace uBase
{
    /** \ingroup uIO */
    class TextWriter: public StreamWriter
    {
    public:
        TextWriter(Stream* stream);

        void Write(const char* str, int len);
        void WriteFormat(const char* format, ...);
        void Write(const char* str);
        void Write(const String& str);
        void WriteLine();

        template <typename T>
        void Write(const T& t)
        {
            Write(uBase::Traits<T>::ToString(t));
        }

        template <typename T>
        void WriteLine(const T& t)
        {
            Write(t);
            WriteLine();
        }
    };
}