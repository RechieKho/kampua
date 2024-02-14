#ifndef AST_FUNCTION_HPP
#define AST_FUNCTION_HPP

/**
 * @file
 *
 * @ingroup Tree
 *
 * @brief Abstract syntax node for functions.
 */

#include "node.hpp"
#include "function_prototype.hpp"
#include "../concepts.hpp"

#include <vector>
#include <memory>
#include <span>

namespace Tree
{

    //! @brief Function.
    //! @tparam T Character Type.
    template <typename T>
        requires Character<T>
    class Function : public Node
    {
    private:
        //! @brief Function signature
        //! @see FunctionPrototype
        FunctionPrototype<T> prototype;

        //! @brief List of statements in function body.
        std::vector<std::unique_ptr<Node>> statements;

    public:
        Function(FunctionPrototype<T> p_prototype, std::vector<std::unique_ptr<Node>> p_statements) noexcept
            : prototype(std::move(p_prototype)), statements(std::move(p_statements)) {}

        ~Function() override = default;

        inline auto operator<=>(const Function<T> &p_function) const & = default;

        inline const FunctionPrototype<T> &view_prototype() const & noexcept { return prototype; }

        inline const std::vector<std::unique_ptr<Node>> &view_statements() const & noexcept { return statements; }
    };

} // namespace Tree

#endif // AST_FUNCTION_HPP
