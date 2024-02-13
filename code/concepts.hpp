#ifndef CONCEPTS_HPP
#define CONCEPTS_HPP

/**
 * @file
 *
 * @brief Generic concepts.
 */

#include <concepts>
#include <iterator>

template <typename T>
concept ConstContiguousIterable = requires(T p_iterable) {
    {
        p_iterable.cbegin()
    } -> std::contiguous_iterator;
    {
        p_iterable.cend()
    } -> std::contiguous_iterator;
};

template <typename T>
concept ValueTyped = requires {
    typename T::value_type;
};

template <typename T>
concept Character = std::convertible_to<T, char> ||
                    std::convertible_to<T, wchar_t> ||
                    std::convertible_to<T, char8_t> ||
                    std::convertible_to<T, char16_t> ||
                    std::convertible_to<T, char32_t>;

#endif // CONCEPTS_HPP