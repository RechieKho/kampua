#ifndef AST_VESSEL_HPP
#define AST_VESSEL_HPP

/**
 * @file
 *
 * @ingroup AST
 *
 * @brief A value holder of given type.
 */

#include "../concepts.hpp"

#include <string>
#include <vector>
#include <span>

namespace AST
{

    //! @brief A value holder of given type.
    //! Such as variable, parameter and etc.
    //! @tparam T Character Type.
    template <typename T>
        requires Character<T>
    class Vessel
    {
    private:
        //! @brief Name of the value holder.
        std::basic_string<T> identifier;

        //! @brief Type of the value it holds.
        std::basic_string<T> type;

        //! @brief Mutability of the holder and its pointed value (if it is a pointer).
        std::vector<bool> mutability;

    public:
        Vessel(std::basic_string<T> p_identifier, std::basic_string<T> p_type, std::vector<bool> p_mutability) noexcept
            : identifier(std::move(p_identifier)), type(std::move(p_type)), mutability(std::move(p_mutability)) {}

        inline auto operator<=>(const Vessel<T> &p_vessel) const & = default;

        inline auto get_pointer_degree() const & noexcept { return mutability.size(); }

        inline const std::basic_string<T> &view_identifier() const & noexcept { return identifier; }

        inline const std::basic_string<T> &view_type() const & noexcept { return type; }

        inline const std::vector<bool> &view_mutability() const & noexcept { return mutability; }
    };

} // namespace AST

#endif // AST_VESSEL_HPP