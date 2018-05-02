#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <uBase/Time.h>
#include <chrono>

namespace uBase
{
    double GetSeconds()
    {
        LARGE_INTEGER frequency;
        QueryPerformanceFrequency(&frequency);

        LARGE_INTEGER currentTime;
        QueryPerformanceCounter(&currentTime);

        return (double)currentTime.QuadPart / (double)frequency.QuadPart;
    }

    long long GetTicks()
    {
        std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();
        return start.time_since_epoch().count();
    }

    uint64_t GetTimestamp()
    {
        FILETIME utcft, ft;
        GetSystemTimeAsFileTime(&utcft);
        FileTimeToLocalFileTime(&utcft, &ft);
        return ((uint64_t)ft.dwHighDateTime << 32) | ft.dwLowDateTime;
    }

    uint64_t GetTimestampUtc()
    {
        FILETIME ft;
        GetSystemTimeAsFileTime(&ft);
        return ((uint64_t)ft.dwHighDateTime << 32) | ft.dwLowDateTime;
    }
}