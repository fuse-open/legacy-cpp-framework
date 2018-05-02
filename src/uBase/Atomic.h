#pragma once
#include <uBase/Config.h>

#if MSVC
#include <intrin.h>
#endif

namespace uBase
{
    inline int AtomicIncrement(int* ptr)
    {
#if MSVC
        return _InterlockedIncrement((volatile long*)ptr);
#else
        return __sync_add_and_fetch(ptr, 1);
#endif
    }

    inline int AtomicDecrement(int* ptr)
    {
#if MSVC
        return _InterlockedDecrement((volatile long*)ptr);
#else
        return __sync_sub_and_fetch(ptr, 1);
#endif
    }
}