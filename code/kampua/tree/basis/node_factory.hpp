#ifndef KAMPUA_TREE_BASIS_NODE_FACTORY_HPP
#define KAMPUA_TREE_BASIS_NODE_FACTORY_HPP

/**
 * @file
 *
 * @ingroup Kampua::Tree::Basis
 *
 * @brief Base factory class to generate abstract syntax tree.
 */

#include "concepts.hpp"

namespace Kampua::Tree::Basis
{

    //! @brief Base factory class to generate abstract syntax tree.
    //! @tparam T Character type
    template <typename T>
        requires Character<T>
    class NodeFactory
    {
    public:
        virtual ~NodeFactory() = 0;
    };

    template <typename T>
        requires Character<T>
    NodeFactory<T>::~NodeFactory()
    {
    }

} // namespace Kampua::Tree::Basis

#endif // KAMPUA_TREE_BASIS_NODE_FACTORY_HPP