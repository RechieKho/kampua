#ifndef TREE_BASIS_NODE_HPP
#define TREE_BASIS_NODE_HPP

/**
 * @file
 *
 * @ingroup Tree::Basis
 *
 * @brief Base class for node in abstract syntax tree.
 */

#include "../../concepts.hpp"

namespace Tree::Basis
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

} // namespace Tree::Basis

#endif // TREE_BASIS_NODE_HPP