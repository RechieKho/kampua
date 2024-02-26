#include "binary_expression.hpp"

BinaryExpression::~BinaryExpression() {}

BinaryExpression::string_type BinaryExpression::as_string() const& noexcept {
  return string_type() + left_argument->as_string() + " " + handler + " " +
         right_argument->as_string();
}

bool BinaryExpression::could_be_operator() const& noexcept { return false; }