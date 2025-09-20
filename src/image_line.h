/**
 * @file   image_line.h
 * @brief  Type used to represent the lines of pixels that will be saved as a png file.
 *
 * Each base 10 digit has a well defined bit mapping that will render it on a 7 segment lcd display.
 * This file contains the tools to convert digits (and collections of digits) to these bit-mapped 
 * values ready for serialisation as a png file.
 */
#pragma once

#include <array>
#include <optional>

#include "asset_id.h"

namespace asset_id
{
/**
 * @brief The `pixel_byte_t` represents the bit-mapped type for displaying a particular digit on a 7 
 * segment lcd display.
 *
 * As `pixel_byte_t` has 8 bits we (arbitrarily) discard the 4th bit which is always set to '0'.
 * IMPORTANT: the order of the bits in a `pixel_byte_t` is reversed from normal conventions.
 * For example in standard C/C++ the unsigned integer value '0b01000000' has the 6th bit set; if this 
 * value were used to instantiate a `pixel_byte_t` then we would refer to bit 1 instead.
 */
using pixel_byte_t = std::uint8_t;

/**
 * @brief This tool will write png images that are 256 pixels wide (and 1 pixel high) and each
 * pixel only requires 1 bit.
 */
constexpr auto const image_line_width_pixels = 256U;
constexpr auto const image_line_num_bytes = 32U;

/**
 * @brief The `image_line_t` is used to hold a the bits that will be written as a single line png file.
 */
using image_line_t = std::array<pixel_byte_t, image_line_num_bytes>;

/**
 * @brief Convert an instance of `digit` to the corresponding bit-pattern that will render the digit 
 * on a 7 segment lcd display.
 * 
 * @param a_digit the digit to map.
 *
 * @return std::optional<pixel_byte_t> containing the bit-pattern if the mapping was successful; 
 *         empty optional otherwise.
 */
std::optional<pixel_byte_t> digit_to_pixel(digit a_digit);

/**
 * @brief Attempt to instantiate `image_line_t` containing the bit-patterns to render a given asset id 
 * and checksum. 
 *
 * The bit patterns can be placed starting at any (byte) entry in the instantiated `image_line_t`. 
 * This does not permit the bit-patterns to start midway in a particular entry of `image_line_t`.
 * 
 * All pixels apart from the bit-patterns will be set to 0.
 * 
 * @param asset_id     the collection of digits for a checksum and asset id to render.
 * @param start_index  the index of the first entry in `image_line_t` to carry the bit-patterns.
 *
 * @return std::optional<image_line_t> containing the asset id bit patterns if successfully created; 
 *        the empty optional otherwise, including a value of `start_index` that would cause out of bounds access.
 */
std::optional<image_line_t>
create_image_line(checked_asset_id_t const& asset_id, std::size_t start_index);
} // namespace asset_id