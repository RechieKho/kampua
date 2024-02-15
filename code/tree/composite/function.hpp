#ifndef TREE_COMPOSITE_FUNCTION_HPP
#define TREE_COMPOSITE_FUNCTION_HPP

/**
 * @file
 *
 * @ingroup Tree::Composite
 *
 * @brief Abstract syntax node for functions.
 */

#include "tree/basis/node.hpp"
#include "tree/basis/vessel.hpp"
#include "concepts.hpp"
#include "function_prototype.hpp"

#include <vector>
#include <memory>
#include <span>

namespace Tree::Composite
{

    using namespace Tree::Basis;

    //! @brief Function.
    //! @tparam T Character Type.
    template <typename T>
        requires Character<T>
    class Function : public Node<T>
    {
    private:
        //! @brief Function signature
        //! @see FunctionPrototype
        FunctionPrototype<T> prototype;

        //! @brief List of statements in function body.
        std::vector<std::unique_ptr<Node<T>>> statements;

    public:
        Function(FunctionPrototype<T> p_prototype, std::vector<std::unique_ptr<Node<T>>> p_statements) noexcept
            : prototype(std::move(p_prototype)), statements(std::move(p_statements)) {}

        ~Function() override = default;

        inline auto operator<=>(const Function<T> &p_function) const & noexcept = default;

        inline const auto &view_prototype() const & noexcept { return prototype; }

        inline const auto &view_statements() const & noexcept { return statements; }
    };

} // namespace Tree::Composite

#endif // TREE_COMPOSITE_FUNCTION_HPP
