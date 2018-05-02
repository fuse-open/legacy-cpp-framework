#include <uImage/ImageIO.h>
#include <uBase/File.h>
using namespace uBase;

namespace uImage
{
    Buffer* ImageReader::ReadBuffer(ProgressCallback* callback)
    {
        Buffer* buf = Buffer::Create(GetBufferSize());
        Read(buf->Ptr(), callback);
        return buf;
    }

    Bitmap* ImageReader::ReadBitmap(ProgressCallback* callback)
    {
        Auto<Buffer> buf = ReadBuffer(callback);
        Bitmap* bmp = new Bitmap(GetWidth(), GetHeight(), GetFormat(), buf, GetWidth() * FormatInfo::SizeOf(GetFormat()), 0);
        return bmp;
    }

    void ImageWriter::WriteBitmap(Bitmap* bmp, ProgressCallback* callback)
    {
        if (bmp->GetWidth() != GetWidth()) throw ArgumentException("Bitmap must have same width as ImageWriter");
        if (bmp->GetHeight() != GetHeight()) throw ArgumentException("Bitmap must have same height as ImageWriter");
        if (bmp->GetFormat() != GetFormat()) throw ArgumentException("Bitmap must have same format as ImageWriter");
        Write(bmp->GetPtr(), bmp->GetPitch(), callback);
    }

    void ImageWriter::WriteImage(Image* img, ProgressCallback* callback)
    {
        Auto<Bitmap> bmp = img->ToBitmap();
        WriteBitmap(bmp, callback);
    }
}
