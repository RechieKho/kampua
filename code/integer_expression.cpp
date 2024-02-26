#include "integer_expression.hpp"

#include <string>

IntegerExpression IntegerExpression::from(const entry_type& p_entry) {
  const auto& chunk = std::get<chunk_type>(p_entry);
  auto chunk_string = string_type(chunk.begin(), chunk.end());
  try {
    return IntegerExpression(std::stol(chunk_string));
  } catch (...) {
    const auto& attribute = std::get<attribute_type>(p_entry);
    throw std::runtime_error(string_type() + "Unable to convert '" +
                             chunk_string + "' to integer expression. " +
                             attribute.describe());
  }
}

IntegerExpression::~IntegerExpression() {}

IntegerExpression::string_type IntegerExpression::as_string() const& noexcept {
  return std::to_string(value);
}

bool IntegerExpression::could_be_operator() const& noexcept { return false; }