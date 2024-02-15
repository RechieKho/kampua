#ifndef KAMPUA_TREE_BASIS_IDENTIFIER_BUILDER_HPP
#define KAMPUA_TREE_BASIS_IDENTIFIER_BUILDER_HPP

/**
 * @file
 *
 * @ingroup Kampua::Tree::Basis
 *
 * @brief Build identifier from token.
 */

#include "concepts.hpp"
#include "tree/basis/identifier.hpp"
#include "kampua/tree/basis/declaration.hpp"

#include <sstream>

namespace Kampua::Tree::Basis
{

    //! @brief Build identifier from token.
    //! @tparam T Character type.
    template <typename T>
        requires Character<T>
    class IdentifierBuilder
    {
    public:
    private:
    public:
        static auto build(const Token<T> &p_tokens)
        try
        {
            const auto &view = std::get<View<T>>(p_tokens);
            return Identifier<T>(std::basic_string<T>(view.begin(), view.end()));
        }
        catch (std::exception e)
        {
            const auto &metadata = std::get<Metadata<T>>(p_tokens);
            std::stringstream message;
            message << e.what() << " " << metadata.make_position_description();
            throw std::runtime_error(message.str());
        }

        IdentifierBuilder() = delete;
        ~IdentifierBuilder() = delete;
    };

} // namespace Kampua::Tree::Basis

#endif // KAMPUA_TREE_BASIS_IDENTIFIER_BUILDER_HPP