#pragma once
#include <uBase/Config.h>
#include <uBase/Exception.h>
#include <cstddef>

namespace uBase
{
    /** \ingroup uObject */
    template <typename T> class U_DLLEXPORT Auto
    {
    protected:
        T* _object;
        void Retain()
        {
            if (_object)
                _object->Retain();
        }
        void Release()
        {
            if (_object)
                _object->Release();
        }

    public:
        Auto() : _object(NULL) {}
        Auto(const Auto& ref) : _object(ref._object) { Retain(); }
        Auto(T* object) : _object(object) {}
        ~Auto() { Release(); }

        operator T*() { return _object; }
        operator const T*() const { return _object; }
        T* operator ->() { return Get(); }
        const T* operator ->() const { return Get(); }

        Auto& operator =(T* ptr)
        {
            if (ptr == _object)
                return *this;
            Release();
            _object = ptr;
            return *this;
        }
        Auto& operator =(const Auto& ref)
        {
            if (ref._object == _object)
                return *this;
            Release();
            _object = ref._object;
            Retain();
            return *this;
        }
        T* Get()
        {
#ifdef DEBUG
            if (!_object)
                throw NullPointerException();
#endif
            return _object;
        }
        const T* Get() const
        {
#ifdef DEBUG
            if (!_object)
                throw NullPointerException();
#endif
            return _object;
        }
    };

    /** \ingroup uObject */
    template <typename T> class U_DLLEXPORT Shared : public Auto<T>
    {
        using Auto<T>::_object;
        using Auto<T>::Retain;
        using Auto<T>::Release;
    public:
        using Auto<T>::operator ->;
        operator T*() { return _object; }
        operator const T*() const { return _object; }

        Shared() {}
        Shared(T* ptr) : Auto<T>(ptr) { Retain(); }
        Shared(const Shared& ref) : Auto<T>(ref._object) { Retain(); }
        Shared(const Auto<T>& ref) : Auto<T>(ref) {}

        Shared& operator =(T* ptr)
        {
            if (ptr == _object)
                return *this;
            Release();
            _object = ptr;
            Retain();
            return *this;
        }
        Shared& operator =(const Shared& ref)
        {
            return operator =(ref._object);
        }
    };

    /** \ingroup uObject */
    template <typename T>
    Auto<T> AutoPtr(T* ptr)
    {
        return ptr;
    }

    /** \ingroup uObject */
    template <typename T>
    T* SharePtr(T* ptr)
    {
        if (ptr)
            ptr->Retain();
        
        return ptr;
    }
}
