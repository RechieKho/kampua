#ifndef AST_VESSEL_HPP
#define AST_VESSEL_HPP

/**
 * @file
 *
 * @ingroup AST
 *
 * @brief A value holder of given type.
 */

#include <string>
#include <vector>
#include <span>

namespace AST
{

    //! @brief A value holder of given type.
    //! Such as variable, parameter and etc.
    class Vessel
    {
    private:
        //! @brief Name of the value holder.
        std::string identifier;

        //! @brief Type of the value it holds.
        std::string type;

        //! @brief Mutability of the holder and its pointed value (if it is a pointer).
        std::vector<bool> mutability;

    public:
        Vessel(const std::string &p_identifier, const std::string &p_type, const std::vector<bool> &p_mutability) noexcept;
        Vessel(std::string &&p_identifier, std::string &&p_type, std::vector<bool> &&p_mutability) noexcept;
        ~Vessel() = default;

        std::size_t get_pointer_degree() const;
        const std::string &view_identifier() const;
        const std::string &view_type() const;
        const std::vector<bool> &view_mutability() const;
    };

} // namespace AST

#endif // AST_VESSEL_HPP