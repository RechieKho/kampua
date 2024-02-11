#ifndef AST_FUNCTION_PROTOTYPE_HPP
#define AST_FUNCTION_PROTOTYPE_HPP

/**
 * @file
 *
 * @ingroup AST
 *
 * @brief Abstract syntax node for function prototype.
 */

#include <string>

#include "asn.hpp"
#include "vessel.hpp"

namespace AST
{

    //! @brief Signature of a function.
    class FunctionPrototype : public ASN
    {
    private:
        //! @brief Name of the function.
        std::string identifier;

        //! @brief Parameters of the function.
        std::vector<Vessel> parameters;

    public:
        FunctionPrototype(const std::string &p_identifier, const std::vector<Vessel> &p_parameters) noexcept;
        FunctionPrototype(std::string &&p_identifier, std::vector<Vessel> &&p_parameters) noexcept;
        ~FunctionPrototype() override;

        const std::string &view_identifier() const;
        const std::vector<Vessel> &view_parameters() const;
    };

} // namespace AST

#endif // AST_FUNCTION_PROTOTYPE_HPP