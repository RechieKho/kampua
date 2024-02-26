#ifndef BINARY_EXPRESSION_HPP
#define BINARY_EXPRESSION_HPP

#include <memory>
#include <string>

#include "code_cleaver.hpp"
#include "expression.hpp"

class BinaryExpression : public Expression {
 public:
 private:
  operator_type handler;
  object_type left_argument;
  object_type right_argument;

 public:
  inline BinaryExpression(operator_type p_handler, object_type p_left_argument,
                          object_type p_right_argument) noexcept
      : handler(p_handler),
        left_argument(std::move(p_left_argument)),
        right_argument(std::move(p_right_argument)) {}

  ~BinaryExpression() override;

  string_type as_string() const& noexcept override;

  bool could_be_operator() const& noexcept override;

  inline const auto& view_handler() const& noexcept { return handler; }

  inline const auto& view_left_argument() const& noexcept {
    return left_argument;
  }

  inline const auto& view_right_argument() const& noexcept {
    return right_argument;
  }
};

#endif  // BINARY_EXPRESSION_HPP