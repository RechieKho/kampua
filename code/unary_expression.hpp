#ifndef UNARY_EXPRESSION_HPP
#define UNARY_EXPRESSION_HPP

#include <memory>

#include "code_cleaver.hpp"
#include "expression.hpp"

class UnaryExpression : public Expression {
 public:
  using handler_type = CodeCleaver::value_type;
  using argument_type = std::unique_ptr<Expression>;

 private:
  handler_type handler;
  argument_type argument;

 public:
  inline UnaryExpression(handler_type p_handler,
                         argument_type p_argument) noexcept
      : handler(p_handler), argument(std::move(p_argument)) {}

  inline bool operator==(const UnaryExpression& p_expression) const& noexcept =
      default;

  inline auto get_handler() const& noexcept { return handler; }

  inline const auto& view_argument() const& noexcept { return argument; }
}

#endif  // UNARY_EXPRESSION_HPP