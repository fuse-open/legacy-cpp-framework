#include <uBase/BaseLib.h>
#include "API.h"

using namespace uBase;
using namespace uImage;

EXPORT void STDCALL uInit()
{
    BaseLib::Init();
    FontFace::Init();
}

EXPORT uNativeObject* STDCALL uCreate()
{
    return new uNativeObject();
}

EXPORT void STDCALL uRelease(uNativeObject* obj)
{
    obj->Release();
}

EXPORT const uChar* STDCALL uGetErrorString(uNativeObject* obj)
{
    return obj->Error.Ptr();
}

uNativeObject::uNativeObject()
{
}

uNativeObject::~uNativeObject()
{
}

struct U_DLLEXPORT uStringData: Object
{
    StringU16 String;
};

const uChar* uNativeObject::GetString(const String& str)
{
    uStringData* sd = new uStringData();
    sd->String = Unicode::Utf8To16(str);
    Objects.Add(sd);
    return sd->String.Ptr();
}

void uNativeObject::SetError(const String& str)
{
    Error = Unicode::Utf8To16(str);
}
