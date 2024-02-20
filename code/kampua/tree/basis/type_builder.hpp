#ifndef KAMPUA_TREE_BASIS_TYPE_BUILDER_HPP
#define KAMPUA_TREE_BASIS_TYPE_BUILDER_HPP

/**
 * @file
 *
 * @ingroup Kampua::Tree::Basis
 *
 * @brief Build type from token.
 */

#include "concepts.hpp"
#include "tree/basis/type.hpp"
#include "kampua/tree/basis/identifier_builder.hpp"

#include <span>
#include <vector>
#include <sstream>
#include <stdexcept>

namespace Kampua::Tree::Basis
{

    //! @brief Build type from token.
    //! @tparam T Character type.
    template <typename T>
        requires Character<T>
    class TypeBuilder
    {
    public:
    private:
    public:
        static auto build(const std::span<Token<T>> &p_tokens)
        try
        {
            // Check tokens availability.
            if (p_tokens.size() == 0)
                throw std::invalid_argument("Unable to build vessel from empty tokens.");

            const auto default_mutability = false;
            const auto mutability_keyword = std::basic_string<T>("mut");

            const auto &last = p_tokens.back();
            const auto rest = std::span<Token<T>>(p_tokens.begin(), std::prev(p_tokens.end()));

            // Retrieve mutabilities.
            auto mutabilities = std::vector<bool>();
            auto mutability = false;
            for (const auto &token : rest)
            {
                const auto &view = std::get<View<T>>(token);
                const auto &metadata = std::get<Metadata<T>>(token);
                const auto string_view = std::basic_string_view<T>(view.begin(), view.end());

                if (metadata.get_tag() == Tag<T>::POINTER_OPERATOR)
                {
                    mutabilities.push_back(mutability);
                    mutability = default_mutability;
                }
                else if (string_view == mutability_keyword)
                {
                    mutability = !default_mutability;
                }
                else
                {
                    std::stringstream message;
                    message << "Unexpected keyword `" << string_view << "` during type definition. " << metadata.make_position_description();
                    throw std::runtime_error(message.str());
                }
            }
            if (mutability != default_mutability)
                mutabilities.push_back(mutability);
            std::reverse(mutabilities.begin(), mutabilities.end());

            // Retrieve type identifier.
            auto identifier = IdentifierBuilder<T>::build(last);

            return Type<T>(std::move(identifier), std::move(mutabilities));
        }
        catch (...)
        {
            throw;
        }

        TypeBuilder() = delete;
        ~TypeBuilder() = delete;
    };

} // namespace Kampua::Tree::Basis

#endif // KAMPUA_TREE_BASIS_TYPE_BUILDER_HPP