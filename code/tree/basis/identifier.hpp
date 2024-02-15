#ifndef TREE_BASIS_IDENTIFIER_HPP
#define TREE_BASIS_IDENTIFIER_HPP

#include "concepts.hpp"

#include <string>
#include <sstream>

namespace Tree::Basis
{

    template <typename T>
        requires Character<T>
    class Identifier
    {
    private:
        std::basic_string<T> label;

    public:
        static bool is_valid_identifier(const std::basic_string<T> &p_label) noexcept
        {
            // Empty label is not a valid identifier.
            if (p_label.size() == 0)
                return false;

            // First character should only be alphebet or underscore.
            const auto &first = p_label.first();
            if (!isalpha(first) && first != T('_'))
                return false;

            // Rest should be alphanumeric or underscore.
            const auto rest = std::basic_string_view<T>(std::next(p_label.cbegin()), p_label.cend());
            for (const auto &character : rest)
                if (!isalnum(character) && character != T('_'))
                    return false;

            return true;
        }

        Identifier(std::basic_string<T> p_label)
            : label()
        {
            if (!is_valid_identifier(p_label))
            {
                std::stringstream message;
                message << "`" << p_label << "` is not a valid identifier.";
                // Raise error.
                throw std::runtime_error(message.str().c_str());
            }

            label = std::move(p_label);
        }

        inline auto operator<=>(const Identifier<T> &p_identifier) const & noexcept = default;

        inline const auto operator->() const & noexcept { return &label; }
    };

} // namespace Tree::Basis

#endif // TREE_BASIS_IDENTIFIER_HPP