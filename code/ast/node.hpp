#ifndef AST_ASN_HPP
#define AST_ASN_HPP

/**
 * @file
 *
 * @ingroup AST
 *
 * @brief Base class for node in abstract syntax tree.
 */

namespace AST
{

    //! @brief Base class for node in abstract syntax tree.
    class Node
    {
    public:
        virtual ~Node() = 0;
    };

} // namespace AST

#endif // AST_ASN_HPP