#pragma once
#include <uImage/Image.h>
#include <uBase/Memory.h>
#include <uBase/Buffer.h>

namespace uImage
{
    /** \ingroup uImage */
    class CompressedImage : public Image
    {
        int _w, _h, _d;
        Format _format;
        uBase::Shared<uBase::DataAccessor> _data;

    public:
        CompressedImage(int width, int height, int depth, Format format, uBase::DataAccessor* data);

        virtual int GetWidth() const;
        virtual int GetHeight() const;
        virtual int GetDepth() const;
        virtual Format GetFormat() const;
        virtual Bitmap* ToBitmap();

        virtual int GetSizeInBytes() const;
        virtual const uint8_t* GetPtr() const;
    };
}
