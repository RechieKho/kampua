#ifndef KAMPUA_TREE_FACTORY_HPP
#define KAMPUA_TREE_FACTORY_HPP

/**
 * @file
 *
 * @ingroup Kampua::Tree
 *
 * @brief Base factory class to generate abstract syntax tree.
 */

#include "../../concepts.hpp"

namespace Kampua::Tree
{

    //! @brief Base factory class to generate abstract syntax tree.
    //! @tparam T Character type
    template <typename T>
        requires Character<T>
    class Factory
    {
    public:
        virtual ~Factory() = 0;
    };

    template <typename T>
        requires Character<T>
    Factory<T>::~Factory()
    {
    }

} // namespace Kampua

#endif // KAMPUA_TREE_FACTORY_HPP