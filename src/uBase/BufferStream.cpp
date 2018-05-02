#include <uBase/BufferStream.h>
#include <cstring>

namespace uBase
{
    BufferStream::BufferStream(DataAccessor* buf, bool canRead, bool canWrite)
    {
        if (!buf)
            throw NullPointerException();

        _buf = buf;
        _buf->Retain();
        _pos = 0;
        _read = canRead;
        _write = canWrite;
        _closed = false;
    }

    BufferStream::~BufferStream()
    {
        _buf->Release();
    }

    void BufferStream::Close()
    {
        _closed = true;
    }

    bool BufferStream::IsClosed() const
    {
        return _closed;
    }

    bool BufferStream::AtEnd() const
    {
        if (_closed)
            throw StreamClosedException();

        return _pos == _buf->GetSizeInBytes();
    }

    bool BufferStream::CanRead() const
    {
        return _read;
    }

    bool BufferStream::CanWrite() const
    {
        return _write;
    }

    bool BufferStream::CanSeek() const
    {
        return true;
    }

    int BufferStream::GetLength() const
    {
        if (_closed)
            throw StreamClosedException();

        return _buf->GetSizeInBytes();
    }

    int BufferStream::GetPosition() const
    {
        if (_closed)
            throw StreamClosedException();

        return _pos;
    }

    int BufferStream::Read(void* data, int elementSize, int elementCount)
    {
        if (!_read)
            throw StreamCantReadException();

        if (_closed)
            throw StreamClosedException();

        int bytes = elementCount * elementSize;
        int length = _buf->GetSizeInBytes();

        if (bytes > length - _pos)
        {
            bytes = length - _pos;
            bytes -= bytes % elementSize;
            elementCount = bytes / elementSize;
        }
        else if (bytes < 0)
        {
            bytes = 0;
            elementCount = 0;
        }

        memcpy(data, _buf->GetPtr() + _pos, bytes);
        _pos += bytes;

        return elementCount;
    }

    void BufferStream::Write(const void* data, int elementSize, int elementCount)
    {
        if (!_write)
            throw StreamCantReadException();

        if (_closed)
            throw StreamClosedException();

        int bytes = elementCount * elementSize;
        int length = _buf->GetSizeInBytes();

        if (bytes > length - _pos)
        {
            bytes = length - _pos;
            bytes -= bytes % elementSize;
            elementCount = bytes / elementSize;
        }
        else if (bytes < 0)
        {
            bytes = 0;
            elementCount = 0;
        }

        memcpy(const_cast<uint8_t*>(_buf->GetPtr()) + _pos, data, bytes);
        _pos += bytes;
    }

    void BufferStream::Seek(int offset, SeekOrigin origin)
    {
        if (_closed)
            throw StreamClosedException();

        int len = _buf->GetSizeInBytes();

        switch (origin)
        {
        case SeekOriginBegin:
            _pos = offset;
            break;

        case SeekOriginCurrent:
            _pos += offset;
            break;

        case SeekOriginEnd:
            _pos = len + offset;
            break;
        }

        if (_pos < 0)
            _pos = 0;
        else if (_pos > len)
            _pos = len;
    }

    DataAccessor* BufferStream::CreateDataAccessor()
    {
        return _buf;
    }
}
