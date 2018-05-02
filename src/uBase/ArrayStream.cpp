#include <uBase/ArrayStream.h>
#include <cstring>

namespace uBase
{
    ArrayStream::ArrayStream(int initCapacity)
    {
        _buf.Reserve(initCapacity);
    }

    bool ArrayStream::CanWrite() const
    {
        return true;
    }

    void ArrayStream::Write(const void* src, int elmSize, int elmCount)
    {
        _buf.Expand();
        int loc = _buf.Length();
        _buf.Resize(loc + elmSize * elmCount);
        memcpy(_buf.Ptr() + loc, src, elmSize * elmCount);
    }

    int ArrayStream::GetLength() const
    {
        return _buf.Length();
    }

    const uint8_t* ArrayStream::GetPtr() const
    {
        return _buf.Ptr();
    }

    Buffer* ArrayStream::CopyBuffer() const
    {
        return Buffer::CopyFrom(_buf.Ptr(), _buf.Length());
    }

    void ArrayStream::Clear()
    {
        _buf.Clear();
    }
}
