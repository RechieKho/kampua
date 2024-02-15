#ifndef CLEAVE_SIMPLE_CLEAVER_HPP
#define CLEAVE_SIMPLE_CLEAVER_HPP

#include "cleave.hpp"

#include <memory>

/**
 * @file
 *
 * @ingroup Cleave
 *
 * @brief Simple Cleaver for `cleave` function.
 * @see Cleave::cleave
 */

namespace Cleave
{

    //! @brief Simple Cleaver for `cleave` function.
    //! Cleave on `SimpleCleaver::value` by cleave option specified.
    //! @tparam T Type of element.
    //! @tparam C Cleave option.
    template <typename T, CleaveOption C = CleaveOption::OMIT>
    class SimpleCleaver
    {
    private:
        T value;

    public:
        using value_type = CleaveOption;

        SimpleCleaver(T &&p_value)
            : value(std::forward<T>(p_value)) {}

        value_type cleave(const T &p_value, CleaveChunkSize p_size) const
        {
            if (p_value == value)
                return C;
            else
                return CleaveOption::IGNORE;
        }

        value_type terminate(CleaveChunkSize p_size) const
        {
            return CleaveOption::IGNORE;
        }
    };

} // namespace Cleave

#endif // CLEAVE_SIMPLE_CLEAVER_HPP