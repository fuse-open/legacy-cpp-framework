#include <uBase/Path.h>
#include <uBase/Exception.h>
#include <uBase/String.h>

namespace uBase
{
    static bool IsRooted(const String& path)
    {
        return
            (path.Length() > 0 && (path[0] == '/' || path[0] == '\\')) ||
            (path.Length() > 1 && path[1] == ':');
    }

    String Path::Combine(const String& path1, const String& path2)
    {
        return
            IsRooted(path2) ?
                path2 :
                path1.Length() && path1.Last() != '/' && path1.Last() != '\\' ?
                    path1 + "/" + path2 :
                    path1 + path2;
    }

    String Path::GetFilename(const String& path)
    {
        for (int i = path.Length() - 1; i >= 0; i--)
            switch (path[i])
            {
                case '/':
                case '\\':
                    return path.Substring(i + 1);
            }

        return path;
    }

    String Path::GetFilenameWithoutExtension(const String& path)
    {
        String filename = GetFilename(path);

        for (int i = filename.Length() - 1; i >= 0; i--)
            if (filename[i] == '.')
                return filename.Substring(0, i);

        return filename;
    }

    String Path::GetExtension(const String& path)
    {
        for (int i = path.Length() - 1; i >= 0; i--)
            switch (path[i])
            {
            case '.':
                return path.Substring(i);

            case '/':
            case '\\':
                return "";
            }

        return "";
    }

    String Path::GetDirectoryName(const String& path)
    {
        for (int i = path.Length() - 1; i >= 0; i--)
            switch (path[i])
            {
            case '/':
            case '\\':
                return path.Substring(0, i);
            }

        return "";
    }
}
