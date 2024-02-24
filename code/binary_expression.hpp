#ifndef BINARY_EXPRESSION_HPP
#define BINARY_EXPRESSION_HPP

#include <memory>

#include "code_cleaver.hpp"
#include "expression.hpp"

class BinaryExpression : public Expression {
 public:
  using handler_type = CodeCleaver::value_type;
  using argument_type = std::unique_ptr<Expression>;

 private:
  handler_type handler;
  argument_type left_argument;
  argument_type right_argument;

 public:
  inline BinaryExpression(handler_type p_handler, argument_type p_left_argument,
                          argument_type p_right_argument) noexcept
      : handler(p_handler),
        left_argument(std::move(p_left_argument)),
        right_argument(std::move(p_right_argument)) {}

  inline bool operator==(const BinaryExpression& p_expression) const& noexcept =
      default;

  inline auto get_handler() const& noexcept { return handler; }

  inline const auto& view_left_argument() const& noexcept {
    return left_argument;
  }

  inline const auto& view_right_argument() const& noexcept {
    return right_argument;
  }
}

#endif  // BINARY_EXPRESSION_HPP