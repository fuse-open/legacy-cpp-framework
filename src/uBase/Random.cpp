#include <uBase/Config.h>
#include <uBase/Random.h>

namespace uBase
{
    Random::Random(uint32_t seed)
    {
        SetSeed(seed);
    }

    void Random::SetSeed(uint32_t seed)
    {
        if (!seed) seed = 1;
        z = seed;
        w = seed + 1;
    }

    uint32_t Random::NextUInt32()
    {
        z = 36969 * (z & UINT16_MAX) + (z >> 16);
        w = 18000 * (w & UINT16_MAX) + (w >> 16);
        return (z << 16) + w;
    }

    int Random::NextInt(int high)
    {
        return (int)(NextFloat() * (float)high);
    }

    int Random::NextInt(int low, int high)
    {
        return (int)(NextFloat() * (float)(high - low) + (float)low);
    }

    float Random::NextFloat()
    {
        return ((float)NextUInt32() / (float)UINT32_MAX);
    }

    float Random::NextFloat(float min, float max)
    {
        return NextFloat() * (max - min) + min;
    }

    Vector2 Random::NextVector2(float min, float max)
    {
        return Vector2(NextFloat(min, max), NextFloat(min, max));
    }

    Vector3 Random::NextVector3(float min, float max)
    {
        return Vector3(NextFloat(min, max), NextFloat(min, max), NextFloat(min, max));
    }

    Vector4 Random::NextVector4(float min, float max)
    {
        return Vector4(NextFloat(min, max), NextFloat(min, max), NextFloat(min, max), NextFloat(min, max));
    }


    Random Rand;
}
