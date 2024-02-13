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

#endif // CONCEPTS_HPP