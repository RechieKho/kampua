#ifndef UNARY_EXPRESSION_HPP
#define UNARY_EXPRESSION_HPP

#include <memory>
#include <string>

#include "code_cleaver.hpp"
#include "expression.hpp"

class UnaryExpression : public Expression {
 public:
 private:
  operator_type handler;
  object_type argument;

 public:
  inline UnaryExpression(operator_type p_handler,
                         object_type p_argument) noexcept
      : handler(p_handler), argument(std::move(p_argument)) {}

  ~UnaryExpression() override;

  string_type as_string() const& noexcept override;

  bool could_be_operator() const& noexcept override;

  inline const auto& view_handler() const& noexcept { return handler; }

  inline const auto& view_argument() const& noexcept { return argument; }
};

#endif  // UNARY_EXPRESSION_HPP