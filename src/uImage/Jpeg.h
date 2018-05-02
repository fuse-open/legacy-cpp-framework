#pragma once
#include <uBase/Stream.h>
#include <uImage/ImageIO.h>

namespace uImage
{
    /** \ingroup uImage */
    class Jpeg
    {
    public:
        static ImageWriter* CreateWriter(uBase::Stream* output, int width, int height, Format format, int quality);
        static ImageReader* CreateReader(uBase::Stream* input);

        static Bitmap* Load(uBase::Stream* input);
        static Bitmap* Load(const uBase::String& filename);

        static void Save(uBase::Stream* output, Bitmap* bmp, int quality = 75);
        static void Save(const uBase::String& filename, Bitmap* bmp, int quality = 75);
    };
}
