#include "digit.h"
#include <iostream>

namespace asset_id
{

constexpr bool digit::is_digit(char const value)
{
    static_assert(digit::base() == 10U, "digit::from_char is assuming base 10 digits are used.");

    return (value >= '0') && (value <= '9');
}

std::optional<digit> digit::from_int(value_t integer_value)
{
    static_assert(digit::base() == 10U, "digit::from_int is assuming base 10 digits are used.");
    if (integer_value >= digit::base())
    {
        std::cout << "digit::from_int failed instantiating with '" << integer_value << "', failing.\n";
        return std::nullopt;
    }

    return digit{integer_value};
}

std::optional<digit> digit::from_char(char const character)
{
    static_assert(digit::base() == 10U, "digit::from_char is assuming base 10 digits are used.");
    if (!is_digit(character))
    {
        std::cout << "digit::from_char failed instantiating with '" << character << "' failing.\n";
        return std::nullopt;
    }

    return digit{static_cast<std::uint8_t>(character - '0')};
}

} // namespace asset_id
