#ifndef CONCEPTS_HPP
#define CONCEPTS_HPP

/**
 * @file
 *
 * @brief Generic concepts.
 */

#include <concepts>

template <typename T>
concept Iterable = requires(T iterable) {
    iterable.begin();
    iterable.end();
};

template <typename T>
concept ValueTyped = requires {
    typename T::value_type;
};

#endif // CONCEPTS_HPP