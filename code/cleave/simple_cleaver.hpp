#ifndef CLEAVE_SIMPLE_CLEAVER_HPP
#define CLEAVE_SIMPLE_CLEAVER_HPP

#include "cleave.hpp"

#include <memory>

namespace Cleave
{

    template <typename T, CleaveOption C = CleaveOption::OMIT>
    class SimpleCleaver
    {
    private:
        T value;

    public:
        using value_type = CleaveOption;

        SimpleCleaver(T &&p_value)
            : value(std::forward<T>(p_value)) {}

        ~SimpleCleaver() = default;

        value_type cleave(const T &p_value) const
        {
            if (p_value == value)
                return C;
            else
                return CleaveOption::IGNORE;
        }

        value_type terminate() const
        {
            return CleaveOption::IGNORE;
        }
    };

} // namespace Cleave

#endif // CLEAVE_SIMPLE_CLEAVER_HPP