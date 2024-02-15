#ifndef TREE_BASIS_TYPE_HPP
#define TREE_BASIS_TYPE_HPP

#include "concepts.hpp"
#include "tree/basis/identifier.hpp"

#include <string>

namespace Tree::Basis
{

    template <typename T>
        requires Character<T>
    class Type
    {
    private:
        //! @brief Name of the type.
        Identifier<T> identifier;

        //! @brief Mutability of the holder and its pointed value (if it is a pointer).
        std::vector<bool> mutability;

    public:
        Type(Identifier<T> p_identifier, std::vector<bool> p_mutability) noexcept
            : identifier(std::move(p_identifier)), mutability(std::move(p_mutability)) {}

        inline auto operator<=>(const Type<T> &p_type) const & noexcept = default;

        inline const auto &view_identifier() const & noexcept { return identifier; }

        inline const auto &view_mutability() const & noexcept { return mutability; }
    };

} // namespace Tree::Basis

#endif // TREE_BASIS_TYPE_HPP