#include "list_expression.hpp"

ListExpression::~ListExpression() {}

ListExpression::string_type ListExpression::as_string() const& noexcept {
  auto string = string_type();
  string += "[";
  for (const auto& element : list) string += element->as_string() + ", ";
  string += "]";
  return std::move(string);
}

bool ListExpression::could_be_operator() const& noexcept { return false; }