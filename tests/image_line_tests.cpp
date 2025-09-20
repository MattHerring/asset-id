#include <iostream>
#include <catch2/catch.hpp>

#include "image_line.h"

using namespace asset_id;

TEST_CASE("Base 10 digits return a non-empty optional")
{
    for (auto digit_value = 0U; digit_value < digit::base(); ++digit_value)
    {
        auto const maybe_digit = digit::from_int(digit_value);
        REQUIRE(maybe_digit);

        REQUIRE(digit_to_pixel(*maybe_digit));
    }
}

TEST_CASE("Check the known value for the digit 5")
{
    auto const maybe_five = digit::from_int(5);
    REQUIRE(maybe_five);

    auto const pixels = digit_to_pixel(*maybe_five);
    REQUIRE(pixels);

    // This magic value was copy-pasted from the task documentation NOT the implementation.
    REQUIRE(pixels == 0b11010101);
}

TEST_CASE("create_image_line will not overflow the line")
{
    auto const asset_id = create_asset_id("7890");
    REQUIRE(asset_id);

    auto const digits = create_checked_asset_id(*asset_id);
    REQUIRE(digits);

    for (auto start_byte = image_line_num_bytes - checked_asset_id_length + 1;
         start_byte < image_line_num_bytes;
         ++start_byte)
    {
        REQUIRE(!create_image_line(*digits, start_byte));
    }
}

TEST_CASE("create_image_line injects the exptected bytes")
{
    const auto start_byte = 1U;

    auto const asset_id = create_asset_id("7890");
    REQUIRE(asset_id);

    auto const digits = create_checked_asset_id(*asset_id);
    REQUIRE(digits);

    const auto line = create_image_line(*digits, start_byte);
    REQUIRE(line);

    for(auto index = 0U; index < checked_asset_id_length; ++index)
    {
        auto const expected = digit_to_pixel((*digits)[index]);
        REQUIRE(expected);

        auto const actual = (*line)[index + start_byte];
        REQUIRE(actual == expected);
    }
}


