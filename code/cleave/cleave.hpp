#ifndef CLEAVE_HPP
#define CLEAVE_HPP

/**
 * @file
 *
 * @ingroup Cleave
 *
 * @brief Generic algorithm to split iterables.
 */

#include "../concepts.hpp"

#include <span>
#include <concepts>
#include <cstdint>

namespace Cleave
{

    using CleaveOptionUnderlyingType = uint8_t;
    using CleaveChunkSize = typename std::size_t;

    //! @brief Option for `cleave` function returned by cleaver.
    //! @see cleave
    enum CleaveOption : CleaveOptionUnderlyingType
    {
        //! @brief Not cleave.
        IGNORE = 0,

        //! @brief Cleave before the current element.
        BEFORE,

        //! @brief Cleave after the current element.
        AFTER,

        //! @brief Cleave and omit the current element.
        OMIT
    };

    //! @brief Cleaver object that handles when to cleave object.
    template <typename T, typename E>
    concept Cleaver = ValueTyped<T> &&
                      std::convertible_to<typename T::value_type, CleaveOptionUnderlyingType> &&
                      requires(T p_cleaver, const E &p_element, CleaveChunkSize p_size) {
                          {
                              p_cleaver.cleave(p_element, p_size)
                          } -> std::same_as<typename T::value_type>;

                          {
                              p_cleaver.terminate(p_size)
                          } -> std::same_as<typename T::value_type>;
                      };

    //! @brief Cleave container based on instruction given by cleaver object.
    //! `p_cleaver::cleave(current_element, chunk_size)` will be called on each of the element in the container.
    //! `p_cleaver::terminate(chunk_size)` will be called when it is finished.
    //! The result of the methods should be able to convert to `CleaveOptionUnderlyingType` to determine how to cleave.
    //! The result of the methods would append onto each chunk to record user's data describing each chunk.
    //! @tparam T Cleaver type.
    //! @tparam C Container type.
    //! @param p_container Container to be cleaved.
    //! @param p_cleaver Cleaver object.
    //! @return A vector of tuple of constant element span and cleaver's return value.
    //! @see Cleaver
    //! @see CleaveOption
    template <typename T, typename C>
        requires ConstContiguousIterable<C> && ValueTyped<C> && Cleaver<T, typename C::value_type>
    auto cleave(const C &p_container, T p_cleaver)
    try
    {
        using Element = const typename C::value_type;
        using CleaverResult = typename T::value_type;
        using View = std::tuple<std::span<Element>, CleaverResult>;
        using Output = std::vector<View>;

        auto chunks = Output();
        auto start = p_container.cbegin();

        // Moving end of span.
        for (auto i = p_container.cbegin(); i != p_container.cend(); i++)
        {
            const auto &current = *i;
            auto result = p_cleaver.cleave(current, i - start);
            auto end = i;

            // Record chunk.
            switch (CleaveOption(result))
            {
            case CleaveOption::BEFORE:
            {
                if (start != i)
                    chunks.push_back(
                        View(
                            std::span<Element>(start, i),
                            std::move(result)));
                start = i;
                break;
            }
            case CleaveOption::AFTER:
            {
                if (start != i)
                    chunks.push_back(
                        View(
                            std::span<Element>(start, i + 1),
                            std::move(result)));
                start = i + 1;
                break;
            }
            case CleaveOption::OMIT:
            {
                if (start != i)
                    chunks.push_back(
                        View(
                            std::span<Element>(start, i),
                            std::move(result)));

                start = i + 1;
            }
            default:
                break;
            }
        }

        // Record chunk.
        if (start != p_container.cend())
            chunks.push_back(
                View(
                    std::span<Element>(start, p_container.cend()),
                    std::move(p_cleaver.terminate(start - p_container.cend()))));

        return chunks;
    }
    catch (...)
    {
        throw;
    }

    //! @brief Forbidding cleaving rvalue.
    template <typename T, typename C>
        requires ConstContiguousIterable<C> && ValueTyped<C> && Cleaver<T, typename C::value_type>
    auto cleave(C &&p_container, T p_cleaver) = delete;

} // namespace Cleave

#endif // CLEAVE_HPP