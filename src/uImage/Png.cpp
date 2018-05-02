#include <uBase/File.h>
#include <uBase/Memory.h>
#include <uImage/Png.h>
#include <cstring>
#include <png.h>
using namespace uBase;

namespace uImage
{
    class PngReader: public ImageReader
    {
        Shared<Stream> f;

        png_structp png_ptr;
        png_infop info_ptr;

        uint32_t width, height;
        Format format;
        int comps;
        bool isInterlaced;
        int interlacePasses;

        static void read(png_structp png_ptr, png_bytep data, png_size_t length)
        {
            Stream* stream = (Stream*)png_get_io_ptr(png_ptr);
            try
            {
                int bytes_read = stream->Read(data, 1, (int)length);
                if (bytes_read != length)
                    png_error(png_ptr, "Failed to read");
            }
            catch (const Exception &e)
            {
                png_error(png_ptr, e.what());
            }
        }

        static void error(png_structp png_ptr, png_const_charp err)
        {
            throw Exception(String("Error while reading from PNG file: ") + err);
        }

    public:
        PngReader(Stream* input)
        {
            char header[8];
            if (input->Read(header, 1, 8) != 8)
            {
                throw Exception("Not a PNG file");
            }

            if (png_sig_cmp((png_const_bytep)header, 0, 8))
            {
                throw Exception("Not a PNG file");
            }

            f = input;

            png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, error, NULL);
            if (!png_ptr)
            {
                throw Exception("png_create_read_struct failed");
            }

            info_ptr = png_create_info_struct(png_ptr);
            if (!info_ptr)
            {
                png_destroy_read_struct(&png_ptr, (png_infopp) NULL, (png_infopp) NULL);
                throw Exception("png_create_info_struct failed");
            }

            png_set_read_fn (png_ptr, (png_voidp)f.Get(), read);

            png_set_sig_bytes(png_ptr, 8);
            png_read_info(png_ptr, info_ptr);

            int bit_depth = png_get_bit_depth (png_ptr, info_ptr);
            int color_type = png_get_color_type (png_ptr, info_ptr);

            if (color_type == PNG_COLOR_TYPE_PALETTE) png_set_palette_to_rgb(png_ptr);
            else if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8) png_set_expand_gray_1_2_4_to_8(png_ptr);

            if (png_get_valid (png_ptr, info_ptr, PNG_INFO_tRNS)) png_set_tRNS_to_alpha (png_ptr);

            if (bit_depth == 16) png_set_strip_16 (png_ptr);
            else if (bit_depth < 8) png_set_packing (png_ptr);

            isInterlaced = png_get_interlace_type (png_ptr, info_ptr) != 0;
            interlacePasses = isInterlaced ? png_set_interlace_handling (png_ptr) : 0;
            png_read_update_info (png_ptr, info_ptr);

            png_get_IHDR (png_ptr, info_ptr,
                          (png_uint_32*)(&width),
                          (png_uint_32*)(&height),
                          &bit_depth, &color_type,
                          NULL, NULL, NULL);

            switch (color_type)
            {
            case PNG_COLOR_TYPE_GRAY:
                format = FormatL_8_UInt_Normalize;
                comps = 1;
                break;

            case PNG_COLOR_TYPE_GRAY_ALPHA:
                format = FormatLA_8_8_UInt_Normalize;
                comps = 2;
                break;

            case PNG_COLOR_TYPE_RGB:
                format = FormatRGB_8_8_8_UInt_Normalize;
                comps = 3;
                break;

            case PNG_COLOR_TYPE_RGB_ALPHA:
                format = FormatRGBA_8_8_8_8_UInt_Normalize;
                comps = 4;
                break;

            default:
                throw Exception("Unsupported PNG pixel format");
                break;
            }
        }

        virtual ~PngReader()
        {
            png_destroy_read_struct (&png_ptr, &info_ptr, NULL);
        }

        virtual int GetWidth()
        {
            return width;
        }

        virtual int GetHeight()
        {
            return height;
        }

        virtual int GetDepth()
        {
            return 0;
        }

        virtual Format GetFormat()
        {
            return format;
        }

        virtual int GetBufferSize()
        {
            return comps * width * height;
        }

        virtual void Read(void* targetBuffer, ProgressCallback* callback)
        {
            png_bytep row = (png_bytep)targetBuffer;
            int pitch = width * comps;

            if (isInterlaced)
            {
                png_bytep *row_pointers = new png_bytep[height];
                for (unsigned int y = 0; y < height; y++)
                    row_pointers[y] = new png_byte[png_get_rowbytes(png_ptr, info_ptr)];

                for (int i = 0; i < interlacePasses; i++)
                {
                    for (unsigned int y = 0; y < height; y++)
                    {
                        if (callback) callback->Update(i * height + y, interlacePasses * height);
                        png_read_row (png_ptr, row_pointers[y], NULL);
                    }
                }

                for (unsigned int y = 0; y < height; y++)
                {
                    if (callback) callback->Update(y, height);
                    memcpy(row, row_pointers[y], pitch);
                    row += pitch;
                }

                for (unsigned int y = 0; y < height; y++)
                    delete [] row_pointers[y];
                delete [] row_pointers;
            }
            else
            {
                for (unsigned int i = 0; i < height; ++i)
                {
                    if (callback) callback->Update(i, height);
                    png_read_row(png_ptr, row, NULL);
                    row += pitch;
                }
            }

            png_read_end(png_ptr, NULL);
            if (callback) callback->Update(height, height);
        }
    };

    class PngWriter: public ImageWriter
    {
        Shared<Stream> f;
        Format _format;
        png_structp png_ptr;
        png_infop info_ptr;
        int comps;

        static void write(png_structp png_ptr, png_bytep data, png_size_t length)
        {
            ((Stream*)png_get_io_ptr(png_ptr))->Write(data, 1, (int)length);
        }

        static void flush(png_structp png_ptr)
        {
            ((Stream*)png_get_io_ptr(png_ptr))->Flush();
        }

        static void error(png_structp png_ptr, png_const_charp)
        {
            throw Exception("Error while writing to PNG file");
        }

    public:
        PngWriter(Stream* output, int width, int height, Format format)
        {
            f = output;

            png_byte color_type;
            const png_byte bit_depth = 8;
            _format = format;

            switch (format)
            {
            case FormatL_8_UInt_Normalize:
                color_type = PNG_COLOR_TYPE_GRAY;
                comps = 1;
                break;

            case FormatLA_8_8_UInt_Normalize:
                color_type = PNG_COLOR_TYPE_GRAY_ALPHA;
                comps = 2;
                break;

            case FormatRGB_8_8_8_UInt_Normalize:
                color_type = PNG_COLOR_TYPE_RGB;
                comps = 3;
                break;

            case FormatRGBA_8_8_8_8_UInt_Normalize:
                color_type = PNG_COLOR_TYPE_RGB_ALPHA;
                comps = 4;
                break;

            default:
                throw Exception(String("Unsupported PNG Format: ") + FormatInfo::ToString(format));
                break;
            }

            png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, error, NULL);
            if (!png_ptr) throw Exception("Error creating write struct");

            info_ptr = png_create_info_struct(png_ptr);
            if (!info_ptr) throw Exception("Error creating info struct");

            png_set_write_fn(png_ptr, (png_voidp)f.Get(), write, flush);

            png_set_IHDR(png_ptr, info_ptr, width, height,
                bit_depth, color_type, PNG_INTERLACE_NONE,
                PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

            png_write_info(png_ptr, info_ptr);
        }

        virtual ~PngWriter()
        {
            png_destroy_write_struct(&png_ptr, &info_ptr);
        }

        virtual int GetWidth()
        {
            return png_get_image_width(png_ptr, info_ptr);
        }

        virtual int GetHeight()
        {
            return png_get_image_height(png_ptr, info_ptr);
        }

        virtual int GetDepth()
        {
            return 0;
        }

        virtual int GetBufferSize()
        {
            return comps * png_get_image_width(png_ptr, info_ptr) * png_get_image_height(png_ptr, info_ptr);
        }

        virtual Format GetFormat()
        {
            return _format;
        }

        virtual void Write(void* sourceBuffer, int pitch, ProgressCallback* callback)
        {
            png_bytep row = (png_bytep)sourceBuffer;
            int height = png_get_image_height(png_ptr, info_ptr);

            for (int i = 0; i < height; i++)
            {
                if (callback) callback->Update(i, height);
                png_write_row(png_ptr, row);
                row += pitch;
            }

            png_write_end(png_ptr, NULL);
            if (callback) callback->Update(height, height);
        }
    };

    ImageReader* Png::CreateReader(Stream* input)
    {
        return new PngReader(input);
    }

    ImageWriter* Png::CreateWriter(Stream* output, int width, int height, Format format)
    {
        return new PngWriter(output, width, height, format);
    }

    Bitmap* Png::Load(Stream* input)
    {
        PngReader r(input);
        return r.ReadBitmap();
    }

    Bitmap* Png::Load(const String& filename)
    {
        File f(filename, FileModeRead);
        return Load(&f);
    }

    void Png::Save(Stream* output, Bitmap* bmp)
    {
        PngWriter w(output, bmp->GetWidth(), bmp->GetHeight(), bmp->GetFormat());
        w.WriteBitmap(bmp);
    }

    void Png::Save(const String& filename, Bitmap* bmp)
    {
        File f(filename, FileModeWrite);
        Save(&f, bmp);
    }
}
