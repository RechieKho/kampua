#ifndef KAMPUA_TREE_BASIS_VESSEL_BUILDER_HPP
#define KAMPUA_TREE_BASIS_VESSEL_BUILDER_HPP

/**
 * @file
 *
 * @ingroup Kampua::Tree::Basis
 *
 * @brief Build vessel from token.
 */

#include "concepts.hpp"
#include "tree/basis/vessel.hpp"
#include "tree/basis/identifier.hpp"
#include "kampua/cleave/cleaver.hpp"
#include "kampua/tree/basis/identifier_builder.hpp"
#include "kampua/tree/basis/type_builder.hpp"

#include <span>
#include <tuple>
#include <string>
#include <vector>
#include <stdexcept>
#include <sstream>

namespace Kampua::Tree::Basis
{

    //! @brief Build vessel from token.
    //! @tparam T Character Type.
    template <typename T>
        requires Character<T>
    class VesselBuilder
    {
    public:
    private:
    public:
        static auto build(std::span<Token> p_tokens)
        try
        {
            // Check tokens availability.
            if (p_tokens.size() == 0)
                throw std::invalid_argument("Unable to build vessel from empty tokens.");

            // Retrieve identifier.
            const auto &identifier_token = p_tokens[0];
            const auto &identifier_view = std::get<View>(identifier_token);
            const auto &identifier_metadata = std::get<Metadata>(identifier_token);
            auto identifier = IdentifierBuilder<T>::build(std::basic_string<T>(identifier_view.begin(), identifier_view.end()));

            // Check type operator availability.
            if (p_tokens.size() == 1)
            {
                std::stringstream message;
                message << "Expecting a type operator (`:`). " << identifier_metadata.make_position_description();
                throw std::runtime_error(message.str());
            }

            // Check type operator.
            const auto &type_token = p_tokens[1];
            const auto &type_view = std::get<View>(token_token);
            const auto &type_metadata = std::get<Metadata>(token_token);
            if (type_metadata.tag != Tag::TYPE_OPERATOR)
            {
                std::stringstream message;
                message << "Expecting a type operator (`:`) but `" << std::basic_string_view<T>(type_view.begin(), type_view.end()) << "` is given. " << type_metadata.make_position_description();
                throw std::runtime_error(message.str());
            }

            // Check type availability.
            if (p_tokens.size() <= 2)
            {
                std::stringstream message;
                message << "Expecting a type. " << type_metadata.make_position_description();
                throw std::runtime_error(message.str());
            }

            // Retrieve type.
            auto type = TypeBuilder<T>::build(p_tokens.subspan(3));

            return Vessel<T>(std::move(identifier), std::move(type));
        }
        catch (...)
        {
            throw;
        }

        VesselBuilder() = delete;
        ~VesselBuilder() = delete;
    };

} // namespace Kampua::Tree::Basis

#endif // KAMPUA_TREE_BASIS_VESSEL_BUILDER_HPP