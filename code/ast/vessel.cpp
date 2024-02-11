#include "vessel.hpp"

#include <memory>

using namespace AST;

Vessel::Vessel(const std::string &p_identifier, const std::string &p_type, const std::vector<bool> &p_mutability) noexcept
    : identifier(p_identifier), type(p_type), mutability(p_mutability) {}

Vessel::Vessel(std::string &&p_identifier, std::string &&p_type, std::vector<bool> &&p_mutability) noexcept
    : identifier(std::move(p_identifier)), type(std::move(p_type)), mutability(std::move(p_mutability)) {}

std::size_t Vessel::get_pointer_degree() const
{
    return mutability.size() - 1;
}

const std::string &Vessel::view_identifier() const
{
    return identifier;
}

const std::string &Vessel::view_type() const
{
    return type;
}

const std::vector<bool> &Vessel::view_mutability() const
{
    return mutability;
}
