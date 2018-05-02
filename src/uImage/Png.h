#pragma once
#include <uBase/Stream.h>
#include <uImage/ImageIO.h>

namespace uImage
{
    /** \ingroup uImage */
    class Png
    {
    public:
        static ImageWriter* CreateWriter(uBase::Stream* output, int width, int height, Format format);
        static ImageReader* CreateReader(uBase::Stream* input);

        static Bitmap* Load(uBase::Stream* input);
        static Bitmap* Load(const uBase::String& filename);

        static void Save(uBase::Stream* output, Bitmap* bmp);
        static void Save(const uBase::String& filename, Bitmap* bmp);
    };
}
