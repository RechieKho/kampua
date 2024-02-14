#ifndef AST_FUNCTION_PROTOTYPE_HPP
#define AST_FUNCTION_PROTOTYPE_HPP

/**
 * @file
 *
 * @ingroup Tree
 *
 * @brief Abstract syntax node for function prototype.
 */

#include "node.hpp"
#include "vessel.hpp"
#include "../concepts.hpp"

#include <string>

namespace Tree
{

    //! @brief Signature of a function.
    //! @tparam T Character Type.
    template <typename T>
        requires Character<T>
    class FunctionPrototype : public Node
    {
    private:
        //! @brief Name of the function.
        std::basic_string<T> identifier;

        //! @brief Parameters of the function.
        std::vector<Vessel<T>> parameters;

    public:
        FunctionPrototype(std::basic_string<T> p_identifier, std::vector<Vessel<T>> p_parameters) noexcept
            : identifier(std::move(p_identifier)), parameters(std::move(parameters)) {}

        ~FunctionPrototype() override = default;

        inline auto operator<=>(const FunctionPrototype<T> &p_prototype) const & = default;

        inline const std::basic_string<T> &view_identifier() const & noexcept { return identifier; }

        inline const std::vector<Vessel<T>> &view_parameters() const & noexcept { return identifier; }
    };

} // namespace Tree

#endif // AST_FUNCTION_PROTOTYPE_HPP