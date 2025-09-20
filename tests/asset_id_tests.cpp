#include <algorithm>
#include <catch2/catch.hpp>
#include <cstddef>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <optional>
#include <sstream>

#include "asset_id.h"
#include "digit.h"

using namespace asset_id;

/**
 * @brief A test helper function that attempts to create an instance of `asset_id_t`
 * from an integer value. 
 * 
 * @param value the integer value to create the `asset_id_t` from.

 * @return std::optional<asset_id_t> containing the `asset_id_t` if the construction 
 *         was possible; empty optional otherwise.
 */
std::optional<asset_id_t> create_asset_id(unsigned const value)
{
    if (value >= 10000) { return std::nullopt; }

    std::stringstream iss;
    iss << std::setw(4) << std::setfill('0') << value;

    return create_asset_id(iss.str());
}

/**
 * @brief A test helper function that converts an arbitrary sized array of digits 
 * into the equivalent integer value.
 * 
 * @tparam N         The number of digits in the array.
 * @param digits     The collection of digits. 
 * 
 * @return unsigned containing the equivalent integer value.
 */
template<std::size_t N>
unsigned as_integer(std::array<digit, N> digits)
{
    auto result = 0U;
    const auto base = digit::base();

    for(const auto digit : digits)
    {
        result = digit.value() + base * result;
    }

    return result;
}

/**
 * @brief Test helper that validates that an instance of `asset_id_t` has the expected 
 * digits.
 *
 * The check is implicit in that the `asset_id_t` instance is converted to its integer 
 * value and compared with an expected value. 
 * 
 * @param str       the string representation of the `asset_id_t`
 * @param expected  the expected value of the id.
 *
 * @return true    if `asset_id_t` can be instantiated from `str` and that its 
 *                 underlying value is the same as `expected`.
 * @return false   otherwise.
 */
bool check_asset_id_creation(const std::string& str, const unsigned expected)
{
    const auto asset_id = create_asset_id(str);
    return asset_id && (expected == as_integer(*asset_id));
}

TEST_CASE("create_asset_id succeeds with 4 digits with zero padding")
{
    REQUIRE(check_asset_id_creation("0000", 0));
    REQUIRE(check_asset_id_creation("0001", 1));
    REQUIRE(check_asset_id_creation("0020", 20));
    REQUIRE(check_asset_id_creation("0345", 345));
    REQUIRE(check_asset_id_creation("5678", 5678));
}

TEST_CASE("create_asset_id fails with negative numbers")
{
    REQUIRE(!create_asset_id("-1234"));
}

TEST_CASE("create_asset_id only accepts digits")
{
    REQUIRE(!create_asset_id("123a"));
    REQUIRE(!create_asset_id("!123"));
    REQUIRE(!create_asset_id("!./-="));
}

TEST_CASE("create_asset_id fails for digits of length != 4")
{
    REQUIRE(!create_asset_id("1"));
    REQUIRE(!create_asset_id("12"));
    REQUIRE(!create_asset_id("123"));
    REQUIRE(!create_asset_id("12345"));
    REQUIRE(!create_asset_id("123456"));
}

TEST_CASE("create_asset_id doesn't skip whitespace")
{
    REQUIRE(!create_asset_id(" 1234"));
    REQUIRE(!create_asset_id("1234 "));
    REQUIRE(!create_asset_id(" 1234 "));
    REQUIRE(!create_asset_id("12 34"));
}

TEST_CASE("Example value is given correct checksum")
{
    auto const asset_id = create_asset_id(1337);
    REQUIRE(asset_id);

    const auto checksum = calculate_checksum(*asset_id, 10U, 97U);
    REQUIRE(checksum);

    REQUIRE(56 == as_integer(*checksum));
}

TEST_CASE("Example value is not correct by coincidence")
{
    // A very silly test. Just make sure that the checksum of the reversal of '1337' 
    // is different from '56'. That means that the previous test (probably) didn't pass
    // by coincidence.
    auto const asset_id = create_asset_id(7331);
    REQUIRE(asset_id);

    const auto checksum = calculate_checksum(*asset_id, 10U, 97U);
    REQUIRE(checksum);

    REQUIRE(56 != as_integer(*checksum));
}

TEST_CASE("Checksum is always less than the modulus")
{
    const auto modulus = 97U;
    for(auto i = 0; i < 10000; ++i)
    {
        auto const asset_id = create_asset_id(i);
        REQUIRE(asset_id);
        
        const auto checksum = calculate_checksum(*asset_id, 10U, modulus);
        REQUIRE(checksum);

        REQUIRE(as_integer(*checksum) < modulus);
    }   
}

TEST_CASE("Checked asset id is correctly formed from checksum and asset_id")
{
    const auto modulus = 97U;
    for(auto i = 0; i < 10000; ++i)
    {
        auto const asset_id = create_asset_id(i);
        REQUIRE(asset_id);
        
        const auto checksum = calculate_checksum(*asset_id, 10U, modulus);
        REQUIRE(checksum);

        const auto expected = 10000 * as_integer(*checksum) + as_integer(*asset_id);

        const auto checked = create_checked_asset_id(*asset_id);
        REQUIRE(checked);

        REQUIRE(expected == as_integer(*checked));
    }   
}

