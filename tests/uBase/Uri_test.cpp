#include <uBase/Uri.h>

#include "../catch.hpp"

using uBase::Uri;
using uBase::String;

void EncodesTo(const char* raw, const char* expectedEncoded);
void AutoEncodesUriTo(const char* raw, const char* expectedEncoded);

TEST_CASE("DoesNotEncodeSimpleUri")
{
    EncodesTo("http://outracks.com/foo?bar=baz", "http://outracks.com/foo?bar=baz");
}

TEST_CASE("EncodesOtherCharactersInUri")
{
    EncodesTo("http://outracks.com/æ?å=™ €", "http://outracks.com/%C3%A6?%C3%A5=%E2%84%A2%20%E2%82%AC");
}

TEST_CASE("DoesNotEncodeAnyReservedOrUnreservedCharacters")
{
    //http://en.wikipedia.org/wiki/Percent-encoding#Types_of_URI_characters
    const char* chars = "!*'();:@&=+$,/?#[]ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_.~";
    EncodesTo(chars, chars);
}

TEST_CASE("EncodesParenthesis")
{
    EncodesTo("%", "%25");
}

TEST_CASE("DoesNotEncodeParenthesis")
{
    AutoEncodesUriTo("%", "%");
}

void EncodesTo(const char* raw, const char* expectedEncoded)
{
    //Convert to std::string before REQUIRE, to make default comparison and cout work better
    REQUIRE(std::string(String(expectedEncoded).Ptr()) == std::string(Uri::Encode(String(raw)).Ptr()));
}

void AutoEncodesUriTo(const char* raw, const char* expectedEncoded)
{
    //Convert to std::string before REQUIRE, to make default comparison and cout work better
    REQUIRE(std::string(String(expectedEncoded).Ptr()) == std::string(Uri::AutoEncodeUri(String(raw)).Ptr()));
}
