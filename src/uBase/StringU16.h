#pragma once
#include <uBase/Config.h>
#include <stdint.h>

namespace uBase
{
    /** \addtogroup uText */
    /** @{ */
#ifdef WIN32
    typedef wchar_t Utf16;
#else
    typedef uint16_t Utf16;
#endif

    int Utf16StrLen(const Utf16* str);
    /** @} */

    /** \ingroup uText */
    class U_DLLEXPORT StringU16
    {
        friend class Unicode;
        Utf16* _data;
        int _length;

    public:
        StringU16();
        StringU16(const StringU16& copy);
        StringU16(const Utf16* str, int len);
        StringU16(const Utf16* str);
        ~StringU16();

        static StringU16 Create(int len);

        int Length() const;

        Utf16& operator[] (int index);
        const Utf16& operator[] (int index) const;

        Utf16* Ptr();
        const Utf16* Ptr() const;

        StringU16& operator =(const StringU16& copy);
        StringU16 operator +(const StringU16& str) const;
    };
}
