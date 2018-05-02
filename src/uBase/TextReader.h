#pragma once
#include <uBase/Array.h>
#include <uBase/Stream.h>

namespace uBase
{
    /** \ingroup uIO */
    class TextReader: public StreamReader
    {
    public:
        TextReader(Stream* stream);

        String Read(int len);
        String ReadAll();
        void ReadLines(Array<String>& lines);
        char ReadChar();
        String ReadTo(char terminal);
        String ReadLine();
        bool AtEnd();
    };
}