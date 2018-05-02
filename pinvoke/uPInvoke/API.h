#pragma once
#include <uBase/Unicode.h>
#include <uBase/Console.h>
#include <uImage/Bitmap.h>
#include <uImage/FontFace.h>
#include <uImage/Texture.h>

#ifdef WIN32
    #define EXPORT extern "C" __declspec(dllexport)
    #define STDCALL __stdcall
    typedef wchar_t uChar;
#else
    #define EXPORT extern "C"
    #define STDCALL
    typedef unsigned short uChar;
#endif

struct U_DLLEXPORT uNativeObject: uBase::Object
{
    uBase::StringU16 Error;
    uBase::Array<uBase::Auto<uBase::Object> > Objects;
    uBase::Auto<uImage::Texture> TexturePtr;
    uBase::Auto<uImage::FontFace> FontFacePtr;
    uBase::Auto<uImage::Bitmap> BitmapPtr;

    uNativeObject();
    virtual ~uNativeObject();
    const uChar* GetString(const uBase::String& str);
    void SetError(const uBase::String& str);
};

#define BEGIN_FUNC \
    try \
    {

#define END_FUNC \
        return 0; \
    } \
    catch (const uBase::Exception& e) \
    { \
        obj->SetError(e.GetMessage()); \
        return -1; \
    }
