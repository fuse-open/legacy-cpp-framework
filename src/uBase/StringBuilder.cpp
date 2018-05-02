#include <uBase/StringBuilder.h>
#include <cstring>

namespace uBase
{
    StringBuilder::StringBuilder(int cap)
    {
        _buffer.Reserve(cap);
    }

    StringBuilder& StringBuilder::operator +=(const String& text)
    {
        Append(text);
        return *this;
    }

    StringBuilder& StringBuilder::operator +=(const char* str)
    {
        Append(str);
        return *this;
    }

    StringBuilder& StringBuilder::operator +=(char c)
    {
        Append(c);
        return *this;
    }

    int StringBuilder::GetLength() const
    {
        return _buffer.Length();
    }

    void StringBuilder::Append(const char* str)
    {
        _buffer.AddRange(str, (int)strlen(str));
    }

    void StringBuilder::Append(const String& text)
    {
        _buffer.AddRange(text.Ptr(), text.Length());
    }

    void StringBuilder::Append(char c)
    {
        _buffer.Add(c);
    }

    void StringBuilder::AppendLine(const char* str)
    {
        Append(str);
        _buffer.Add('\n');
    }

    void StringBuilder::AppendLine(const String& text)
    {
        Append(text);
        _buffer.Add('\n');
    }

    void StringBuilder::AppendLine()
    {
        _buffer.Add('\n');
    }

    void StringBuilder::AppendFormat(const char* format, ...)
    {
        va_list argList;
        va_start(argList, format);
        Append(String::Format(format, argList));
        va_end(argList);
    }

    void StringBuilder::Clear()
    {
        _buffer.Clear();
    }

    String StringBuilder::ToString() const
    {
        return String(_buffer.Ptr(), _buffer.Length());
    }
}
