#pragma once
#include <uBase/Vector2.h>
#include <uBase/Vector3.h>
#include <uBase/Vector4.h>

namespace uBase
{
    /** \ingroup uUtils
        A simple 32-bit pseudo-random number generator */
    class Random
    {
        uint32_t z, w;

    public:
        Random(uint32_t seed = 1);

        void SetSeed(uint32_t seed);

        /** Returns a uint32_t in range [0..UInt32Max] */
        uint32_t NextUInt32();

        /** Returns an int in range [0..high-1] */
        int NextInt(int high);

        /** Returns an int in range [low..high-1] */
        int NextInt(int low, int high);

        /** Returns a float in range [0..1] */
        float NextFloat();

        /** Returns a float in range [min..max] */
        float NextFloat(float min, float max);

        /** Returns a Vector2 in range [min..max] */
        Vector2 NextVector2(float min, float max);

        /** Returns a random Vector3 in range [min..max] */
        Vector3 NextVector3(float min, float max);

        /** Returns a random Vector4 in range [min..max] */
        Vector4 NextVector4(float min, float max);
    };

    /** \addtogroup uUtils */
    /** @{ */
    /** Global Random object */
    extern Random Rand;
    /** @} */
}
