#pragma once
#include <uBase/TextReader.h>
#include <uBase/TextWriter.h>

namespace uBase
{
    /** \ingroup uIO */
    class Text
    {
    public:
        static void Append(const String& filename, const String& content);

        static String Load(const String& filename);
        static String Load(Stream* stream);

        static void Save(const String& filename, const String& content);
        static void Save(Stream* stream, const String& content);
    };
}