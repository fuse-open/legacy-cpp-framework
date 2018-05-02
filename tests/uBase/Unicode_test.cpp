#include "../catch.hpp"
#include <uBase/Unicode.h>
#include <uBase/Exception.h>

using namespace uBase;

TEST_CASE("ToUpper")
{
    REQUIRE(0 == Unicode::ToUpper(0));
    REQUIRE('.' == Unicode::ToUpper('.'));
    REQUIRE('A' == Unicode::ToUpper('a'));
    REQUIRE('A' == Unicode::ToUpper('A'));
    REQUIRE(0xc5 == Unicode::ToUpper(0xe5)); //æ
}

TEST_CASE("ToLower")
{
    REQUIRE(0 == Unicode::ToLower(0));
    REQUIRE('.' == Unicode::ToLower('.'));
    REQUIRE('a' == Unicode::ToLower('a'));
    REQUIRE('a' == Unicode::ToLower('A'));
    REQUIRE(0xe5 == Unicode::ToLower(0xc5)); //æ
}

TEST_CASE("ModifiedUtf8")
{
    StringU16 utf16 = Unicode::Utf8To16(String("\0\0foo\0\0bar\0\0", 12));
    String mutf8 = Unicode::Utf16To8(utf16, UnicodeFlagsModifiedUtf8);
    REQUIRE(mutf8.Length() == 18);
    REQUIRE(mutf8[0] == (char)(unsigned char)0xC0);
    REQUIRE(mutf8[1] == (char)(unsigned char)0x80);

    StringU16 utf16_3 = Unicode::Utf8To16(mutf8, UnicodeFlagsModifiedUtf8);
    REQUIRE(Unicode::Utf16To8(utf16_3, 0).Length() == 12);
    REQUIRE(Unicode::Utf16To8(utf16_3, UnicodeFlagsModifiedUtf8).Length() == 18);

    StringU16 utf16_2 = Unicode::Utf8To16(mutf8, UnicodeFlagsIgnoreError);
    REQUIRE(Unicode::Utf16To8(utf16_2).Length() == 0);

    const char *input = "foo\300\200bar";
    StringU16 utf16_4 = Unicode::Utf8To16(input, strlen(input), UnicodeFlagsModifiedUtf8);
    REQUIRE(Unicode::Utf16To8(utf16_4).Length() == 7);
    REQUIRE(utf16_4[0] == 'f');
    REQUIRE(utf16_4[1] == 'o');
    REQUIRE(utf16_4[2] == 'o');
    REQUIRE(utf16_4[3] == '\0');
    REQUIRE(utf16_4[4] == 'b');
    REQUIRE(utf16_4[5] == 'a');
    REQUIRE(utf16_4[6] == 'r');

    const char *input2 = "foo\300";
    bool thrown = false;
    try
    {
        Unicode::Utf8To16(input2, strlen(input2), UnicodeFlagsModifiedUtf8);
    }
    catch (const Exception &e)
    {
        thrown = true;
    }
    REQUIRE(thrown == true);
}
