#include "function.hpp"

using namespace AST;

Function::Function(const FunctionPrototype &p_prototype, std::vector<std::unique_ptr<Node>> p_statements) noexcept
    : prototype(p_prototype), statements(std::move(p_statements)) {}

Function::Function(FunctionPrototype &&p_prototype, std::vector<std::unique_ptr<Node>> p_statements) noexcept
    : prototype(std::move(p_prototype)), statements(std::move(p_statements)) {}

Function::~Function() {}

const FunctionPrototype &Function::view_prototype() const
{
    return prototype;
}

const std::vector<std::unique_ptr<Node>> &Function::view_statements() const
{
    return statements;
}