#include <uImage/Bitmap.h>
#include <uBase/Memory.h>
#include <uBase/Array.h>
#include <cstring>
using namespace uBase;

namespace uImage
{
    Bitmap::Bitmap(int width, int height, Format format)
    {
        _width = width;
        _height = height;
        _format = format;
        _pitch = width * FormatInfo::SizeOf(format);
        _dataOwner = Buffer::Create(_pitch * height);
        _data = const_cast<uint8_t*>(_dataOwner->GetPtr());
    }

    Bitmap::Bitmap(int width, int height, Format format, DataAccessor* dataAccessor, int pitch, int offset)
    {
        if (!dataAccessor)
            throw NullPointerException();

        _width = width;
        _height = height;
        _format = format;
        _pitch = pitch;
        _dataOwner = dataAccessor;
        _dataOwner->Retain();
        _data = const_cast<uint8_t*>(_dataOwner->GetPtr()) + offset;
    }

    Bitmap::~Bitmap()
    {
        _dataOwner->Release();
    }

    Format Bitmap::GetFormat() const
    {
        return _format;
    }

    int Bitmap::GetWidth() const
    {
        return _width;
    }

    int Bitmap::GetHeight() const
    {
        return _height;
    }

    int Bitmap::GetDepth() const
    {
        return 0;
    }

    int Bitmap::GetPitch() const
    {
        return _pitch;
    }

    int Bitmap::GetComponentCount() const
    {
        return FormatInfo::ComponentCount(_format);
    }

    int Bitmap::GetBytesPerPixel() const
    {
        return FormatInfo::SizeOf(_format);
    }

    Recti Bitmap::GetRect() const
    {
        return Recti(0, 0, _width, _height);
    }

    int Bitmap::GetSizeInBytes() const
    {
        return _height * _pitch;
    }

    uint8_t* Bitmap::GetPtr()
    {
        return _data;
    }

    const uint8_t* Bitmap::GetPtr() const
    {
        return _data;
    }

    uint8_t* Bitmap::GetScanlinePtr(int y)
    {
        return _data + _pitch * y;
    }

    const uint8_t* Bitmap::GetScanlinePtr(int y) const
    {
        return _data + _pitch * y;
    }

    uint8_t* Bitmap::GetPixelPtr(int x, int y)
    {
        return _data + _pitch * y + GetBytesPerPixel() * x;
    }

    const uint8_t* Bitmap::GetPixelPtr(int x, int y) const
    {
        return _data + _pitch * y + GetBytesPerPixel() * x;
    }

    DataAccessor* Bitmap::GetDataAccessor()
    {
        return _dataOwner;
    }

    Bitmap* Bitmap::Pack()
    {
        if (_pitch == FormatInfo::SizeOf(_format) * _width)
            return SharePtr(this);

        // TODO: Implement this
        throw NotSupportedException(U_FUNCTION);
    }

    Bitmap* Bitmap::ToBitmap()
    {
        return SharePtr(this);
    }

    Bitmap* Bitmap::SubBitmap(const Recti& rect)
    {
        if (rect.Left >= 0 && rect.Right <= _width && rect.Top >= 0 && rect.Bottom <= _height)
        {
            int offset = rect.Top * _pitch + rect.Left * GetBytesPerPixel() + (int)(_dataOwner->GetPtr() - _data);
            return new Bitmap(rect.Width(), rect.Height(), _format, _dataOwner, _pitch, offset);
        }

        throw IndexOutOfRangeException();
    }

    Vector4u8 Bitmap::GetPixelColor(int x, int y)
    {
        switch (_format)
        {
        case FormatRGBA_8_8_8_8_UInt_Normalize:
            return *(Vector4u8*)GetPixelPtr(x, y);

        case FormatRGB_8_8_8_UInt_Normalize:
            return Vector4u8(Vector3u8(*(Vector3u8*)GetPixelPtr(x, y)), (uint8_t)0xff);

        default:
            throw Exception("Bitmap::GetPixelColor: unsupported pixel format");
        }
    }

    void Bitmap::SetPixelColor(int x, int y, const Vector4u8& color)
    {
        switch (_format)
        {
        case FormatRGBA_8_8_8_8_UInt_Normalize:
            *(Vector4u8*)GetPixelPtr(x,y) = color;
            break;

        case FormatRGB_8_8_8_UInt_Normalize:
            *(Vector3u8*)GetPixelPtr(x, y) = color.XYZ();
            break;

        default:
            throw Exception("Bitmap::SetPixelColor: unsupported pixel format");
        }
    }

    Bitmap* Bitmap::ConvertTo(Format dstFormat)
    {
        if (dstFormat == _format)
        {
            return SharePtr(this);
        }
        else if (_format == FormatL_8_UInt_Normalize && dstFormat == FormatRGBA_8_8_8_8_UInt_Normalize)
        {
            Bitmap* bmp = new Bitmap(_width, _height, FormatRGBA_8_8_8_8_UInt_Normalize);
            Vector4u8* dst = (Vector4u8*)bmp->GetPtr();

            for (int y = 0; y < _height; y++)
            {
                uint8_t* src = GetScanlinePtr(y);

                for (int x = 0; x < _width; x++)
                {
                    uint8_t c = *src++;
                    (*dst).R = c;
                    (*dst).G = c;
                    (*dst).B = c;
                    (*dst).A = (uint8_t)0xff;
                    dst++;
                }
            }

            return bmp;
        }
        else if (_format == FormatL_8_UInt_Normalize && dstFormat == FormatRGB_8_8_8_UInt_Normalize)
        {
            Bitmap* bmp = new Bitmap(_width, _height, FormatRGB_8_8_8_UInt_Normalize);
            uint8_t* dst = bmp->GetPtr();

            for (int y = 0; y < _height; y++)
            {
                uint8_t* src = GetScanlinePtr(y);

                for (int x = 0; x < _width; x++)
                {
                    uint8_t c = *src++;
                    *dst++ = c;
                    *dst++ = c;
                    *dst++ = c;
                }
            }

            return bmp;
        }
        else if (_format == FormatRGB_8_8_8_UInt_Normalize && dstFormat == FormatRGBA_8_8_8_8_UInt_Normalize)
        {
            Bitmap* bmp = new Bitmap(_width, _height, FormatRGBA_8_8_8_8_UInt_Normalize);
            Vector4u8* dst = (Vector4u8*)bmp->GetPtr();

            for (int y = 0; y < _height; y++)
            {
                uint8_t* src = GetScanlinePtr(y);

                for (int x = 0; x < _width; x++)
                {
                    (*dst).R = *src++;
                    (*dst).G = *src++;
                    (*dst).B = *src++;
                    (*dst).A = (uint8_t)0xff;
                    dst++;
                }
            }

            return bmp;
        }
        else if (_format == FormatRGBA_8_8_8_8_UInt_Normalize && dstFormat == FormatRGB_8_8_8_UInt_Normalize)
        {
            Bitmap* bmp = new Bitmap(_width, _height, FormatRGB_8_8_8_UInt_Normalize);
            uint8_t* dst = bmp->GetPtr();

            for (int y = 0; y < _height; y++)
            {
                uint8_t* src = GetScanlinePtr(y);

                for (int x = 0; x < _width; x++)
                {
                    *dst++ = *src++;
                    *dst++ = *src++;
                    *dst++ = *src++;
                    src++;
                }
            }

            return bmp;
        }

        throw Exception(String("Unable to convert ") + FormatInfo::ToString(_format) + " to " + FormatInfo::ToString(dstFormat));
    }

    Bitmap* Bitmap::Rotate(RotateAngle angle)
    {
        Bitmap* target = new Bitmap(_width, _height, _format);

        int bpp = GetBytesPerPixel();

        if (angle == RotateAngle90CW || angle == RotateAngle90CCW)
        {
            target->_width = _height;
            target->_height = _width;
            target->_pitch = target->_width * bpp;
        }

        int dstDeltaX;
        int dstDeltaY;
        int dstOffset;

        switch (angle)
        {
        case RotateAngle90CW:
            dstDeltaX = _height * bpp;
            dstDeltaY = -(_width * _height + 1) * bpp;
            dstOffset = (_height - 1) * bpp;
            break;

        case RotateAngle90CCW:
            dstDeltaX = -_height * bpp;
            dstDeltaY = (_width * _height + 1) * bpp;
            dstOffset = (_width - 1) * _height * bpp;
            break;

        case RotateAngle180:
            dstDeltaX = -bpp;
            dstDeltaY = 0;
            dstOffset = (_width * _height - 1) * bpp;
            break;

        default:
            throw Exception("Can't Rotate: Invalid RotateAngle");
        }

        uint8_t* src = _data;
        uint8_t* dst = target->_data + dstOffset;
        int srcDeltaY = (_pitch - _width * bpp);

        for (int y = 0; y < _height; y++)
        {
            for (int x = 0; x < _width; x++)
            {
                memcpy(dst, src, bpp);
                dst += dstDeltaX;
                src += bpp;
            }

            dst += dstDeltaY;
            src += srcDeltaY;
        }

        return target;
    }

    void Bitmap::Clear()
    {
        memset(_data, 0, _pitch * _height);
    }

    void Bitmap::Blit(Bitmap* src, Recti srcRect, Vector2i dstPos)
    {
        if (srcRect.Left < 0) srcRect.Left = 0;
        if (srcRect.Top < 0) srcRect.Top = 0;
        if (srcRect.Right > src->GetWidth()) srcRect.Right = src->GetWidth();
        if (srcRect.Bottom > src->GetHeight()) srcRect.Bottom = src->GetHeight();

        if (dstPos.X < 0) dstPos.X = 0;
        if (dstPos.Y < 0) dstPos.Y = 0;
        if (dstPos.X + srcRect.Width() > GetWidth()) srcRect.Right = GetWidth() - dstPos.X + srcRect.Left;
        if (dstPos.Y + srcRect.Height() > GetHeight()) srcRect.Bottom = GetHeight() - dstPos.Y + srcRect.Top;

        Auto<Bitmap> temp = src->ConvertTo(_format);

        int srcOffset = temp->_pitch * srcRect.Top + GetBytesPerPixel() * srcRect.Left;
        int dstOffset = _pitch * dstPos.Y + GetBytesPerPixel() * dstPos.X;
        int scanSize = srcRect.Width() * GetBytesPerPixel();

        uint8_t* srcData = temp->_data + srcOffset;
        uint8_t* dstData = _data + dstOffset;

        for (int i = 0; i < srcRect.Height(); i++)
        {
            memcpy(dstData, srcData, scanSize);
            srcData += temp->_pitch;
            dstData += _pitch;
        }
    }

    Bitmap* Bitmap::DownSample2x2()
    {
        int cc = GetComponentCount();
        int bpp = GetBytesPerPixel();

        if (cc != bpp)
        {
            throw Exception("Can't DownSample2x2: Unsupported pixel format");
        }

        int w = _width >> 1; if (w < 1) w = 1;
        int h = _height >> 1; if (h < 1) h = 1;

        Bitmap* target = new Bitmap(w, h, _format);
        uint8_t* dst = target->_data;

        for (int y = 0; y < h; y++)
        {
            int dy = 1; if (y * 2 + 1 >= _height) dy = 0;
            uint8_t* scan0 = GetScanlinePtr(y*2);
            uint8_t* scan1 = GetScanlinePtr(y*2+dy);

            for (int x = 0; x < w; x++)
            {
                int dx = 1; if (x * 2 + 1 >= _width) dx = 0;

                uint8_t* src0 = scan0 + (x*2)*cc;
                uint8_t* src1 = scan0 + (x*2+dx)*cc;
                uint8_t* src2 = scan1 + (x*2)*cc;
                uint8_t* src3 = scan1 + (x*2+dx)*cc;

                for (int c = 0; c < cc; c++)
                {
                    *dst++ = (uint8_t)(((unsigned int)*src0 + *src1 + *src2 + *src3) >> 2);
                    src0++;
                    src1++;
                    src2++;
                    src3++;
                }
            }
        }

        return target;
    }

    void Bitmap::GenerateMipMaps(Array<Bitmap*>& chain)
    {
        chain.Add(SharePtr(this));
        Bitmap* bmp = this;

        while (bmp->GetWidth() > 1 || bmp->GetHeight() > 1)
        {
            bmp = bmp->DownSample2x2();
            chain.Add(bmp);
        }
    }
}
