#pragma once
#include <uBase/String.h>

namespace uBase
{
    /** \ingroup uIO */
    class Path
    {
    public:
        static String Combine(const String& path1, const String& path2);

        static String GetFilename(const String& path);
        static String GetFilenameWithoutExtension(const String& path);

        /** Returns the extension part of the path, including the prefixed dot. An empty string is returned if the path has no extension part */
        static String GetExtension(const String& path);

        /** Returns the directory part of the path, without the trailing slash. '.' is returned if the path has no directory part */
        static String GetDirectoryName(const String& path);
    };
}