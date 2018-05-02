#include "UnixFileSystem.h"

#include <uBase/Time.h>
#include <uBase/Random.h>
#include <uBase/Traits.h>
#include <uBase/DateTime.h>
#include <uBase/Path.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>
#include <errno.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>

namespace uBase
{
    static uint64_t ConvertToTimestamp(time_t time)
    {
        return (uint64_t)time * DateTime::PerSecond + (369 * 365 + 89) * DateTime::PerDay;
    }

    Stream* UnixFileSystem::OpenFile(const String& filename, FileMode mode)
    {
        return new File(filename, mode);
    }

    String UnixFileSystem::CreateTempFilename()
    {
        String prefix = Path::Combine(GetTempDirectory(), (String)(int)getpid() + "-");

        Random rand(Default::Hash(GetTimestamp()));
        static const char* cs = "1234567890abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
        int cl = strlen(cs);

        while (true)
        {
            String tmp = String::Create(8);

            for (int i = 0; i < tmp.Length(); i++)
                tmp[i] = cs[rand.NextInt(cl)];

            String result = prefix + tmp;

            struct stat attributes;
            if (stat(result.Ptr(), &attributes) == -1)
                return result;
        }
    }

    String UnixFileSystem::GetCurrentDirectory()
    {
        char buf[1024];
        const char* ptr = getcwd(buf, 1024);

        if (ptr != buf)
            return ".";

        return buf;
    }

    void UnixFileSystem::SetCurrentDirectory(const String& path)
    {
        if (!path.Length())
            return;

        if (chdir(path.Ptr()) != 0)
            throw Exception("Unable to change directory to '" + path + "'");
    }

    void UnixFileSystem::CreateDirectory(const String& path)
    {
        struct stat st;

        if (!path.Length() ||
            path == "~" ||
            stat(path.Ptr(), &st) == 0)
            return;

        if (mkdir(path.Ptr(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) != 0)
        {
            if (errno == EEXIST)
                return;

            throw Exception("Unable to create directory '" + path + "'");
        }
    }

    void UnixFileSystem::DeleteDirectory(const String& path)
    {
        if (rmdir(path.Ptr()) != 0)
            throw Exception("Unable to delete directory '" + path + "'");
    }

    void UnixFileSystem::DeleteFile(const String& path)
    {
        if (unlink(path.Ptr()) != 0)
            throw Exception("Unable to delete file '" + path + "'");
    }

    void UnixFileSystem::MoveDirectory(const String& oldPath, const String& newPath)
    {
        if (rename(oldPath.Ptr(), newPath.Ptr()) != 0)
            throw Exception("Unable to move directory '" + oldPath + "' to '" + newPath + "'");
    }

    void UnixFileSystem::MoveFile(const String& oldPath, const String& newPath)
    {
        if (rename(oldPath.Ptr(), newPath.Ptr()) != 0)
            throw Exception("Unable to move file '" + oldPath + "' to '" + newPath + "'");
    }

    void UnixFileSystem::CopyFile(const String& sourcePath, const String& destinationPath, bool overwrite)
    {
        int source;
        int destination;
        int createFlags = O_WRONLY | O_CREAT;
        if (!overwrite)
            createFlags |=O_EXCL;
        struct stat stat_buf;
        size_t size = 0;
        char buf[BUFSIZ];

        source = open(sourcePath.Ptr(), O_RDONLY);
        if (source == -1)
        {
            throw Exception("Unable to copy file '" + sourcePath + "'");
        }
        fstat(source, &stat_buf);

        destination = open(destinationPath.Ptr(), createFlags, stat_buf.st_mode);
        if (destination == -1)
        {
            throw Exception("Unable to copy to '" + destinationPath + "'");
        }
        while ((size = read(source, buf, BUFSIZ)) > 0) {
            write(destination, buf, size);
        }
        close (source);
        close (destination);
    }

    bool UnixFileSystem::GetFileInfo(const String& path, FileInfo& f)
    {
        struct stat attributes;

        if (stat(path.Ptr(), &attributes) == -1)
            return false;

        f.Name = path;

        f.Attributes = 0;
        if (!(((attributes.st_mode & S_IWOTH) == S_IWOTH)
            || (attributes.st_gid == getgid() && ((attributes.st_mode & S_IWGRP) == S_IWGRP))
            || (attributes.st_uid == getuid() && ((attributes.st_mode & S_IWUSR) == S_IWUSR))))
            f.Attributes |= FileAttributesReadOnly;

        if (S_ISDIR(attributes.st_mode))
            f.Attributes |= FileAttributesDirectory;

        f.Size = attributes.st_size;
        f.CreationTime = ConvertToTimestamp(attributes.st_mtime);
        f.LastAccessTime = ConvertToTimestamp(attributes.st_atime);
        f.LastWriteTime = ConvertToTimestamp(attributes.st_mtime);

        return true;
    }

    void UnixFileSystem::GetFiles(const String& path, Array<FileInfo>& list)
    {
        String prefix =
            path.Length() > 0 && path.Last() != '/' ?
                path + "/" :
                path;

        DIR *dp;
        struct dirent *ep;

        if ((dp = opendir(prefix.Ptr())) == NULL)
            throw FileNotFoundException(prefix);

        if (prefix == "./")
            prefix = "";

        while ((ep = readdir(dp)) != NULL)
        {
            String fn = ep->d_name;

            if (fn == "." || fn == "..")
                continue;

            FileInfo info;
            if (GetFileInfo(prefix + fn, info))
                list.Add(info);
        }

        closedir(dp);
    }
}
