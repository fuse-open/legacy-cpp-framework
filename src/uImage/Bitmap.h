#pragma once
#include <uBase/Buffer.h>
#include <uBase/Vector2.h>
#include <uBase/Vector4.h>
#include <uImage/Rectangle.h>
#include <uImage/Image.h>

namespace uImage
{
    /** \ingroup uImage */
    enum RotateAngle
    {
        RotateAngle90CW,
        RotateAngle90CCW,
        RotateAngle180,
    };

    /** \ingroup uImage */
    class Bitmap: public Image
    {
        Format _format;
        int _width, _height, _pitch;
        uBase::DataAccessor* _dataOwner;
        uint8_t* _data;

    public:
        Bitmap(int width, int height, Format format);
        Bitmap(int width, int height, Format format, uBase::DataAccessor* data, int pitch, int offset);
        virtual ~Bitmap();

        Format GetFormat() const;
        int GetWidth() const;
        int GetHeight() const;
        int GetDepth() const;
        int GetPitch() const;
        int GetComponentCount() const;
        int GetBytesPerPixel() const;
        Recti GetRect() const;

        uBase::Vector4u8 GetPixelColor(int x, int y);
        void SetPixelColor(int x, int y, const uBase::Vector4u8& color);

        /** Returns a version of this bitmap where pixels are packed (pitch = pixelsize*width).
            If this bitmap is already packed, then a new reference is added and this is returned. */
        Bitmap* Pack();
        Bitmap* ToBitmap();
        Bitmap* SubBitmap(const Recti& rect);
        Bitmap* ConvertTo(Format format);
        Bitmap* Rotate(RotateAngle angle);

        Bitmap* DownSample2x2();
        void GenerateMipMaps(uBase::Array<Bitmap*>& chain);

        void Clear();
        void Blit(Bitmap* src, Recti srcRect, uBase::Vector2i dstPos);

        virtual int GetSizeInBytes() const;

        uint8_t* GetPtr();
        virtual const uint8_t* GetPtr() const;

        uint8_t* GetScanlinePtr(int y);
        const uint8_t* GetScanlinePtr(int y) const;

        uint8_t* GetPixelPtr(int x, int y);
        const uint8_t* GetPixelPtr(int x, int y) const;

        uBase::DataAccessor* GetDataAccessor();
    };
}
