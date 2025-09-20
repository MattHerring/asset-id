/**
 * @file   asset_id.h
 * @brief  Introduces types and values used to represent numbers in this tool.
 *
 * This tool represents integers as collections of digits (in big-endian order).
 * This representation has been chosen as the logic in the tool is largely based on 
 * manipulating individual digits in a number so this is more nature than holding an 
 * integer as a simple value.
 * 
 * The tool has specific contracts on the size of the integers that will be analysed 
 * so using fixed size containers (`std::array`) will enforce these contracts naturally.
 */
 
 #pragma once

#include <array>
#include <optional>
#include <string_view>

#include "digit.h"

namespace asset_id
{
/**
 * @brief The asset_id_t type is used to hold the digits of an asset id.
 *
 * The value asset_id_length is declared separately as it is used in several
 * parts of this tools implementation and std::array type only exposes its size
 * via instance methods.
 */
constexpr auto asset_id_length = 4U;
using asset_id_t = std::array<digit, asset_id_length>;

/**
 * @brief The checksum_t type is used to hold the digits of a checksum which is 
 * used in this tool to validate an instance of asset_id_t.
 *
 * The value checksum_length is declared separately as it is used in several
 * parts of this tools implementation and std::array type only exposes its size
 * via instance methods.
 */
constexpr auto checksum_length = 2U;
using checksum_t = std::array<digit, checksum_length>;

/**
 * @brief The checked_asset_id_t type holds the digits of an asset_id_t and its 
 * calculate checksum. The digits of the checksum are placed at the start of 
 * checked_asset_id_t followed by the asset_id_t digits.
 *
 * The value checked_asset_id_length is declared separately as it is used in several
 * parts of this tools implementation and std::array type only exposes its size
 * via instance methods.
 */
constexpr auto checked_asset_id_length = asset_id_length + checksum_length;
using checked_asset_id_t = std::array<digit, checked_asset_id_length>;

/**
 * @brief Create an instance of asset_id_t from a string.
 *
 * The string must be exactly 4 characters long, and consist entirely of base 10 digits.
 * 
 * @param id_str string containing the asset_id digits.
 *
 * @return std::optional<asset_id_t> containing the id digits if `id_str` is of the correct 
 *         form; empty optional otherwise.
 */
std::optional<asset_id_t> create_asset_id(std::string_view id_str);

/**
 * @brief Calculates a simple checksum on an instance of asset_id.
 * 
 * @param asset_id       the instance to calculate the checksum on.
 * @param digit_base     weight to apply to each digit of the asset_id
 * @param checksum_base 
 *
 * @return std::optional<checksum_t> containing the digits of the checksum if the 
*          calculation succeeded; empty optional otherwise.
 */
std::optional<checksum_t> calculate_checksum(
    asset_id::asset_id_t const& asset_id,
    std::uint8_t digit_base,
    std::uint8_t checksum_base
);

/**
 * @brief Create a instance of checked_asset_id_t by prepending an asset_id with the digits
 *        of its checksum.
 * 
 * @param asset_id  the instance to use.
 * @return std::optional<checked_asset_id_t> containing the checksum digits followed by the 
 *         asset_id digits if this calculation succeeded; empty optional otherwise.
 */
std::optional<checked_asset_id_t> create_checked_asset_id(asset_id_t const& asset_id);
} // namespace asset_id