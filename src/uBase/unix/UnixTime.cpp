#include <uBase/DateTime.h>
#include <uBase/Time.h>
#include <sys/time.h>
#include <time.h>
#include <stdlib.h>

#if OSX || IOS
#include <mach/mach.h>
#include <mach/mach_time.h>
#endif

namespace uBase
{
    static uint64_t ConvertToTimestamp(const tm* const time, const suseconds_t useconds = 0)
    {
        DateTime dt;
        dt.Year = time->tm_year + 1900;
        dt.Month = time->tm_mon + 1;
        dt.Day = time->tm_mday;
        dt.Hour = time->tm_hour;
        dt.Minute = time->tm_min;
        dt.Second = time->tm_sec;
        dt.Millisecond = useconds / 1000;
        return dt.ToTimestamp();
    }

    double GetSeconds()
    {
#if ANDROID || LINUX
        // this method is more accurate for android
        struct timespec now;
        clock_gettime(CLOCK_MONOTONIC, &now);
        return ((double)now.tv_sec) + (((double)now.tv_nsec) / 1000000000.0);
#elif OSX || IOS
        //https://developer.apple.com/library/mac/qa/qa1398/_index.html
        static double multiplier = 0;
        if (multiplier == 0)
        {
            mach_timebase_info_data_t timebase;
            mach_timebase_info(&timebase);
            multiplier = (double)timebase.numer / (double)timebase.denom / 1e9;
        }

        return (double)mach_absolute_time() * multiplier;
#else
        struct timeval t;
        gettimeofday(&t, 0);
        return (double)t.tv_sec + (double)t.tv_usec * (1.0 / 1000000.0);
#endif
    }

    long long GetTicks()
    {
#if ANDROID || LINUX
        // this method is more accurate for android
        struct timespec now;
        clock_gettime(CLOCK_REALTIME, &now);
        return (long long)now.tv_sec * 10000000 + (long long)now.tv_nsec / 100;
#else
        // Needed as osx and ios dont implement clock_gettime
        struct timeval t;
        gettimeofday(&t, 0);
        return (long long)t.tv_sec * 10000000 + (long long)t.tv_usec * 10;
#endif
    }

    uint64_t GetTimestamp()
    {
        timeval t;
        gettimeofday(&t, NULL);
        return ConvertToTimestamp(localtime(&t.tv_sec), t.tv_usec);
    }

    uint64_t GetTimestampUtc()
    {
        timeval t;
        gettimeofday(&t, NULL);
        return ConvertToTimestamp(gmtime(&t.tv_sec), t.tv_usec);
    }
}
