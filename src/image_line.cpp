#include "image_line.h"
#include <iostream>
#include <optional>

namespace asset_id
{
std::optional<pixel_byte_t> digit_to_pixel(digit const a_digit)
{
    static_assert(
        static_cast<std::uint8_t>(digit::base()) == 10U,
        "digit_to_pixel is assuming base 10 digits are used."
    );

    if (a_digit.value() >= digit::base())
    {
        std::cout << "digit_to_pixel recieved an unsupported digit: '" << a_digit.value()
                  << "', failng.\n";
        return std::nullopt;
    }

    static std::array<pixel_byte_t, 10U> const pixels_per_digit = {
        0b01110111,
        0b01000010,
        0b10110110,
        0b11010110,
        0b11000011,
        0b11010101,
        0b11110101,
        0b01000110,
        0b11110111,
        0b11010111,
    };

    return pixels_per_digit[a_digit.value()];
}

std::optional<image_line_t>
create_image_line(checked_asset_id_t const& asset_id, std::size_t start_index)
{
    if (start_index + asset_id.size() > image_line_num_bytes)
    {
        std::cout << "Cannot embed pixels into buffer\n";
        return std::nullopt;
    }

    image_line_t result{};
    for (auto digit_index = 0U; digit_index < asset_id.size(); ++digit_index)
    {
        auto const& digit = asset_id[digit_index];
        auto const pixel_byte = digit_to_pixel(digit);
        if (!pixel_byte)
        {
            std::cout << "Failed to convert digit " << digit.value() << "' to pixel\n";
            return std::nullopt;
        }

        result[digit_index + start_index] = *pixel_byte;
    }

    return result;
}

} // namespace asset_id