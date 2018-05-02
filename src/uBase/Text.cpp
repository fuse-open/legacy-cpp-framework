#include <uBase/Text.h>

namespace uBase
{
    void Text::Append(const String& filename, const String& content)
    {
        File f(filename, FileModeAppend);
        TextWriter(&f).Write(content);
    }

    String Text::Load(const String& filename)
    {
        File f(filename, FileModeRead);
        return TextReader(&f).ReadAll();
    }

    String Text::Load(Stream* stream)
    {
        return TextReader(stream).ReadAll();
    }

    void Text::Save(const String& filename, const String& content)
    {
        File f(filename, FileModeWrite);
        TextWriter(&f).Write(content);
    }

    void Text::Save(Stream* stream, const String& content)
    {
        TextWriter(stream).Write(content);
    }
}
