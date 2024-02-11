#include "function_prototype.hpp"

#include <memory>

using namespace AST;

FunctionPrototype::FunctionPrototype(const std::string &p_identifier, const std::vector<Vessel> &p_parameters) noexcept
    : identifier(p_identifier), parameters(p_parameters) {}

FunctionPrototype::FunctionPrototype(std::string &&p_identifier, std::vector<Vessel> &&p_parameters) noexcept
    : identifier(std::move(p_identifier)), parameters(std::move(p_parameters)) {}

FunctionPrototype::~FunctionPrototype() {}

const std::string &FunctionPrototype::view_identifier() const
{
    return identifier;
}

const std::vector<Vessel> &FunctionPrototype::view_parameters() const
{
    return parameters;
}