#pragma once
#include <uBase/FileSystem.h>

namespace uBase
{
    /** \ingroup uIO */
    class BundleAccessor
    {
    public:
        void SetFilesystem(FileSystem* fs);

        FileSystem* operator ->();
        operator FileSystem*();
    };

    /** \ingroup uIO */
    extern BundleAccessor Bundle;
}