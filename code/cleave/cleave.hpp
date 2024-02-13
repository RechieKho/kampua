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

    using CleaveOptionType = uint8_t;

    //! @brief Option for `cleave` function returned by cleaver.
    //! @see cleave
    enum CleaveOption : CleaveOptionType
    {
        //! @brief Not cleave.
        IGNORE = 0,

        //! @brief Cleave before the current element.
        BEFORE,

        //! @brief Cleave after the current element.
        AFTER,

        //! @brief Cleave before and after the current element.
        BOTH,

        //! @brief Cleave and omit the current element.
        OMIT
    };

    //! @brief Cleaver object that handles when to cleave object.
    template <typename T, typename E>
    concept Cleaver = ValueTyped<T> &&
                      std::convertible_to<typename T::value_type, CleaveOptionType> &&
                      requires(T p_cleaver, const E &p_element) {
                          {
                              p_cleaver.cleave(p_element)
                          } -> std::same_as<typename T::value_type>;

                          {
                              p_cleaver.terminate()
                          } -> std::same_as<typename T::value_type>;
                      };

    //! @brief Cleave container based on instruction given by cleaver object.
    //! `p_cleaver::cleave(current_element)` will be called on each of the element in the container.
    //! `p_cleaver::terminate()` will be called when it is finished.
    //! The result of the methods should be able to convert to `CleaveOptionType` to determine how to cleave.
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
        using ElementType = const typename C::value_type;
        using CleaverResultType = typename T::value_type;
        using ViewType = std::tuple<std::span<ElementType>, CleaverResultType>;
        using OutputType = std::vector<ViewType>;

        auto chunks = OutputType();
        auto start = p_container.cbegin();

        // Moving end of span.
        for (auto i = p_container.cbegin(); i != p_container.cend(); i++)
        {
            const auto &current = *i;
            auto result = p_cleaver.cleave(current);
            auto end = i;

            // Record chunk.
            switch (CleaveOption(result))
            {
            case CleaveOption::BEFORE:
            {
                chunks.push_back(
                    ViewType(
                        std::span<ElementType>(start, i),
                        std::move(result)));
                start = i;
                break;
            }
            case CleaveOption::AFTER:
            {
                chunks.push_back(
                    ViewType(
                        std::span<ElementType>(start, i + 1),
                        std::move(result)));
                start = i + 1;
                break;
            }
            case CleaveOption::BOTH:
            {
                chunks.push_back(
                    ViewType(
                        std::span<ElementType>(start, i),
                        std::move(result)));

                chunks.push_back(
                    ViewType(
                        std::span<ElementType>(i, i + 1),
                        std::move(result)));

                start = i + 1;
                break;
            }
            case CleaveOption::OMIT:
            {
                chunks.push_back(
                    ViewType(
                        std::span<ElementType>(start, i),
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
                ViewType(
                    std::span<ElementType>(start, p_container.cend()),
                    std::move(p_cleaver.terminate())));

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