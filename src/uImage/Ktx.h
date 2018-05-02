#pragma once
#include <uBase/File.h>
#include <uImage/Texture.h>

namespace uImage
{
    /** \ingroup uImage */
    class Ktx
    {
    public:
        static void Save(uBase::Stream* output, Texture* tex);
        static void Save(const uBase::String& filename, Texture* tex);
        static Texture* Load(uBase::Stream* input);
        static Texture* Load(const uBase::String& filename);
    };
}
