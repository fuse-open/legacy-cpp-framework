#include <uBase/TextWriter.h>
#include <cstring>

namespace uBase
{
    TextWriter::TextWriter(Stream* stream)
        : StreamWriter(stream)
    {
    }

    void TextWriter::Write(const char* str, int len)
    {
        _stream->Write(str, 1, len);
    }

    void TextWriter::WriteFormat(const char* format, ...)
    {
        va_list argList;
        va_start(argList, format);
        Write(String::Format(format, argList));
        va_end(argList);
    }

    void TextWriter::Write(const char* str)
    {
        Write(str, (int)strlen(str));
    }

    void TextWriter::Write(const String& str)
    {
        Write(str.Ptr(), str.Length());
    }

    void TextWriter::WriteLine()
    {
        _stream->Write("\r\n", 2, 1);
    }
}
