#define CATCH_CONFIG_MAIN

#include <catch2/catch.hpp>

#include <digit.h>

using namespace asset_id;

/**
 * @brief 
 * 
 * @param character 
 * @return true 
 * @return false 
 */
bool check_valid_digit(const char character)
{
    auto maybe_digit = digit::from_char(character);
    if(!maybe_digit)
    {
        return false;
    }

    const auto expected = static_cast<digit::value_t>(character - '0');
    return expected == maybe_digit->value();
}

TEST_CASE("digit factory function creates correct values from characters representing a digit")
{
    REQUIRE(check_valid_digit('0'));
    REQUIRE(check_valid_digit('1'));
    REQUIRE(check_valid_digit('2'));
    REQUIRE(check_valid_digit('3'));
    REQUIRE(check_valid_digit('4'));
    REQUIRE(check_valid_digit('5'));
    REQUIRE(check_valid_digit('6'));
    REQUIRE(check_valid_digit('7'));
    REQUIRE(check_valid_digit('8'));
    REQUIRE(check_valid_digit('9'));
}

TEST_CASE("digit factory function creates empty option from characters not representing a digit")
{
    REQUIRE(!digit::from_char(' '));
    REQUIRE(!digit::from_char('a'));
    REQUIRE(!digit::from_char('Z'));
    REQUIRE(!digit::from_char('!'));
}