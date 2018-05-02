#pragma once
#include <uBase/Config.h>

namespace uBase
{
    class String;

    /** \ingroup uObject */
    class U_DLLEXPORT Object
    {
        int _refCount;

        Object(const Object& copy);
        Object& operator =(const Object& copy);

    public:
        Object();
        virtual ~Object();

        int GetRefCount() const;
        void Retain();
        void Release();

        virtual void Delete();
        virtual String ToString() const;
    };
}
