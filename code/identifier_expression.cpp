#include "identifier_expression.hpp"

IdentifierExpression::~IdentifierExpression() {}

IdentifierExpression::string_type IdentifierExpression::as_string()
    const& noexcept {
  return value.view();
}

bool IdentifierExpression::could_be_operator() const& noexcept { return true; }