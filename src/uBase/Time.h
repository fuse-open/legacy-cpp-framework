#pragma once
#include <stdint.h>

namespace uBase
{
    /** \addtogroup uTime */
    /** @{ */
    double GetSeconds();
    long long GetTicks();
    uint64_t GetTimestamp();
    uint64_t GetTimestampUtc();
    /** @} */
}
