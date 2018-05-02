#include <uBase/TextReader.h>
#include <uBase/File.h>

namespace uBase
{
    TextReader::TextReader(Stream* stream)
        : StreamReader(stream)
    {
    }

    String TextReader::Read(int len)
    {
        String s = String::Create(len);
        _stream->ReadSafe(s.Ptr(), 1, len);
        return s;
    }

    String TextReader::ReadAll()
    {
        if (_stream->CanSeek())
            return Read(_stream->GetLength());

        Array<char> str;

        char buf[1024];
        int len = 0;

        while ((len = _stream->Read(buf, 1, 1024)))
            str.AddRange(buf, len);

        return String(str.Ptr(), str.Length());
    }

    void TextReader::ReadLines(Array<String>& lines)
    {
        while (!AtEnd())
            lines.Add(ReadLine());
    }

    char TextReader::ReadChar()
    {
        char c;
        _stream->ReadSafe(&c, 1, 1);
        return c;
    }

    String TextReader::ReadTo(char terminal)
    {
        Array<char> s;
        char c;

        while (_stream->Read(&c, 1, 1) == 1)
        {
            s.Add(c);
            if (c == terminal)
                break;
        }

        return String(s.Ptr(), s.Length());
    }

    String TextReader::ReadLine()
    {
        Array<char> s;
        char c;

        while (_stream->Read(&c, 1, 1) == 1)
        {
            if (c == '\n')
                break;

            if (c == '\r' && _stream->Read(&c, 1, 1) == 1)
            {
                if (c != '\n')
                    _stream->Seek(-1, SeekOriginCurrent);
                break;
            }
            s.Add(c);
        }

        return String(s.Ptr(), s.Length());
    }

    bool TextReader::AtEnd()
    {
        return _stream->AtEnd();
    }
}
