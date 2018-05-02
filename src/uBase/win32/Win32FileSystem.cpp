#include <uBase/Console.h>
#include <uBase/Path.h>
#include <uBase/Random.h>
#include <uBase/Unicode.h>
#include <uBase/Disk.h>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <ShlObj.h>
#undef GetSystemDirectory
#undef GetCurrentDirectory
#undef SetCurrentDirectory
#undef CreateDirectory
#undef ChangeDirectory
#undef DeleteFile
#undef MoveFile
#undef CopyFile

namespace uBase
{
    static uint64_t ConvertToUInt64(const DWORD& high, const DWORD& low)
    {
        return ((uint64_t)high << 32) | low;
    }

    static uint64_t ConvertToTimestamp(const FILETIME& ft)
    {
        return ConvertToUInt64(ft.dwHighDateTime, ft.dwLowDateTime);
    }

    static String GetLastErrorString()
    {
        LPWSTR lpMsgBuf;
        DWORD dw = GetLastError();

        FormatMessageW(
            FORMAT_MESSAGE_ALLOCATE_BUFFER |
            FORMAT_MESSAGE_FROM_SYSTEM |
            FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL,
            dw,
            MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT),
            (LPWSTR)&lpMsgBuf, // Cast because callee is allocating buffer
            0, NULL);

        String msg = Unicode::Utf16To8(lpMsgBuf);
        LocalFree(lpMsgBuf);

        return msg;
    }

    class Win32FileSystem : public NativeFileSystem
    {
    public:
        Stream* OpenFile(const String& filename, FileMode mode)
        {
            return new File(filename, mode);
        }

        virtual String GetBaseDirectory()
        {
            const size_t BufSize = 4096;

            WCHAR buf[BufSize];
            DWORD result = GetModuleFileNameW(GetModuleHandle(0), buf, BufSize);

            if (result > 0 && result < BufSize)
            {
                String exe = Unicode::Utf16To8(buf, (int)result);

                for (int i = 0; i < exe.Length(); i++)
                    if (exe[i] == '\\')
                        exe[i] = '/';

                return Path::GetDirectoryName(exe);
            }

            return "";
        }

        virtual String CreateTempFilename()
        {
            Random rand(GetTickCount());
            static const char* cs = "-_1234567890abcdefghijklmnopqrstuvwxyz";
            int cl = (int)strlen(cs);

            WCHAR preW[] =
            {
                (WCHAR)(cs[rand.NextInt(cl)]),
                (WCHAR)(cs[rand.NextInt(cl)]),
                (WCHAR)(cs[rand.NextInt(cl)]),
                0
            };

            WCHAR pathW[MAX_PATH];
            WCHAR fnW[MAX_PATH];
            GetTempPathW(MAX_PATH, pathW);
            GetTempFileNameW(pathW, preW, 0, fnW);

            String result = Unicode::Utf16To8(fnW);

            for (int i = 0; i < result.Length(); i++)
                if (result[i] == '\\')
                    result[i] = '/';

            return result;
        }

        static String GetKnownDirectory(REFKNOWNFOLDERID rfid)
        {
            String result = ".";
            PWSTR pszPathW;

            if (::SHGetKnownFolderPath(rfid, KF_FLAG_NO_ALIAS, 0, &pszPathW) == S_OK)
            {
                result = Unicode::Utf16To8(pszPathW);
                CoTaskMemFree(pszPathW);
            }

            for (int i = 0; i < result.Length(); i++)
                if (result[i] == '\\')
                    result[i] = '/';

            return result;
        }

        virtual String GetSystemDirectory(SystemDirectory dir)
        {
            switch (dir)
            {
            default:
                Error->WriteLine("WARNING: Unknown system directory: " + (String)(int)dir);
                return GetKnownDirectory(FOLDERID_Documents);

            case SystemDirectoryDocuments:
                return GetKnownDirectory(FOLDERID_Documents);

            case SystemDirectoryConfig:
                return GetKnownDirectory(FOLDERID_RoamingAppData);

            case SystemDirectoryData:
                return GetKnownDirectory(FOLDERID_LocalAppData);
            }
        }

        virtual String GetCurrentDirectory()
        {
            WCHAR bufW[4096];
            ::GetCurrentDirectoryW(4096, bufW);

            String result = Unicode::Utf16To8(bufW);

            for (int i = 0; i < result.Length(); i++)
                if (result[i] == '\\')
                    result[i] = '/';

            return result;
        }

        virtual void SetCurrentDirectory(const String& dir)
        {
            StringU16 dirW = Unicode::Utf8To16(dir);
            ::SetCurrentDirectoryW(dirW.Ptr());
        }

        virtual void CreateDirectory(const String& path)
        {
            // silent ignore on disk roots
            if (path.EndsWith(":"))
                return;

            StringU16 pathW = Unicode::Utf8To16(path);

            if (!CreateDirectoryW(pathW.Ptr(), 0))
            {
                DWORD err = GetLastError();
                switch (err)
                {
                case ERROR_ALREADY_EXISTS:
                    break;

                case ERROR_PATH_NOT_FOUND:
                default:
                    throw Exception("Couldn't create directory: " + path);
                };
            }
        }

        virtual void DeleteDirectory(const String& name)
        {
            StringU16 nameW = Unicode::Utf8To16(name);

            if (!RemoveDirectoryW(nameW.Ptr()))
            {
                throw Exception("Unable to delete directory '" + name + "': " + GetLastErrorString());
            }
        }

        virtual void DeleteFile(const String& name)
        {
            StringU16 nameW = Unicode::Utf8To16(name);

            if (!DeleteFileW(nameW.Ptr()))
                throw Exception("Unable to delete file '" + name + "': " + GetLastErrorString());
        }

        virtual void MoveDirectory(const String& oldName, const String& newName)
        {
            StringU16 oldNameW = Unicode::Utf8To16(oldName);
            StringU16 newNameW = Unicode::Utf8To16(newName);

            if (!MoveFileW(oldNameW.Ptr(), newNameW.Ptr()))
                throw Exception("Unable to move directory '" + oldName + "' to '" + newName + "': " + GetLastErrorString());
        }

        virtual void MoveFile(const String& oldName, const String& newName)
        {
            StringU16 oldNameW = Unicode::Utf8To16(oldName);
            StringU16 newNameW = Unicode::Utf8To16(newName);

            if (!MoveFileW(oldNameW.Ptr(), newNameW.Ptr()))
                throw Exception("Unable to move file '" + oldName + "' to '" + newName + "': " + GetLastErrorString());
        }

        virtual void CopyFile(const String& sourceName, const String& destinationName, bool overwrite)
        {
            StringU16 sourceNameW = Unicode::Utf8To16(sourceName);
            StringU16 destinationNameW = Unicode::Utf8To16(destinationName);

            if (!CopyFileW(sourceNameW.Ptr(), destinationNameW.Ptr(), !overwrite))
                throw Exception("Unable to copy '" + sourceName + "' to '" + destinationName + "': " + GetLastErrorString());
        }

        virtual bool GetFileInfo(const String& path, FileInfo& info)
        {
            StringU16 pathW = Unicode::Utf8To16(path);

            WIN32_FILE_ATTRIBUTE_DATA data;

            if (GetFileAttributesEx(pathW.Ptr(), GetFileExInfoStandard, &data))
            {
                info.Name = path;
                info.Attributes = 0;
                info.LastAccessTime = ConvertToTimestamp(data.ftLastAccessTime);
                info.LastWriteTime = ConvertToTimestamp(data.ftLastWriteTime);
                info.CreationTime = ConvertToTimestamp(data.ftCreationTime);
                info.Size = ConvertToUInt64(data.nFileSizeHigh, data.nFileSizeLow);

                if (data.dwFileAttributes & FILE_ATTRIBUTE_READONLY)
                    info.Attributes |= FileAttributesReadOnly;

                if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
                    info.Attributes |= FileAttributesDirectory;

                return true;
            }

            return false;
        }

        virtual void GetFiles(const String& path, Array<FileInfo>& list)
        {
            String filter = path.Length() ?
                path + (path.Last() == '/' ? "*" : "/*") :
                "*";

            StringU16 filterW = Unicode::Utf8To16(filter);

            WIN32_FIND_DATA findData;
            HANDLE h = FindFirstFile((LPCWSTR)filterW.Ptr(), &findData);

            if (h != INVALID_HANDLE_VALUE)
            {
                do
                {
                    FileInfo info;
                    info.Name = Unicode::Utf16To8(findData.cFileName);

                    if (info.Name == "." || info.Name == "..")
                        continue;

                    if (path.Length() && path != "." && path != "./")
                    {
                        if (path.Last() == '/')
                            info.Name = path + info.Name;
                        else
                            info.Name = path + "/" + info.Name;
                    }

                    info.Attributes = 0;
                    info.LastAccessTime = ConvertToTimestamp(findData.ftLastAccessTime);
                    info.LastWriteTime = ConvertToTimestamp(findData.ftLastWriteTime);
                    info.CreationTime = ConvertToTimestamp(findData.ftCreationTime);
                    info.Size = ConvertToUInt64(findData.nFileSizeHigh, findData.nFileSizeLow);

                    if (findData.dwFileAttributes & FILE_ATTRIBUTE_READONLY)
                        info.Attributes |= FileAttributesReadOnly;

                    if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
                        info.Attributes |= FileAttributesDirectory;

                    list.Add(info);

                }
                while (FindNextFile(h, &findData) != 0);
            }

            FindClose(h);
        }
    };

    NativeFileSystem* CreateNativeFileSystem()
    {
        return new Win32FileSystem();
    }
}
