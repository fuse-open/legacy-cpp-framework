#include <uBase/File.h>
#include <uBase/Memory.h>
#include <uImage/Jpeg.h>
using namespace uBase;

extern "C"
{
#include <jpeglib.h>
}

namespace uImage
{
    static const int BufSize = 4096;

    struct JpegData
    {
        Shared<Stream> File;
        uint8_t Buf[BufSize];
        JpegData(Stream* f): File(f) {}
    };

    class JpegReader: public ImageReader
    {
        JpegData cd;

        static void initSource (j_decompress_ptr cinfo)
        {
        }

        static boolean fillInputBuffer (j_decompress_ptr cinfo)
        {
            JpegData* cd = reinterpret_cast<JpegData*>(cinfo->client_data);
            struct jpeg_source_mgr *jsrc = cinfo->src;

            jsrc->next_input_byte = cd->Buf;
            jsrc->bytes_in_buffer = (size_t)cd->File->Read(cd->Buf, 1, 1024);

            if (!jsrc->bytes_in_buffer)
                return FALSE;

            return TRUE;
        }

        static void skipInputData(j_decompress_ptr cinfo, long num_bytes)
        {
            JpegData* cd = reinterpret_cast<JpegData*>(cinfo->client_data);
            struct jpeg_source_mgr *jsrc = cinfo->src;

            if ((size_t)num_bytes > jsrc->bytes_in_buffer)
            {
                cd->File->Seek((int)num_bytes - (int)jsrc->bytes_in_buffer, SeekOriginCurrent);
                fillInputBuffer(cinfo);
            }
            else
            {
                jsrc->next_input_byte += num_bytes;
                jsrc->bytes_in_buffer -= num_bytes;
            }
        }

        static void termSource (j_decompress_ptr cinfo)
        {
        }

        static void errorExit(j_common_ptr cinfo)
        {
            char buf[JMSG_LENGTH_MAX];
            (*cinfo->err->format_message)(cinfo, buf);
            throw Exception(String("Error while reading JPEG file: ") + buf);
        }

        jpeg_decompress_struct cinfo;
        jpeg_source_mgr jsrc;
        jpeg_error_mgr pub;

        Format format;

    public:
        JpegReader(Stream* input): cd(input)
        {
            jpeg_create_decompress (&cinfo);

            cinfo.err = jpeg_std_error (&pub);
            cinfo.client_data = (void*)&cd;
            cinfo.src = &jsrc;

            pub.error_exit = errorExit;

            /* configure source manager */
            jsrc.next_input_byte = 0;
            jsrc.bytes_in_buffer = 0;
            jsrc.init_source = initSource;
            jsrc.fill_input_buffer = fillInputBuffer;
            jsrc.skip_input_data = skipInputData;
            jsrc.resync_to_restart = jpeg_resync_to_restart;
            jsrc.term_source = termSource;

            /* read file's header and prepare for decompression */
            if (jpeg_read_header (&cinfo, TRUE) != JPEG_HEADER_OK)
            {
                jpeg_destroy_decompress (&cinfo);
                throw Exception("Not a JPEG file");
            }

            switch (cinfo.num_components)
            {
            case 1:
                format = FormatL_8_UInt_Normalize;
                break;

            case 3:
                format = FormatRGB_8_8_8_UInt_Normalize;
                break;

            default:
                throw Exception("Unsupported JPEG pixel format");
                break;
            }
        }

        virtual ~JpegReader()
        {
            jpeg_destroy_decompress (&cinfo);
        }

        virtual int GetWidth()
        {
            return cinfo.image_width;
        }

        virtual int GetHeight()
        {
            return cinfo.image_height;
        }

        virtual int GetDepth()
        {
            return 0;
        }

        virtual int GetBufferSize()
        {
            return cinfo.image_width * cinfo.image_height * cinfo.num_components;
        }

        virtual Format GetFormat()
        {
            return format;
        }

        virtual void Read(void* targetBuffer, ProgressCallback* callback)
        {
            JSAMPROW j = (JSAMPROW)targetBuffer;
            int pitch = cinfo.image_width * cinfo.num_components;
            jpeg_start_decompress (&cinfo);

            /* read scanlines */
            for (int i = 0; i < cinfo.image_height; i += cinfo.rec_outbuf_height)
            {
                if (callback) callback->Update(i, cinfo.image_height);
                jpeg_read_scanlines (&cinfo, &j, cinfo.rec_outbuf_height);
                j += pitch * cinfo.rec_outbuf_height;
            }

            jpeg_finish_decompress (&cinfo);
            if (callback) callback->Update(cinfo.image_height, cinfo.image_height);
        }
    };

    class JpegWriter: public ImageWriter
    {
        JpegData cd;
        Format _format;
        jpeg_compress_struct cinfo;
        jpeg_destination_mgr jdest;
        jpeg_error_mgr pub;

        static void initDestination(j_compress_ptr cinfo)
        {
            JpegData* cd = reinterpret_cast<JpegData*>(cinfo->client_data);
            struct jpeg_destination_mgr *jdest = cinfo->dest;

            jdest->next_output_byte = cd->Buf;
            jdest->free_in_buffer = BufSize;
        }

        static boolean emptyOutputBuffer(j_compress_ptr cinfo)
        {
            JpegData* cd = reinterpret_cast<JpegData*>(cinfo->client_data);
            struct jpeg_destination_mgr *jdest = cinfo->dest;

            cd->File->Write(cd->Buf, 1, BufSize);

            jdest->next_output_byte = cd->Buf;
            jdest->free_in_buffer = BufSize;

            return TRUE;
        }

        static void termDestination(j_compress_ptr cinfo)
        {
            JpegData* cd = reinterpret_cast<JpegData*>(cinfo->client_data);
            struct jpeg_destination_mgr *jdest = cinfo->dest;

            int d = BufSize - (int)jdest->free_in_buffer;
            if (d) cd->File->Write(cd->Buf, 1, d);

            cd->File->Flush();
        }

        static void errorExit(j_common_ptr cinfo)
        {
            char buf[JMSG_LENGTH_MAX];
            (*cinfo->err->format_message) (cinfo, buf);
            throw Exception(String("Error while writing JPEG file: ") + buf);
        }

    public:
        JpegWriter(Stream* output, int width, int height, Format format, int quality): cd(output)
        {
            jpeg_create_compress(&cinfo);

            switch (format)
            {
            case FormatL_8_UInt_Normalize:
                cinfo.input_components = 1;
                cinfo.in_color_space = JCS_GRAYSCALE;
                break;

            case FormatRGB_8_8_8_UInt_Normalize:
                cinfo.input_components = 3;
                cinfo.in_color_space = JCS_RGB;
                break;

            default:
                throw Exception(String("Unsupported JPEG Format: ") + FormatInfo::ToString(format));
                break;
            }

            _format = format;
            cinfo.image_width = width;
            cinfo.image_height = height;

            cinfo.err = jpeg_std_error(&pub);
            cinfo.dest = &jdest;
            cinfo.client_data = (void*)&cd;

            jdest.next_output_byte = 0;
            jdest.free_in_buffer = 0;
            jdest.init_destination = initDestination;
            jdest.empty_output_buffer = emptyOutputBuffer;
            jdest.term_destination = termDestination;

            pub.error_exit = errorExit;

            jpeg_set_defaults(&cinfo);
            jpeg_set_quality(&cinfo, quality, TRUE);
        }

        virtual ~JpegWriter()
        {
            jpeg_destroy_compress(&cinfo);
        }

        virtual int GetWidth()
        {
            return cinfo.image_width;
        }

        virtual int GetHeight()
        {
            return cinfo.image_height;
        }

        virtual int GetDepth()
        {
            return 0;
        }

        virtual int GetBufferSize()
        {
            return cinfo.image_width * cinfo.image_height * cinfo.input_components;
        }

        virtual Format GetFormat()
        {
            return _format;
        }

        virtual void Write(void* sourceBuffer, int pitch, ProgressCallback* callback)
        {
            jpeg_start_compress(&cinfo, TRUE);
            JSAMPROW j = (JSAMPROW)sourceBuffer;

            while (cinfo.next_scanline < cinfo.image_height)
            {
                if (callback) callback->Update(cinfo.next_scanline, cinfo.image_height);
                jpeg_write_scanlines(&cinfo, &j, 1);
                j += pitch;
            }

            jpeg_finish_compress(&cinfo);
            if (callback) callback->Update(cinfo.image_height, cinfo.image_height);
        }
    };

    ImageReader* Jpeg::CreateReader(Stream* input)
    {
        return new JpegReader(input);
    }

    ImageWriter* Jpeg::CreateWriter(Stream* output, int width, int height, Format format, int quality)
    {
        return new JpegWriter(output, width, height, format, quality);
    }

    Bitmap* Jpeg::Load(Stream* input)
    {
        JpegReader r(input);
        return r.ReadBitmap();
    }

    Bitmap* Jpeg::Load(const String& filename)
    {
        File f(filename, FileModeRead);
        return Load(&f);
    }

    void Jpeg::Save(Stream* output, Bitmap* bmp, int quality)
    {
        JpegWriter w(output, bmp->GetWidth(), bmp->GetHeight(), bmp->GetFormat(), quality);
        w.WriteBitmap(bmp);
    }

    void Jpeg::Save(const String& filename, Bitmap* bmp, int quality)
    {
        File f(filename, FileModeWrite);
        Save(&f, bmp, quality);
    }
}
