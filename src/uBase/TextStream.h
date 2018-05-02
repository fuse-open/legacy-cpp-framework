#pragma once
#include <uBase/ArrayStream.h>

namespace uBase
{
    /** \ingroup uIO */
    class TextStream: public ArrayStream
    {
    public:
        TextStream(int initCapacity = 1024);
        String GetText() const;
    };
}