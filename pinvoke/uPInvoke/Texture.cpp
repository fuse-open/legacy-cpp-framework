#include <uImage/Png.h>
#include <uImage/Jpeg.h>
#include <uImage/Texture.h>
#include <uBase/BufferStream.h>
#include <uBase/Path.h>
#include <cstring>
#include "API.h"

using namespace uBase;
using namespace uImage;

struct uTextureInfo
{
    int Width;
    int Height;
    int Depth;
    int FaceCount;
    int MipCount;
    int FileFormat;
    int TextureType;
    int PixelFormat;
};

static void uTexture_SetInfoInternal(uNativeObject* obj, uTextureInfo* result)
{
    result->Width = obj->TexturePtr->GetWidth();
    result->Height = obj->TexturePtr->GetHeight();
    result->Depth = obj->TexturePtr->GetDepth();
    result->FaceCount = obj->TexturePtr->Faces.Length();
    result->MipCount = obj->TexturePtr->GetMipCount();
    result->TextureType = obj->TexturePtr->Type;
    result->PixelFormat = obj->TexturePtr->GetFormat();
}

static void uTexture_LoadInternal(uNativeObject* obj, const uChar* filename, Stream* stream)
{
    String utf8 = Unicode::Utf16To8(filename);
    String ext = Path::GetExtension(utf8).ToLower();

    if (ext == ".jpeg" || ext == ".jpg")
    {
        Auto<Image> image = stream != NULL ? Jpeg::Load(stream) : Jpeg::Load(utf8);
        obj->TexturePtr = Texture::Create(image);
    }
    else if (ext == ".png")
    {
        Auto<Image> image = stream != NULL ? Png::Load(stream) : Png::Load(utf8);
        obj->TexturePtr = Texture::Create(image);
    }
    else
    {
        throw Exception("Unsupported texture file: " + utf8);
    }
}

EXPORT int STDCALL uLoadTexture(uNativeObject* obj, const uChar* filename, uTextureInfo* result)
{
    BEGIN_FUNC;
    uTexture_LoadInternal(obj, filename, NULL);
    uTexture_SetInfoInternal(obj, result);
    END_FUNC;
}

EXPORT int STDCALL uLoadTextureFromMemory(uNativeObject* obj, const uChar* filename, void* data, int size, uTextureInfo* result)
{
    BEGIN_FUNC;
    BufferStream stream(new BufferPtr(data, size, false), true, false);
    uTexture_LoadInternal(obj, filename, &stream);
    uTexture_SetInfoInternal(obj, result);
    END_FUNC;
}

EXPORT int STDCALL uTexture_Convert2DToCube(uNativeObject* obj, uTextureInfo* result)
{
    BEGIN_FUNC;
    obj->TexturePtr = obj->TexturePtr->Convert2DToCube();
    uTexture_SetInfoInternal(obj, result);
    END_FUNC;
}

EXPORT int STDCALL uTexture_ConvertCubeTo2D(uNativeObject* obj, int cube2DMode, uTextureInfo* result)
{
    BEGIN_FUNC;
    obj->TexturePtr = obj->TexturePtr->ConvertCubeTo2D((Cube2DMode)cube2DMode);
    uTexture_SetInfoInternal(obj, result);
    END_FUNC;
}

static Image* uTexture_GetImageInternal(uNativeObject* obj, int faceIndex, int mipIndex)
{
    return obj->TexturePtr->Faces[faceIndex].MipLevels[mipIndex];
}

EXPORT int STDCALL uTexture_GetDataSize(uNativeObject* obj, int faceIndex, int mipIndex, int* result)
{
    BEGIN_FUNC;
    *result = uTexture_GetImageInternal(obj, faceIndex, mipIndex)->GetSizeInBytes();
    END_FUNC;
}

EXPORT int STDCALL uTexture_ReadData(uNativeObject* obj, int faceIndex, int mipIndex, void* result, int byteCount)
{
    BEGIN_FUNC;
    Image* image = uTexture_GetImageInternal(obj, faceIndex, mipIndex);
    if (image->GetSizeInBytes() != byteCount)
        throw IndexOutOfRangeException();
    memcpy(result, image->GetPtr(), image->GetSizeInBytes());
    END_FUNC;
}

EXPORT int STDCALL uTexture_SaveAsPng(uNativeObject* obj, const uChar* dstFilename, int faceIndex, int mipIndex)
{
    BEGIN_FUNC;
    Image* image = uTexture_GetImageInternal(obj, faceIndex, mipIndex);
    Auto<Bitmap> bmp = image->ToBitmap();
    Png::Save(Unicode::Utf16To8(dstFilename), bmp);
    END_FUNC;
}
