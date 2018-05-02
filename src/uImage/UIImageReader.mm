#include <uBase/File.h>
#include <uBase/Memory.h>
#include <uImage/ImageReader.h>
#include <uImage/Jpeg.h>
#include <uImage/Png.h>
#import <UIKit/UIImage.h>
using namespace uBase;

namespace uImage
{
    class UIImageReader : public ImageReader
    {
        Shared<Stream> stream;
        Auto<DataAccessor> buf;

        UIImage* image;

    public:
        UIImageReader(Stream* stream)
        {
            this->stream = stream;
            buf = stream->CreateDataAccessor();

            NSData* data = [[NSData alloc] initWithBytesNoCopy:(void*)buf->GetPtr() length:buf->GetSizeInBytes() freeWhenDone:NO];

            if (data == nil)
                throw Exception("Failed to open image: Unable to create NSData object");

            image = [[UIImage alloc] initWithData:data];
            [data release];

            if (image == nil)
                throw Exception("Failed to open image: Unable to create NSImage object");
        }

        virtual ~UIImageReader()
        {
            [image release];
        }

        virtual int GetWidth()
        {
            return CGImageGetWidth(image.CGImage);
        }

        virtual int GetHeight()
        {
            return CGImageGetHeight(image.CGImage);
        }

        virtual int GetDepth()
        {
            return 0;
        }

        int GetComponentCount()
        {
            int c = 0;
            CGColorSpaceRef cs = CGImageGetColorSpace(image.CGImage);
            CGColorSpaceRef bcs = CGColorSpaceGetBaseColorSpace(cs);
            if (bcs==NULL)
            {
                c = CGColorSpaceGetNumberOfComponents(cs);
            } else {
                c = CGColorSpaceGetNumberOfComponents(bcs);
            }
            if (CGImageGetAlphaInfo(image.CGImage) != kCGImageAlphaNone) {
                c+=1;
            }
            return c;
        }

        virtual int GetBufferSize()
        {
            return GetWidth() * GetHeight() * FormatInfo::SizeOf(GetFormat());
        }

        virtual Format GetFormat()
        {
            switch (GetComponentCount())
            {
                case 1: return FormatL_8_UInt_Normalize;
                case 3: return FormatRGB_8_8_8_UInt_Normalize;
                default: return FormatRGBA_8_8_8_8_UInt_Normalize;
            }
        }

        virtual void Read(void* targetBuffer, ProgressCallback* callback)
        {
            int width = CGImageGetWidth(image.CGImage);
            int height = CGImageGetHeight(image.CGImage);

            Format format = GetFormat();
            int cc = FormatInfo::SizeOf(format);

            if (format == FormatRGBA_8_8_8_8_UInt_Normalize)
            {
                CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
                CGContextRef imgcontext = CGBitmapContextCreate(targetBuffer, width, height, 8, 4 * width, colorSpace,
                                                                kCGImageAlphaPremultipliedLast | kCGBitmapByteOrder32Big);
                CGColorSpaceRelease(colorSpace);
                CGContextClearRect(imgcontext, CGRectMake(0, 0, width, height));
                CGContextTranslateCTM(imgcontext, 0, 0);
                CGContextDrawImage(imgcontext, CGRectMake(0, 0, width, height), image.CGImage);
                CGContextRelease(imgcontext);
            }
            else
            {
                Auto<Buffer> tmp = Buffer::Create(4 * width * height);

                CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
                CGContextRef imgcontext = CGBitmapContextCreate(tmp->Ptr(), width, height, 8, 4 * width, colorSpace, kCGImageAlphaPremultipliedLast | kCGBitmapByteOrder32Big);
                CGColorSpaceRelease(colorSpace);
                CGContextClearRect(imgcontext, CGRectMake(0, 0, width, height));
                CGContextTranslateCTM(imgcontext, 0, 0);
                CGContextDrawImage(imgcontext, CGRectMake(0, 0, width, height), image.CGImage);
                CGContextRelease(imgcontext);

                uint8_t* src = (uint8_t*)tmp->Ptr();
                uint8_t* dst = (uint8_t*)targetBuffer;

                for (int i = 0; i < width * height; i++)
                {
                    for (int c = 0; c < 4; c++)
                    {
                        if (c < cc) *dst++ = *src;
                        src++;
                    }
                }
            }
        }
    };

    ImageReader* Jpeg::CreateReader(Stream* input)
    {
        return new UIImageReader(input);
    }

    Bitmap* Jpeg::Load(Stream* input)
    {
        Auto<ImageReader> r = CreateReader(input);
        return r->ReadBitmap();
    }

    Bitmap* Jpeg::Load(const String& filename)
    {
        Auto<File> f = new File(filename, FileModeRead);
        return Load(f);
    }

    ImageReader* Png::CreateReader(Stream* input)
    {
        return new UIImageReader(input);
    }

    Bitmap* Png::Load(Stream* input)
    {
        Auto<ImageReader> r = CreateReader(input);
        return r->ReadBitmap();
    }

    Bitmap* Png::Load(const String& filename)
    {
        Auto<File> f = new File(filename, FileModeRead);
        return Load(f);
    }
}
