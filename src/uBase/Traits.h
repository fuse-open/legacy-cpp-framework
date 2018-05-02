#pragma once
#include <stdint.h>
#include <uBase/String.h>

namespace uBase
{
    /** \ingroup uTraits */
    struct Default
    {
        template <typename T>
        static bool Equals(const T& a, const T& b)
        {
            return a == b;
        }

        static uint32_t Hash(const uint8_t* data, int size);

        static uint32_t Hash(bool b);
        static uint32_t Hash(const void* p);
        static uint32_t Hash(float f);
        static uint32_t Hash(double d);

        static uint32_t Hash(int8_t i);
        static uint32_t Hash(uint8_t i);
        static uint32_t Hash(int16_t i);
        static uint32_t Hash(uint16_t i);
        static uint32_t Hash(int32_t i);
        static uint32_t Hash(uint32_t i);
        static uint32_t Hash(int64_t i);
        static uint32_t Hash(uint64_t i);
        static uint32_t Hash(const String& str);

        template <typename T>
        static uint32_t Hash(const T* t)
        {
            return Hash((const void*)t);
        }

        template <typename T>
        static uint32_t Hash(T* t)
        {
            return Hash((const void*)t);
        }

        template <typename T>
        static uint32_t Hash(const T& t)
        {
            return Hash((const uint8_t*)&t, (int)sizeof(T));
        }

        static String ToString(void* p);
        static String ToString(int i);
        static String ToString(unsigned int i);
        static String ToString(short i);
        static String ToString(unsigned short i);
        static String ToString(long i);
        static String ToString(unsigned long i);
        static String ToString(long long i);
        static String ToString(unsigned long long i);
        static String ToString(float f);
        static String ToString(double d);
        static String ToString(char c);
        static String ToString(unsigned char c);
        static String ToString(bool b);

        template <typename T>
        static String ToString(const T& t)
        {
            return t.ToString();
        }
    };

    /** \ingroup uTraits */
    template <typename T> struct Traits
    {
        static bool Equals(const T& a, const T& b)
        {
            return Default::Equals(a, b);
        }

        static uint32_t Hash(const T& t)
        {
            return Default::Hash(t);
        }

        static String ToString(const T& t)
        {
            return Default::ToString(t);
        }
    };
}
