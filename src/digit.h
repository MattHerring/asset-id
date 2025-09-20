/**
 * @file   digit.h
 * @brief  Introduces an class to represent individual digits of an integer.
 */
#pragma once

#include <cstdint>
#include <optional>

namespace asset_id
{
/**
 * @brief The `digit` type represents a single base 10 digit.
 * 
 * This tool processes integers not by value but as a collection of digits. 
 * Using the std::array type these collections can be of a fixed size, 
 * which well suits the tools applications.
 *
 * @remark The `digit` type shall be used to instantiate `std::array` which requires
 * the default constructor to be implemented. I follow the `rule of 5` so provide 
 * trivial implementations of the remaining special functions.
 */
class digit
{
public:
    using value_t = std::uint8_t;

    digit() = default;
    digit(digit const&) = default;
    digit(digit&&) = default;

    digit& operator=(digit const&) = default;
    digit& operator=(digit&&) = default;

    /**
     * @return constexpr std::uint8_t holding the numeric base of the digit type.
     */
    constexpr static std::uint8_t base() { return 10U; }

    /**
    * @brief Attempt to create an instance of `digit` from an integer.
    *
    * @param character the integer to try to instantiate as a digit.
    *
    * @return std::optional<digit> containing the correct instance of `digit` if this 
    *         exists; empty optional otherwise.
    */
    static std::optional<digit> from_int(value_t integer_value);

    /**
    * @brief Attempt to create an instance of `digit` from a character.
    *
    * In base 10 the correct characters are, as expected, `0`, ..., `9`.
    * 
    * @param character the character to try to instantiate as a digit.
    *
    * @return std::optional<digit> containing the correct instance of `digit` if this 
    *         exists; empty optional otherwise.
    */
    static std::optional<digit> from_char(char character);

    /**
     * @return value_t holding the value of this instance of `digit`.
     */
    value_t value() const { return _value; }

private:
    /**
    * @brief Construct a new digit object from an integer. This constructor is private
    * so is only called by `digit::from_char` and we can assume that the integer 
    * parameter represents a valid digit.
    * 
    * @param value The value of the digit to construct; assumed to be in the expected 
    *              range for the digit base.
    */
    explicit digit(value_t value):
        _value(value)
    {
    }

    /**
    * @brief Utility function to signal whether a given character represents an instance
    * of a digit. 
    * 
    * In base 10 the correct characters are, as expected, `0`, ..., `9`.
    * 
    * @param value  the character to check.
    *
    * @return true  if the `value` parameter represents a digit.
    * @return false otherwise.
    */
    static constexpr bool is_digit(char value);

    value_t _value;
};

} // namespace asset_id