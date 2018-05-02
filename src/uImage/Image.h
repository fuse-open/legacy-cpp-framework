#pragma once
#include <uBase/Buffer.h>
#include <uImage/Format.h>

namespace uImage
{
    class Bitmap;

    /** \ingroup uImage */
    class Image: public uBase::DataAccessor
    {
    public:
        virtual int GetWidth() const = 0;
        virtual int GetHeight() const = 0;
        virtual int GetDepth() const = 0;
        virtual Format GetFormat() const = 0;
        virtual Bitmap* ToBitmap() = 0;

        virtual int GetSizeInBytes() const = 0;
        virtual const uint8_t* GetPtr() const = 0;
    };
}
