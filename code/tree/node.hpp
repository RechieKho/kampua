#ifndef AST_ASN_HPP
#define AST_ASN_HPP

/**
 * @file
 *
 * @ingroup Tree
 *
 * @brief Base class for node in abstract syntax tree.
 */

#include "../concepts.hpp"

namespace Tree
{

    //! @brief Base class for node in abstract syntax tree.
    //! @tparam T Character type.
    template <typename T>
        requires Character<T>
    class Node
    {
    public:
        virtual ~Node() = 0;
    };

    template <typename T>
        requires Character<T>
    Node<T>::~Node()
    {
    }

} // namespace Tree

#endif // AST_ASN_HPP