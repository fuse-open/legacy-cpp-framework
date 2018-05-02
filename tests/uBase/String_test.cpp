#include "../catch.hpp"
#include <uBase/String.h>
#include <uBase/Exception.h>
#include <climits>

using namespace uBase;

TEST_CASE("ToInt")
{
    String str = "20";
    REQUIRE(20 == str.ToInt());

    str = "-20";
    REQUIRE(-20 == str.ToInt());

    str = "0";
    REQUIRE(0 == str.ToInt());

    str = "-20 ";
    REQUIRE(-20 == str.ToInt());

    str = " 200";
    REQUIRE(200 == str.ToInt());

    str = " 211  ";
    REQUIRE(211 == str.ToInt());

    str = "+70";
    REQUIRE(70 == str.ToInt());

    str = String(INT_MAX);
    REQUIRE(INT_MAX == str.ToInt());

    str = String(INT_MIN);
    REQUIRE(INT_MIN == str.ToInt());

    str = "4294967295";
    CHECK_THROWS_AS(str.ToInt(), OverflowException);

    str = "-4294967295";
    CHECK_THROWS_AS(str.ToInt(), OverflowException);

    str = "";
    CHECK_THROWS_AS(str.ToInt(), FormatException);

    str = "just_string";
    CHECK_THROWS_AS(str.ToInt(), FormatException);

    str = "50foo";
    CHECK_THROWS_AS(str.ToInt(), FormatException);
}

TEST_CASE("ToLong")
{
    String str = "2000";
    REQUIRE(2000 == str.ToLong());

    str = "-2078";
    REQUIRE(-2078 == str.ToLong());

    str = "0";
    REQUIRE(0 == str.ToLong());

    str = "-2087 ";
    REQUIRE(-2087 == str.ToLong());

    str = " 15200";
    REQUIRE(15200 == str.ToLong());

    str = " 21199  ";
    REQUIRE(21199 == str.ToLong());

    str = "+78";
    REQUIRE(78 == str.ToLong());

    str = "9223372036854775807";
    REQUIRE(LLONG_MAX == str.ToLong());

    str = "-9223372036854775808";
    REQUIRE(LLONG_MIN == str.ToLong());

    str = "";
    CHECK_THROWS_AS(str.ToLong(), FormatException);

    str = "just_string";
    CHECK_THROWS_AS(str.ToLong(), FormatException);

    str = "50foo";
    CHECK_THROWS_AS(str.ToLong(), FormatException);
}

TEST_CASE("ToULong")
{
    String str = "2000";
    REQUIRE(2000 == str.ToULong());

    str = "0";
    REQUIRE(0 == str.ToULong());

    str = "2087 ";
    REQUIRE(2087 == str.ToULong());

    str = " 15200000";
    REQUIRE(15200000 == str.ToULong());

    str = " 21199000  ";
    REQUIRE(21199000 == str.ToULong());

    str = "+78";
    REQUIRE(78 == str.ToULong());

    str = "-2078";
    CHECK_THROWS_AS(str.ToULong(), OverflowException);

    str = "";
    CHECK_THROWS_AS(str.ToULong(), FormatException);

    str = "just_string";
    CHECK_THROWS_AS(str.ToULong(), FormatException);

    str = "50foo";
    CHECK_THROWS_AS(str.ToULong(), FormatException);
}

TEST_CASE("ToFloat")
{
    String str = "20.2";
    REQUIRE(20.2f == str.ToFloat());

    str = "-20.6";
    REQUIRE(-20.6f == str.ToFloat());

    str = "0";
    REQUIRE(0 == str.ToFloat());

    str = "-18.6 ";
    REQUIRE(-18.6f == str.ToFloat());

    str = " 200.2";
    REQUIRE(200.2f == str.ToFloat());

    str = " 211.5  ";
    REQUIRE(211.5f == str.ToFloat());

    str = "+70";
    REQUIRE(70.0f == str.ToFloat());

    float minFloatValue = -3.402823e38;
    float maxFloatValue = 3.402823e38;

    str = String(minFloatValue);
    REQUIRE(minFloatValue == str.ToFloat());

    str = String(maxFloatValue);
    REQUIRE(maxFloatValue == str.ToFloat());

    str = String(minFloatValue * 2.0);
    CHECK_THROWS_AS(str.ToFloat(), OverflowException);

    str = String(maxFloatValue * 2.0);
    CHECK_THROWS_AS(str.ToFloat(), OverflowException);

    str = "";
    CHECK_THROWS_AS(str.ToFloat(), FormatException);

    str = "just_string";
    CHECK_THROWS_AS(str.ToFloat(), FormatException);

    str = "50.0foo";
    CHECK_THROWS_AS(str.ToFloat(), FormatException);
}

TEST_CASE("ToDouble")
{
    String str = "20.2";
    REQUIRE(20.2 == str.ToDouble());

    str = "-20.6";
    REQUIRE(-20.6 == str.ToDouble());

    str = "0";
    REQUIRE(0 == str.ToDouble());

    str = "-1801.6 ";
    REQUIRE(-1801.6 == str.ToDouble());

    str = " 205.2";
    REQUIRE(205.2 == str.ToDouble());

    str = " 211.545  ";
    REQUIRE(211.545 == str.ToDouble());

    str = "+7000.568";
    REQUIRE(7000.568 == str.ToDouble());

    str = "";
    CHECK_THROWS_AS(str.ToDouble(), FormatException);

    str = "just_string";
    CHECK_THROWS_AS(str.ToDouble(), FormatException);

    str = "50.0foo";
    CHECK_THROWS_AS(str.ToDouble(), FormatException);
}

TEST_CASE("TryToFloat")
{
    float res;
    String str = " +20.2 ";
    REQUIRE(true == str.TryToFloat(&res));
    REQUIRE(20.2f == res);

    str = "just_string";
    REQUIRE(false == str.TryToFloat(&res));
    REQUIRE(0 == res);

    str = "50.0foo";
    REQUIRE(false == str.TryToFloat(&res));
    REQUIRE(0 == res);
}

TEST_CASE("TryToDouble")
{
    double res;
    String str = " +20.2 ";
    REQUIRE(true == str.TryToDouble(&res));
    REQUIRE(20.2 == res);

    str = "just_string";
    REQUIRE(false == str.TryToDouble(&res));
    REQUIRE(0 == res);

    str = "50.0foo";
    REQUIRE(false == str.TryToDouble(&res));
    REQUIRE(0 == res);
}

TEST_CASE("TryToInt")
{
    int res;
    String str = " +20 ";
    REQUIRE(true == str.TryToInt(&res));
    REQUIRE(20 == res);

    str = "4294967295";
    REQUIRE(false == str.TryToInt(&res));
    REQUIRE(0 == res);

    str = "-4294967295";
    REQUIRE(false == str.TryToInt(&res));
    REQUIRE(0 == res);

    str = " +20.2 ";
    REQUIRE(false == str.TryToInt(&res));
    REQUIRE(0 == res);

    str = "just_string";
    REQUIRE(false == str.TryToInt(&res));
    REQUIRE(0 == res);
}

TEST_CASE("TryToLong")
{
    long long res;
    String str = " +20 ";
    REQUIRE(true == str.TryToLong(&res));
    REQUIRE(20 == res);

    str = " +20.2 ";
    REQUIRE(false == str.TryToLong(&res));
    REQUIRE(0 == res);

    str = "92233720368547758075";
    REQUIRE(false == str.TryToLong(&res));
    REQUIRE(0 == res);

    str = "-92233720368547758085";
    REQUIRE(false == str.TryToLong(&res));
    REQUIRE(0 == res);

    str = "just_string";
    REQUIRE(false == str.TryToLong(&res));
    REQUIRE(0 == res);
}

TEST_CASE("TryToULong")
{
    unsigned long long res;
    String str = " +20 ";
    REQUIRE(true == str.TryToULong(&res));
    REQUIRE(20 == res);

    str = " +20.2 ";
    REQUIRE(false == str.TryToULong(&res));
    REQUIRE(0 == res);

    str = "just_string";
    REQUIRE(false == str.TryToULong(&res));
    REQUIRE(0 == res);

    str = "-2078";
    REQUIRE(false == str.TryToULong(&res));
    REQUIRE(0 == res);
}
