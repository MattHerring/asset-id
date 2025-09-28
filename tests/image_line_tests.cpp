#include <bitset>
#include <initializer_list>
#include <iostream>
#include <catch2/catch.hpp>
#include <string>

#include "image_line.h"

using namespace asset_id;

namespace
{
    /**
     * @brief A test helper function that reproduces the bit patterns used to render 
     * a digit on the lcd display by unsetting specific bits in an integer.
     *
     * The protocol for the bit pattern is unconventional in that the order of the 
     * bits is reversed; `0b10000000` in the protocol has bit 0 set and all others 
     * clear. The protocol also states that bit 4 (in the non-standard ordering) 
     * will allways be clear.
     *
     * The `asset_id` tool maps digits to bit patterns via a look up table of manually 
     * calculated values; this function is used to increase the confidence in this look
     * up table. 
     *
     * I recommend complete validation of the bit patterns by using a physical display. 
     * 
     * @param bit_indexes    the indexes to unset (all others will be set 'true' apart 
     *                       from bit 4); the index is in the protocol ordering i.e. 
     *                       reversed to normal.
     *
     * @return pixel_byte_t  the calculated pattern of bits to represent a digit.
     */
    pixel_byte_t clear_bits(const std::initializer_list<std::uint8_t>& bits_to_clear)
    {
        auto result = std::bitset<8U>{};
        result.set();

        // Bit 4 (in protocol order) is always clear; this is bit 3 in the standard order.
        result.set(3, false);
        
        for(const auto index : bits_to_clear)
        {
            const auto standard_index = result.size() - index - 1U;
            result.set(standard_index, false);
        }

        // As the bitset is of size 8, the cast to `pixel_byte_t` is safe.
        return static_cast<pixel_byte_t>(result.to_ulong());
    }

} // namespace

TEST_CASE("Check the known value for the digit 5")
{
    // I am nervous about the `clear_bits` function, this test expressly validates it 
    // for the known bit pattern for the digit `5`.
    auto const maybe_five = digit::from_int(5);
    REQUIRE(maybe_five);

    auto const pixels = digit_to_pixel(*maybe_five);
    REQUIRE(pixels);

    // This magic value was copy-pasted from the task documentation NOT the implementation.
    REQUIRE(*pixels == 0b11010101);
    REQUIRE(*pixels == clear_bits({6, 2}));   
}

TEST_CASE("Base 10 digits return a non-empty optional which matches the generated pixels")
{
    const std::vector<pixel_byte_t> generated_pixels 
    {
        clear_bits({ 0 }), // 0
        clear_bits({ 0, 2, 3, 4, 5, 7 }), // 1
        clear_bits({ 1, 7 }), // 2
        clear_bits({ 2, 7 }), // 3
        clear_bits({ 2, 3, 5 }), // 4
        clear_bits({ 2, 6 }), // 5
        clear_bits({ 6 }), // 6
        clear_bits({ 0, 2, 3, 7}), // 7
        clear_bits({ }), // 8
        clear_bits({ 2 }), // 9
    };

    REQUIRE(generated_pixels.size() == digit::base());

    for (auto digit_value = 0U; digit_value < digit::base(); ++digit_value)
    {
        auto const maybe_digit = digit::from_int(digit_value);
        REQUIRE(maybe_digit);

        const auto pixels = digit_to_pixel(*maybe_digit);
        REQUIRE(pixels);

        REQUIRE(*pixels == generated_pixels[digit_value]);
    }
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


