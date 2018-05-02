#include <uBase/Traits.h>
#include <cstdio>

#if !MSVC
# define sprintf_s snprintf
#endif

namespace uBase
{
    uint32_t Default::Hash(const uint8_t* data, int size)
    {
        // djb2
        uint32_t hash = 5381;

        for (int i = 0; i < size; i++)
            hash = ((hash << 5) + hash) ^ (uint32_t)data[i];

        return hash;
    }

    uint32_t Default::Hash(bool b)
    {
        return b ? 1 : 0;
    }

    uint32_t Default::Hash(const void* ptr)
    {
        switch (sizeof(const void*))
        {
        case 4:
            return *(uint32_t*)&ptr;
        case 8:
            return ((uint32_t*)&ptr)[0] ^ ((uint32_t*)&ptr)[1];
        default:
            return Hash((const uint8_t*)&ptr, (int)sizeof(const void*));
        }
    }

    uint32_t Default::Hash(float f)
    {
        return *(uint32_t*)&f;
    }

    uint32_t Default::Hash(double d)
    {
        uint32_t hash = 27;
        hash = (13 * hash) + ((uint32_t*)&d)[0];
        hash = (13 * hash) + ((uint32_t*)&d)[1];
        return hash;
    }

    uint32_t Default::Hash(int8_t i)
    {
        return (uint32_t)i;
    }

    uint32_t Default::Hash(uint8_t i)
    {
        return (uint32_t)i;
    }

    uint32_t Default::Hash(int16_t i)
    {
        return (uint32_t)i;
    }

    uint32_t Default::Hash(uint16_t i)
    {
        return (uint32_t)i;
    }

    uint32_t Default::Hash(int32_t i)
    {
        return (uint32_t)i;
    }

    uint32_t Default::Hash(uint32_t i)
    {
        return i;
    }

    uint32_t Default::Hash(int64_t i)
    {
        uint32_t hash = 27;
        hash = (13 * hash) + ((uint32_t*)&i)[0];
        hash = (13 * hash) + ((uint32_t*)&i)[1];
        return hash;
    }

    uint32_t Default::Hash(uint64_t i)
    {
        uint32_t hash = 27;
        hash = (13 * hash) + ((uint32_t*)&i)[0];
        hash = (13 * hash) + ((uint32_t*)&i)[1];
        return hash;
    }

    uint32_t Default::Hash(const String& str)
    {
        return Hash((const uint8_t*)str.Ptr(), str.Length());
    }

    String Default::ToString(void* p)
    {
        char buf[64];
        int len = sprintf_s(buf, 64, "0x%llx", (unsigned long long int)p);
        return String(buf, len);
    }

    String Default::ToString(char c)
    {
        return (int)c;
    }

    String Default::ToString(unsigned char c)
    {
        return (int)c;
    }

    String Default::ToString(short i)
    {
        char buf[64];
        int len = sprintf_s(buf, 64, "%hd", i);
        return String(buf, len);
    }

    String Default::ToString(unsigned short i)
    {
        char buf[64];
        int len = sprintf_s(buf, 64, "%hu", i);
        return String(buf, len);
    }

    String Default::ToString(int i)
    {
        return i;
    }

    String Default::ToString(unsigned int i)
    {
        char buf[64];
        int len = sprintf_s(buf, 64, "%u", i);
        return String(buf, len);
    }

    String Default::ToString(long i)
    {
        char buf[64];
        int len = sprintf_s(buf, 64, "%ld", i);
        return String(buf, len);
    }

    String Default::ToString(unsigned long i)
    {
        char buf[64];
        int len = sprintf_s(buf, 64, "%lu", i);
        return String(buf, len);
    }

    String Default::ToString(long long i)
    {
        char buf[64];
        int len = sprintf_s(buf, 64, "%lld", i);
        return String(buf, len);
    }

    String Default::ToString(unsigned long long i)
    {
        char buf[64];
        int len = sprintf_s(buf, 64, "%llu", i);
        return String(buf, len);
    }

    String Default::ToString(float f)
    {
        return f;
    }

    String Default::ToString(double d)
    {
        return d;
    }

    String Default::ToString(bool b)
    {
        return String::FromBool(b);
    }
}
