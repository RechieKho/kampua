#ifndef TREE_BASIS_VESSEL_HPP
#define TREE_BASIS_VESSEL_HPP

/**
 * @file
 *
 * @ingroup Tree::Basis
 *
 * @brief A value holder of given type.
 */

#include "concepts.hpp"
#include "tree/basis/identifier.hpp"
#include "tree/basis/type.hpp"

#include <string>
#include <vector>
#include <span>

namespace Tree::Basis
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
        Identifier<T> identifier;

        //! @brief Type of the value holder.
        Type<T> type;

    public:
        Vessel(Identifier<T> p_identifier, Type<T> p_type) noexcept
            : identifier(std::move(p_identifier)), type(std::move(p_type)) {}

        inline auto operator<=>(const Vessel<T> &p_vessel) const & noexcept = default;

        inline const auto &view_identifier() const & noexcept { return identifier; }

        inline const auto &view_type() const & noexcept { return type; }
    };

} // namespace Tree::Basis

#endif // TREE_BASIS_VESSEL_HPP