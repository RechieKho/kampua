#ifndef CALL_EXPRESSION_HPP
#define CALL_EXPRESSION_HPP

#include <memory>
#include <vector>

#include "code_cleaver.hpp"
#include "expression.hpp"
#include "identifier.hpp"

class CallExpression : public Expression {
 public:
  using argument_type = std::unique_ptr<Expression>;

  template <typename T>
  using list_type = std::vector<T>;

 private:
  Identifier caller;
  list_type<argument_type> arguments;

 public:
  inline CallExpression(Identifier p_caller,
                        list_type<argument_type> p_arguments) noexcept
      : caller(std::move(p_caller)), arguments(std::move(p_arguments)) {}

  inline bool operator==(const CallExpression& p_expression) const& noexcept =
      default;

  inline const auto& view_caller() const& noexcept { return caller; }

  inline const auto& view_arguments() const& noexcept { return arguments; }
}

#endif  // CALL_EXPRESSION_HPP