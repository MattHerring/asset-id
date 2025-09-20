#include "asset_id.h"
#include <iostream>
#include <optional>

namespace asset_id
{
std::optional<asset_id_t> create_asset_id(std::string_view const id_str)
{
    if (id_str.size() != asset_id_length)
    {
        std::cout << "create_asset_id received string of wrong length '" << id_str << "'\n";
        return std::nullopt;
    }

    auto result = asset_id_t{};

    for (unsigned i = 0; i < id_str.size(); ++i)
    {
        auto const maybe_digit = digit::from_char(id_str[i]);
        if (!maybe_digit)
        {
            std::cout << "create_asset_id recieved an id with unsupported digit '" << id_str[i]
                      << "', skipping\n";
            return std::nullopt;
        }

        result[i] = *maybe_digit;
    }

    return result;
}

std::optional<checksum_t> calculate_checksum(
    asset_id_t const& asset_id,
    std::uint8_t const digit_base,
    std::uint8_t const checksum_base
)
{
    auto digit_sum = std::uint64_t{0U};
    auto coeff = std::uint64_t{1U};

    for (auto const digit: asset_id)
    {
        digit_sum += coeff * digit.value();
        coeff *= digit_base;
    }

    digit_sum = digit_sum % checksum_base;

    auto const lo_digit = digit::from_int(digit_sum % digit_base);
    auto const hi_digit = digit::from_int(digit_sum / digit_base);
    if (!lo_digit || !hi_digit)
    {
        std::cout << "Failed to represent checksum " << digit_sum
                  << " as an array of digits; failing.";
        return std::nullopt;
    }

    return checksum_t{*hi_digit, *lo_digit};
}

std::optional<checked_asset_id_t> create_checked_asset_id(asset_id_t const& asset_id)
{
    constexpr auto checksum_base = 97U;

    auto const checksum = calculate_checksum(asset_id, digit::base(), checksum_base);
    if (!checksum)
    {
        std::cout << "Failed to calculate checksum\n";
        return std::nullopt;
    }

    auto result = checked_asset_id_t{};

    auto dest = std::copy(std::cbegin(*checksum), std::cend(*checksum), std::begin(result));
    std::copy(std::cbegin(asset_id), std::cend(asset_id), dest);

    return result;
}

} // namespace asset_id