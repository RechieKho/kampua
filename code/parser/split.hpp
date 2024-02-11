#ifndef PARSER_SPLIT_SPAN_HPP
#define PARSER_SPLIT_SPAN_HPP

#include <vector>
#include <span>
#include <functional>

/**
 * @file
 *
 * @ingroup
 *
 * @brief Generic split algorithm to generate splits through function.
 */

namespace Parser
{

    //! @brief Split vector into spans by given function.
    //! The given function will be called on each element.
    //! It will be splitted on element in which the function call evaluate to true.
    //! It is inclusive, the element to be splitted on is included at the end of the chunk.
    //! @tparam T
    //! @param p_data Data to be splitted.
    //! @param p_split_func Function to determine when to split.
    //! @return Vector of chunks.
    template <typename T>
    std::vector<std::span<T>> split(std::vector<T> &p_data, std::function<bool(const T &)> p_split_func)
    {
        std::vector<std::span<T>> spans;
        auto start = p_data.begin();

        // Moving end of span.
        for (auto i = p_data.begin(); i != p_data.end(); i++)
        {
            const auto &current = *i;

            // Check to be splitted.
            if (p_split_func(current))
            {
                // Record chunk.
                auto end = i;
                end++;
                spans.push_back(std::span<T>(start, end));
                start = end;
            }
        }

        // Record chunk.
        if (start != p_data.end())
            spans.push_back(std::span<T>(start, p_data.end()));

        return spans;
    }
} // namespace Parser

#endif // PARSER_SPLIT_SPAN_HPP