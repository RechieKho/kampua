#ifndef AST_ASN_HPP
#define AST_ASN_HPP

/**
 * @file
 *
 * @ingroup Tree
 *
 * @brief Base class for node in abstract syntax tree.
 */

namespace Tree
{

    //! @brief Base class for node in abstract syntax tree.
    class Node
    {
    public:
        virtual ~Node() = 0;
    };

} // namespace Tree

#endif // AST_ASN_HPP