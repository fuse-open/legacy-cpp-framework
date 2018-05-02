#pragma once
#include <uBase/Stream.h>

namespace uBase
{
    /** \ingroup uIO */
    class BufferStream: public Stream
    {
        DataAccessor* _buf;
        bool _read, _write, _closed;
        int _pos;

    public:
        BufferStream(DataAccessor* buf, bool canRead, bool canWrite);
        virtual ~BufferStream();

        virtual void Close();

        virtual bool IsClosed() const;
        virtual bool AtEnd() const;

        virtual bool CanRead() const;
        virtual bool CanWrite() const;
        virtual bool CanSeek() const;

        virtual int Read(void* dst, int elmSize, int elmCount);
        virtual void Write(const void* dst, int elmSize, int elmCount);
        virtual void Seek(int offset, SeekOrigin origin);

        virtual int GetPosition() const;
        virtual int GetLength() const;

        virtual DataAccessor* CreateDataAccessor();
    };
}
