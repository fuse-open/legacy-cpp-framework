#include <uImage/CompressedImage.h>
using namespace uBase;

namespace uImage
{
    CompressedImage::CompressedImage(int width, int height, int depth, Format format, DataAccessor* data)
    {
        _w = width;
        _h = height;
        _d = depth;
        _format = format;
        _data = data;
    }

    int CompressedImage::GetWidth() const
    {
        return _w;
    }

    int CompressedImage::GetHeight() const
    {
        return _h;
    }

    int CompressedImage::GetDepth() const
    {
        return _d;
    }

    Format CompressedImage::GetFormat() const
    {
        return _format;
    }

    Bitmap* CompressedImage::ToBitmap()
    {
        throw Exception("Unable to convert compressed image with format '" + FormatInfo::ToString(_format) + "' to bitmap");
    }

    int CompressedImage::GetSizeInBytes() const
    {
        return _data->GetSizeInBytes();
    }

    const uint8_t* CompressedImage::GetPtr() const
    {
        return _data->GetPtr();
    }
}
