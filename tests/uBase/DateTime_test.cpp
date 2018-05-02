#include "../catch.hpp"
#include <uBase/DateTime.h>
#include <uBase/Console.h>

using namespace uBase;

DateTime CreateDateTime(int year, int month, int day, int hour=0, int minute=0, int second=0, int millisecond=0)
{
    DateTime dt;
    dt.Year = year;
    dt.Month = month;
    dt.Day = day;
    dt.Hour = hour;
    dt.Minute = minute;
    dt.Second = second;
    dt.Millisecond = millisecond;
    return dt;
}

void Check(const DateTime& expected, const DateTime& actual)
{
    CHECK(expected.Year == actual.Year);
    CHECK(expected.Month == actual.Month);
    CHECK(expected.Day == actual.Day);
    CHECK(expected.Hour == actual.Hour);
    CHECK(expected.Minute == actual.Minute);
    CHECK(expected.Second == actual.Second);
    CHECK(expected.Millisecond == actual.Millisecond);
}

TEST_CASE("ToTimestamp1")
{
    DateTime dt = CreateDateTime(1601, 1, 1);
    Check(dt, DateTime(dt.ToTimestamp()));
}

TEST_CASE("ToTimestamp2")
{
    DateTime dt = CreateDateTime(1601, 1, 1, 23, 59, 59);
    Check(dt, DateTime(dt.ToTimestamp()));
}

TEST_CASE("ToTimestamp3")
{
    DateTime dt = CreateDateTime(1601, 1, 31, 23, 59, 59);
    Check(dt, DateTime(dt.ToTimestamp()));
}

TEST_CASE("ToTimestamp4")
{
    DateTime dt = CreateDateTime(1601, 2, 1);
    Check(dt, DateTime(dt.ToTimestamp()));
}

TEST_CASE("ToTimestamp5")
{
    DateTime dt = CreateDateTime(1601, 2, 28, 23, 59, 59);
    Check(dt, DateTime(dt.ToTimestamp()));
}

TEST_CASE("ToTimestamp6")
{
    DateTime dt = CreateDateTime(1601, 12, 31, 23, 59, 59);
    Check(dt, DateTime(dt.ToTimestamp()));
}

TEST_CASE("ToTimestamp7")
{
    DateTime dt = CreateDateTime(1602, 1, 1);
    Check(dt, DateTime(dt.ToTimestamp()));
}

TEST_CASE("ToTimestamp8")
{
    DateTime dt = CreateDateTime(2014, 1, 1);
    Check(dt, DateTime(dt.ToTimestamp()));
}

TEST_CASE("ToTimestamp9")
{
    DateTime dt = CreateDateTime(2014, 4, 30);
    Check(dt, DateTime(dt.ToTimestamp()));
}

TEST_CASE("ToTimestamp10")
{
    DateTime dt = CreateDateTime(1604, 1, 1);
    Check(dt, DateTime(dt.ToTimestamp()));
}
