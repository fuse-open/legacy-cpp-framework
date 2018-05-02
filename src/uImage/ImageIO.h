#pragma once
#include <uImage/ProgressCallback.h>
#include <uImage/Bitmap.h>
#include <uImage/Format.h>

namespace uImage
{
    /** \ingroup uImage */
    class ImageReader: public uBase::Object
    {
    public:
        virtual ~ImageReader() {}

        virtual int GetWidth() = 0;
        virtual int GetHeight() = 0;
        virtual int GetDepth() = 0;
        virtual int GetBufferSize() = 0;
        virtual Format GetFormat() = 0;

        virtual void Read(void* targetBuffer, ProgressCallback* callback = 0) = 0;

        virtual uBase::Buffer* ReadBuffer(ProgressCallback* callback = 0);
        virtual Bitmap* ReadBitmap(ProgressCallback* callback = 0);
    };

    /** \ingroup uImage */
    class ImageWriter: public uBase::Object
    {
    public:
        virtual ~ImageWriter() {}

        virtual int GetWidth() = 0;
        virtual int GetHeight() = 0;
        virtual int GetDepth() = 0;
        virtual int GetBufferSize() = 0;
        virtual Format GetFormat() = 0;

        virtual void Write(void* sourceBuffer, int pitch, ProgressCallback* callback = 0) = 0;
        virtual void WriteBitmap(Bitmap* bmp, ProgressCallback* callback = 0);
        virtual void WriteImage(Image* img, ProgressCallback* callback = 0);
    };
}
