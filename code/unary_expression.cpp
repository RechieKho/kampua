#include "unary_expression.hpp"

UnaryExpression::~UnaryExpression() {}

UnaryExpression::string_type UnaryExpression::as_string() const& noexcept {
  return handler + " " + argument->as_string();
}

bool UnaryExpression::could_be_operator() const& noexcept { return false; }