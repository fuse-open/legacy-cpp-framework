#include <uImage/Bitmap.h>
#include <uImage/FontFace.h>
#include <ft2build.h>
#include FT_FREETYPE_H
using namespace uBase;

namespace uImage
{
    static FT_Library FreeTypeLibrary;
    static int FreeTypeRefCount = 0;

    class FreeTypeFontFace: public FontFace
    {
        Auto<Buffer> _buf;
        FT_Face _face;

        float FontUnitsToPixels(float pixelSize, FT_Pos units)
        {
            return pixelSize * (float)units / (float)_face->units_per_EM;
        }

        void SetPixelSize(float pixelSize)
        {
            if (FT_Set_Pixel_Sizes(_face, 0, (FT_UInt)pixelSize))
                throw Exception("Error setting font face pixel size");
        }

    public:
        FreeTypeFontFace(Stream* fontFile)
        {
            _buf = Buffer::Create(fontFile->GetLength());
            fontFile->ReadSafe(_buf->Ptr(), 1, _buf->Size());

            switch (FT_New_Memory_Face(FreeTypeLibrary, (FT_Byte*)_buf->Ptr(), _buf->Size(), 0, &_face))
            {
            case 0:
                break;

            case FT_Err_Unknown_File_Format:
                throw Exception("Error loading font: Unknown file format");
                break;

            default:
                throw Exception("Error loading font");
                break;
            }
        }

        virtual ~FreeTypeFontFace()
        {
            FT_Done_Face(_face);
        }

        virtual String GetFamilyName()
        {
            return _face->family_name;
        }

        virtual String GetStyleName()
        {
            return _face->style_name;
        }

        virtual float GetAscender(float pixelSize)
        {
            return FontUnitsToPixels(pixelSize, _face->ascender);
        }

        virtual float GetDescender(float pixelSize)
        {
            return -FontUnitsToPixels(pixelSize, _face->descender);
        }

        virtual float GetLineHeight(float pixelSize)
        {
            return FontUnitsToPixels(pixelSize, _face->height);
        }

        virtual bool ContainsGlyph(float pixelSize, int c)
        {
            return true;
        }

        virtual Bitmap* RenderGlyph(float pixelSize, int c, FontRenderMode mode, Vector2* outAdvance, Vector2* outBearing)
        {
            SetPixelSize(pixelSize);

            Bitmap* result = 0;
            FT_UInt glyphIndex = FT_Get_Char_Index(_face, c);

            int m = FT_LOAD_DEFAULT;

            switch (mode)
            {
            case FontRenderModeNone:
                break;

            case FontRenderModeNormal:
                m |= FT_LOAD_RENDER;
                break;

            case FontRenderModeMonochrome:
                m |= FT_LOAD_RENDER | FT_LOAD_MONOCHROME;
                break;
            }

            FT_Load_Glyph(_face, glyphIndex, m);

            if (mode != FontRenderModeNone)
            {
                int w = _face->glyph->bitmap.width;
                int h = _face->glyph->bitmap.rows;
                result = new Bitmap(w, h, FormatL_8_UInt_Normalize);

                if (mode == FontRenderModeMonochrome)
                {
                    uint8_t* src = _face->glyph->bitmap.buffer;
                    uint8_t* dst = result->GetPtr();
                    int p = _face->glyph->bitmap.pitch;

                    for (int y = 0; y < h; y++)
                        for (int x = 0; x < w; x++)
                            *dst++ = ((src[y * p + (x >> 3)] >> (7 - (x & 7))) & 1) * 255;
                }
                else
                {
                    memcpy(result->GetPtr(), _face->glyph->bitmap.buffer, w * h);
                }
            }

            if (outAdvance)
                *outAdvance = Vector2((float)_face->glyph->advance.x / 64.0f, (float)_face->glyph->advance.y / 64.0f);

            if (outBearing)
                *outBearing = Vector2((float)_face->glyph->bitmap_left, (float)-_face->glyph->bitmap_top);

            return result;
        }

        virtual bool TryGetKerning(float pixelSize, int left, int right, Vector2* outKerning)
        {
            SetPixelSize(pixelSize);

            FT_Vector kerning;
            if (!FT_Get_Kerning(_face, FT_Get_Char_Index(_face, left), FT_Get_Char_Index(_face, right), FT_KERNING_DEFAULT, &kerning))
            {
                if (outKerning)
                    *outKerning = Vector2((float)kerning.x / 64.0f, (float)kerning.y / 64.0f);

                return true;
            }

            return false;
        }
    };

    static void Done()
    {
        FT_Done_FreeType(FreeTypeLibrary);
    }

    void FontFace::Init()
    {
        if (FreeTypeRefCount)
            return;

        if (FT_Init_FreeType(&FreeTypeLibrary) != 0)
            throw Exception("Error initializing FreeType");

        atexit(Done);
        FreeTypeRefCount++;
    }

    FontFace* FontFace::Load(Stream* fontFile)
    {
        Init();
        return new FreeTypeFontFace(fontFile);
    }
}
