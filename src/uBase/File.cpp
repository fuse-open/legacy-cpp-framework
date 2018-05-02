#include <uBase/File.h>
#include <uBase/TextReader.h>
#include <uBase/Unicode.h>

namespace uBase
{
    String FileModeInfo::ToString(FileMode mode)
    {
        switch (mode)
        {
#define FORMAT(x) case x: return #x
        FORMAT(FileModeRead);
        FORMAT(FileModeWrite);
        FORMAT(FileModeAppend);
        FORMAT(FileModeNew);
        FORMAT(FileModeRandom);
        FORMAT(FileModeReadWrite);
        FORMAT(FileModeReadWriteNew);
        FORMAT(FileModeReadAppend);
        FORMAT(FileModeReadRandom);
        default: return "<unknown>";
#undef FORMAT
        }
    }

    File::File(const String& filename, FileMode mode)
    {
        const char* m = "rb";

        switch (mode)
        {
            case FileModeRead: m = "rb"; break;
            case FileModeWrite: m = "wb"; break;
            case FileModeAppend: m = "ab"; break;
            case FileModeReadWrite: m = "r+b"; break;
            case FileModeReadWriteNew: m = "w+b"; break;
            case FileModeReadAppend: m = "a+b"; break;
            default: throw Exception("Invalid file mode: " + FileModeInfo::ToString(mode));
        }

#if MSVC

        _fp = 0;
        StringU16 filenameW = Unicode::Utf8To16(filename);
        StringU16 mW = Unicode::Utf8To16(m);

        if (_wfopen_s(&_fp, filenameW.Ptr(), mW.Ptr()) != 0)
            throw FileCantOpenException(filename);

#else

        _fp = fopen(filename.Ptr(), m);
        if (!_fp) throw FileCantOpenException(filename);

#endif

        _flags = FileFlagsCanClose | FileFlagsCanSeek;

        if (mode & FileModeRead)
            _flags |= FileFlagsCanRead;

        if ((mode & FileModeWrite) || (mode & FileModeAppend))
            _flags |= FileFlagsCanWrite;
    }

    File::File(FILE* fp, int flags)
    {
        _fp = fp;
        _flags = flags;
    }

    File::~File()
    {
        if (_fp && (_flags & FileFlagsCanClose))
            fclose(_fp);
    }

    void File::Flush()
    {
        if (!_fp) throw StreamClosedException();
        fflush(_fp);
    }

    void File::Close()
    {
        if (!_fp)
            throw StreamClosedException();

        if (_flags & FileFlagsCanClose)
            fclose(_fp);

        _fp = 0;
    }

    bool File::AtEnd() const
    {
        if (!_fp)
            throw StreamClosedException();

        return feof(_fp) != 0;
    }

    bool File::IsClosed() const
    {
        return _fp == 0;
    }

    bool File::CanRead() const
    {
        return _fp != 0 && (_flags & FileFlagsCanRead);
    }

    bool File::CanWrite() const
    {
        return _fp != 0 && (_flags & FileFlagsCanWrite);
    }

    bool File::CanSeek() const
    {
        return _fp != 0 && (_flags & FileFlagsCanSeek);
    }

    int File::GetPosition() const
    {
        if (!_fp)
            throw StreamClosedException();

        return (int)ftell(_fp);
    }

    int File::GetLength() const
    {
        if (!_fp)
            throw StreamClosedException();

        long p = ftell(_fp);
        fseek(_fp, 0, SEEK_END);
        long l = ftell(_fp);
        fseek(_fp, p, SEEK_SET);
        return (int)l;
    }

    int File::Read(void* data, int elmSize, int elmCount)
    {
        if (!_fp)
            throw StreamClosedException();

        if (!(_flags & FileFlagsCanRead))
            throw StreamCantReadException();

        //the stream shall be flushed (fflush) or repositioned (fseek, fsetpos, rewind) before a reading operation that follows a writing operation
        if (CanRead() && CanWrite())
            fflush(_fp);

        return (int)fread(data, elmSize, elmCount, _fp);
    }

    void File::Write(const void* data, int elmSize, int elmCount)
    {
        if (!_fp)
            throw StreamClosedException();

        if (!(_flags & FileFlagsCanWrite))
            throw StreamCantWriteException();

        //the stream shall be repositioned (fseek, fsetpos, rewind) before a writing operation that follows a reading operation (whenever that operation did not reach the end-of-file).
        if (CanRead() && CanWrite())
            fseek(_fp, 0, SEEK_CUR);

        int result = (int)fwrite(data, elmSize, elmCount, _fp);

        if (result != elmCount && !(_flags & FileFlagsIgnoreWriteErrors))
            throw StreamCantWriteException();
    }

    void File::Seek(int offset, SeekOrigin origin)
    {
        if (!_fp)
            throw StreamClosedException();

        if (!(_flags & FileFlagsCanSeek))
            throw StreamCantSeekException();

        switch (origin)
        {
        case SeekOriginBegin:
            if (fseek(_fp, offset, SEEK_SET) == 0)
                return;

            break;

        case SeekOriginCurrent:
            if (fseek(_fp, offset, SEEK_CUR) == 0)
                return;

            break;

        case SeekOriginEnd:
            if (fseek(_fp, offset, SEEK_END) == 0)
                return;

            break;
        }

        throw StreamCantSeekException();
    }
}
