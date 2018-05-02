#include <uBase/BufferStream.h>
#include <cstring>
#include "API.h"

using namespace uBase;
using namespace uImage;

struct uFontFaceInfo
{
    const uChar* FamilyName;
    const uChar* StyleName;
};

struct uFontLineMetricInfo
{
    float Ascender;
    float Descender;
    float LineHeight;
};

enum uFontRenderMode
{
    uFontRenderModeNone = 0,
    uFontRenderModeNormal = 1,
    uFontRenderModeMonochrome = 2,
    uFontRenderModeForceInt = 0x7fffffff,
};

struct uFontGlyphInfo
{
    Vector2 Advance;
    Vector2 Bearing;
    int BitmapWidth;
    int BitmapHeight;
    int PixelFormat;
};

EXPORT int STDCALL uLoadFontFace(uNativeObject* obj, const uChar* filename, uFontFaceInfo* result)
{
    BEGIN_FUNC;
    obj->FontFacePtr = FontFace::Load(Unicode::Utf16To8(filename));
    result->FamilyName = obj->GetString(obj->FontFacePtr->GetFamilyName());
    result->StyleName = obj->GetString(obj->FontFacePtr->GetStyleName());
    END_FUNC;
}

EXPORT int STDCALL uLoadFontFaceFromMemory(uNativeObject* obj, void* data, int size, uFontFaceInfo* result)
{
    BEGIN_FUNC;
    BufferStream stream(new BufferPtr(data, size, false), true, false);
    obj->FontFacePtr = FontFace::Load(&stream);
    result->FamilyName = obj->GetString(obj->FontFacePtr->GetFamilyName());
    result->StyleName = obj->GetString(obj->FontFacePtr->GetStyleName());
    END_FUNC;
}

EXPORT int STDCALL uFontFace_GetLineMetrics(uNativeObject* obj, float pixelSize, uFontLineMetricInfo* result)
{
    BEGIN_FUNC;
    result->Ascender = obj->FontFacePtr->GetAscender(pixelSize);
    result->Descender = obj->FontFacePtr->GetDescender(pixelSize);
    result->LineHeight = obj->FontFacePtr->GetLineHeight(pixelSize);
    END_FUNC;
}

EXPORT int STDCALL uFontFace_GetKerning(uNativeObject* obj, float pixelSize, unsigned int left, unsigned int right, Vector2* result)
{
    BEGIN_FUNC;
    obj->FontFacePtr->TryGetKerning(pixelSize, left, right, result);
    END_FUNC;
}

EXPORT int STDCALL uFontFace_ContainsGlyph(uNativeObject* obj, float pixelSize, unsigned int glyph, int* result)
{
    BEGIN_FUNC;
    *result = obj->FontFacePtr->ContainsGlyph(pixelSize, glyph) ? 1 : 0;
    END_FUNC;
}

EXPORT int STDCALL uFontFace_RenderGlyph(uNativeObject* obj, float pixelSize, unsigned int glyph, uFontRenderMode mode, uFontGlyphInfo* result)
{
    BEGIN_FUNC;
    obj->BitmapPtr = obj->FontFacePtr->RenderGlyph(pixelSize, glyph, (FontRenderMode)mode, &result->Advance, &result->Bearing);
    result->BitmapWidth = obj->BitmapPtr->GetWidth();
    result->BitmapHeight = obj->BitmapPtr->GetHeight();
    result->PixelFormat = obj->BitmapPtr->GetFormat();
    END_FUNC;
}

EXPORT int STDCALL uFontFace_ReadGlyphBitmap(uNativeObject* obj, void* result, int byteCount)
{
    BEGIN_FUNC;
    int size = obj->BitmapPtr->GetBytesPerPixel() * obj->BitmapPtr->GetWidth() * obj->BitmapPtr->GetHeight();

    if (byteCount != size)
    {
        obj->SetError("Invalid buffer size");
        return -1;
    }

    memcpy(result, obj->BitmapPtr->GetPtr(), size);
    END_FUNC;
}
