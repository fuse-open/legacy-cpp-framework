#pragma once
#include <stdint.h>
#include <uBase/String.h>

namespace uBase
{
    /** \ingroup uTime */
    class DateTime
    {
    public:
        static const uint64_t PerMillisecond = 10000;
        static const uint64_t PerSecond = PerMillisecond * 1000;
        static const uint64_t PerMinute = PerSecond * 60;
        static const uint64_t PerHour = PerMinute * 60;
        static const uint64_t PerDay = PerHour * 24;

        static bool IsLeapYear(int year);
        static int GetDaysInYear(int year);
        static int GetDaysInMonth(int month, int year);

        int Year;
        int Month;
        int Day;
        int Hour;
        int Minute;
        int Second;
        int Millisecond;

        DateTime();
        DateTime(uint64_t timestamp);

        static DateTime Now();
        static DateTime NowUtc();
        static DateTime FromString(const String& str);

        uint64_t ToTimestamp() const;
        String ToString() const;

        DateTime operator -(const DateTime& dt) const;
        DateTime operator +(const DateTime& dt) const;
        DateTime& operator =(const DateTime& dt);
    };

}
