#pragma once
#include <uBase/File.h>

namespace uBase
{
    /** \ingroup uIO */
    enum FileAttributes
    {
        FileAttributesReadOnly = 1 << 0,
        FileAttributesDirectory = 1 << 1
    };

    /** \ingroup uIO */
    struct FileInfo
    {
        String Name;
        uint64_t Size;
        uint32_t Attributes;
        uint64_t CreationTime;
        uint64_t LastAccessTime;
        uint64_t LastWriteTime;
    };

    /** \ingroup uIO */
    class FileSystem: public Object
    {
    public:
        virtual ~FileSystem();

        virtual Stream* OpenFile(const String& filename, FileMode mode = FileModeRead) = 0;
        virtual DataAccessor* OpenFileAsBuffer(const String& filename);

        virtual void CreateDirectory(const String& name);
        virtual void DeleteDirectory(const String& name);
        virtual void DeleteFile(const String& name);

        virtual void MoveDirectory(const String& oldName, const String& newName);
        virtual void MoveFile(const String& oldName, const String& newName);

        virtual void CopyFile(const String& sourceName, const String& destination, bool overwrite);

        virtual bool GetFileInfo(const String& path, FileInfo& result);
        virtual void GetFiles(const String& path, Array<FileInfo>& list);

        virtual void GetFiles(Array<FileInfo>& list);

        virtual bool Exists(const String& path);
        virtual bool IsFile(const String& path);
        virtual bool IsDirectory(const String& path);

        /** Makes sure all directories in the path exists. */
        virtual void CreateDirectories(const String& path);

        /** Removes directory and all contents. */
        virtual void DeleteDirectoryRecursive(const String& name);

        virtual FileSystem* CreateSubFileSystem(const String& path);
    };
}