#pragma once
#include <uBase/Stream.h>
#include <cstdio>

namespace uBase
{
    /** \ingroup uIO */
    enum FileMode
    {
        /** Open a file for reading. The file must exist. */
        FileModeRead = 1 << 0,
        /** Create an empty file for writing. If a file with the same name already exists its content is erased and the file is treated as a new empty file. */
        FileModeWrite = 1 << 1,
        /** Append to a file. Writing operations append data at the end of the file. The file is created if it does not exist. */
        FileModeAppend = 1 << 2,
        /** Creates a new empty file. */
        FileModeNew = 1 << 4,
        /** Open a file for random access */
        FileModeRandom = 1 << 5,

        /** Open a file for update both reading and writing. The file must exist. */
        FileModeReadWrite = FileModeRead | FileModeWrite,
        /** Create an empty file for both reading and writing. If a file with the same name already exists its content is erased and the file is treated as a new empty file. */
        FileModeReadWriteNew = FileModeRead | FileModeWrite | FileModeNew,
        /** Open a file for reading and appending. All writing operations are performed at the end of the file, protecting the previous content to be overwritten. You can reposition (fseek, rewind) the internal pointer to anywhere in the file for reading, but writing operations will move it back to the end of file. The file is created if it does not exist. */
        FileModeReadAppend = FileModeRead | FileModeAppend,
        /** Open a file for random reading */
        FileModeReadRandom = FileModeRead | FileModeRandom,
    };

    /** \ingroup uIO */
    class FileModeInfo
    {
    public:
        static String ToString(FileMode mode);

    };

    /** \ingroup uIO */
    enum FileFlags
    {
        FileFlagsCanRead = 1 << 0,
        FileFlagsCanWrite = 1 << 1,
        FileFlagsCanSeek = 1 << 2,
        FileFlagsCanClose = 1 << 3,
        FileFlagsIgnoreWriteErrors = 1 << 4,
    };

    /** \ingroup uIO */
    class File: public Stream
    {
        FILE* _fp;
        int _flags;

    public:
        File(const String& filename, FileMode mode);
        File(FILE* fp, int flags);
        virtual ~File();

        virtual void Flush();
        virtual void Close();

        virtual bool AtEnd() const;
        virtual bool IsClosed() const;
        virtual bool CanRead() const;
        virtual bool CanWrite() const;
        virtual bool CanSeek() const;
        virtual int GetPosition() const;
        virtual int GetLength() const;

        virtual int Read(void* data, int elmSize, int elmCount);
        virtual void Write(const void* data, int elmSize, int elmCount);
        virtual void Seek(int offset, SeekOrigin origin);
    };
}
