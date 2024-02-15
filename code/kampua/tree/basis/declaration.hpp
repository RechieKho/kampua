#ifndef KAMPUA_TREE_BASIS_DECLARATION_HPP
#define KAMPUA_TREE_BASIS_DECLARATION_HPP

/**
 * @file
 *
 * @ingroup Kampua::Tree::Basis
 *
 * @brief Type alias.
 */

#include "concepts.hpp"
#include "tree/basis/identifier.hpp"

namespace Kampua::Tree::Basis
{

    //! @brief Cleaved chunks.
    //! @tparam T Character type.
    template <typename T>
        requires Character<T>
    using View = std::span<T>;

    //! @brief Metadata for the token.
    //! @tparam T Character type.
    template <typename T>
        requires Character<T>
    using Metadata = typename ::Kampua::Cleave::Cleaver<T>::value_type;

    //! @brief Token
    //! @tparam T Character type.
    template <typename T>
        requires Character<T>
    using Token = std::tuple<ViewType<T>, MetadataType<T>>;

    //! @brief Token's Tag.
    //! @tparam T Character type.
    template <typename T>
        requires Character<T>
    using Tag = typename ::Kampua::Cleave::Cleaver<T>::Tag;

    template <typename T>
        requires Character<T>
    using Identifier = ::Tree::Basis::Identifier<T>;

    template <typename T>
        requires Character<T>
    using Type = ::Tree::Basis::Type<T>;

    template <typename T>
        requires Character<T>
    using Vessel = ::Tree::Basis::Vessel<T>;

} // namespace Kampua::Tree::Basis

#endif // KAMPUA_TREE_BASIS_DECLARATION_HPP