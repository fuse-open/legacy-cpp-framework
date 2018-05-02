#include <uBase/TextStream.h>

namespace uBase
{
    TextStream::TextStream(int initCapacity)
        : ArrayStream(initCapacity)
    {
    }

    String TextStream::GetText() const
    {
        return String((const char*)_buf.Ptr(), GetLength());
    }
}
