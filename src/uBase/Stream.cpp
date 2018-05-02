#include <uBase/Stream.h>
#include <uBase/Console.h>
#include <uBase/File.h>

namespace uBase
{
    Stream::~Stream()
    {
    }

    void Stream::Flush()
    {
    }

    void Stream::Close()
    {
    }

    bool Stream::AtEnd() const
    {
        return IsClosed() || (CanSeek() ? GetPosition() == GetLength() : false);
    }

    bool Stream::IsClosed() const
    {
        return false;
    }

    bool Stream::CanRead() const
    {
        return false;
    }

    bool Stream::CanWrite() const
    {
        return false;
    }

    bool Stream::CanSeek() const
    {
        return false;
    }

    int Stream::Read(void* dst, int elmSize, int elmCount)
    {
        throw StreamCantReadException();
    }

    void Stream::Write(const void* src, int elmSize, int elmCount)
    {
        throw StreamCantWriteException();
    }

    void Stream::Seek(int offset, SeekOrigin origin)
    {
        throw StreamCantSeekException();
    }

    int Stream::GetPosition() const
    {
        throw StreamCantSeekException();
    }

    int Stream::GetLength() const
    {
        throw StreamCantSeekException();
    }

    void Stream::ReadSafe(void* dst, int elmSize, int elmCount)
    {
        int r = Read(dst, elmSize, elmCount);

        if (r != elmCount)
        {
            if (AtEnd())
                throw EndOfStreamException();

            throw StreamCantReadException();
        }
    }

    void Stream::WriteStream(Stream* source)
    {
        // TODO: Use constant size buffer
        Auto<Buffer> buf = Buffer::Create(source->GetLength());
        source->Read(buf->Ptr(), 1, buf->Size());
        Write(buf->Ptr(), 1, buf->Size());
    }

    StreamWriter::StreamWriter(Stream* stream)
    {
        _stream = 0;
        SetStream(stream);
    }

    StreamWriter::~StreamWriter()
    {
        _stream->Release();
    }

    void StreamWriter::SetStream(Stream* stream)
    {
        if (!stream)
            throw NullPointerException();

        if (!stream->CanWrite())
            throw StreamCantWriteException();

        stream->Retain();

        if (_stream)
            _stream->Release();

        _stream = stream;
    }

    Stream* StreamWriter::GetStream()
    {
        return _stream;
    }

    StreamReader::StreamReader(Stream* stream)
    {
        _stream = 0;
        SetStream(stream);
    }

    StreamReader::~StreamReader()
    {
        _stream->Release();
    }

    void StreamReader::SetStream(Stream* stream)
    {
        if (!stream)
            throw NullPointerException();

        if (!stream->CanRead())
            throw StreamCantReadException();

        stream->Retain();

        if (_stream)
            _stream->Release();

        _stream = stream;
    }

    Stream* StreamReader::GetStream()
    {
        return _stream;
    }
}
