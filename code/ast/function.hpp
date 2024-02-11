#ifndef AST_FUNCTION_HPP
#define AST_FUNCTION_HPP

/**
 * @file
 *
 * @ingroup AST
 *
 * @brief Abstract syntax node for functions.
 */

#include <vector>
#include <memory>
#include <span>

#include "asn.hpp"
#include "function_prototype.hpp"

namespace AST
{

    //! @brief Function.
    class Function : public ASN
    {
    private:
        //! @brief Function signature
        //! @see FunctionPrototype
        FunctionPrototype prototype;

        //! @brief List of statements in function body.
        std::vector<std::unique_ptr<ASN>> statements;

    public:
        Function(const FunctionPrototype &p_prototype, std::vector<std::unique_ptr<ASN>> p_statements) noexcept;
        Function(FunctionPrototype &&p_prototype, std::vector<std::unique_ptr<ASN>> p_statements) noexcept;
        ~Function() override;

        const FunctionPrototype &view_prototype() const;
        const std::vector<std::unique_ptr<ASN>> &view_statements() const;
    };

} // namespace AST

#endif // AST_FUNCTION_HPP
