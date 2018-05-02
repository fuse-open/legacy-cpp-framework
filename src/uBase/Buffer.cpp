#include <uBase/Buffer.h>
#include <cstdlib>
#include <cstring>

namespace uBase
{
    Buffer::Buffer(int size)
    {
        _size = size;
        _data = ((uint8_t*)this) + sizeof(Buffer);
    }

    void* Buffer::operator new (size_t size, int bufSize)
    {
        return malloc(sizeof(Buffer) + bufSize);
    }

    void Buffer::operator delete (void* buf, int bufSize)
    {
        free(buf);
    }

    void Buffer::operator delete (void* buf)
    {
        free(buf);
    }

    Buffer* Buffer::Create(int size)
    {
        return new (size) Buffer(size);
    }

    void Buffer::Delete()
    {
        this->~Buffer();
        free(this);
    }

    Buffer* Buffer::CopyFrom(const void* data, int size)
    {
        Buffer* b = Buffer::Create(size);
        memcpy(b->Ptr(), data, size);
        return b;
    }

    int Buffer::Size() const
    {
        return _size;
    }

    uint8_t* Buffer::Ptr()
    {
        return _data;
    }

    const uint8_t* Buffer::Ptr() const
    {
        return _data;
    }

    const uint8_t* Buffer::GetPtr() const
    {
        return _data;
    }

    int Buffer::GetSizeInBytes() const
    {
        return _size;
    }

    BufferPtr::BufferPtr(void* data, int size, bool ownsData)
        : _data((uint8_t*)data), _size(size), _ownsData(ownsData)
    {
    }

    BufferPtr::~BufferPtr()
    {
        if (_ownsData)
            delete [] (uint8_t*)_data;
    }

    int BufferPtr::Size() const
    {
        return _size;
    }

    uint8_t* BufferPtr::Ptr()
    {
        return _data;
    }

    const uint8_t* BufferPtr::Ptr() const
    {
        return _data;
    }

    const uint8_t* BufferPtr::GetPtr() const
    {
        return _data;
    }

    int BufferPtr::GetSizeInBytes() const
    {
        return _size;
    }

    BufferRef::BufferRef(void* data, int size, Object* owner)
        : _data((uint8_t*)data), _size(size), _owner(owner)
    {
        if (owner)
            owner->Retain();
    }

    BufferRef::~BufferRef()
    {
        if (_owner)
            _owner->Release();
    }

    const uint8_t* BufferRef::GetPtr() const
    {
        return _data;
    }

    int BufferRef::GetSizeInBytes() const
    {
        return _size;
    }
}
