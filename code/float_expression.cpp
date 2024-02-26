#include "float_expression.hpp"

#include <string>

FloatExpression FloatExpression::from(const entry_type& p_entry) {
  const auto& chunk = std::get<chunk_type>(p_entry);
  auto chunk_string = string_type(chunk.begin(), chunk.end());
  try {
    return FloatExpression(std::stod(chunk_string));
  } catch (...) {
    const auto& attribute = std::get<attribute_type>(p_entry);
    throw std::runtime_error(string_type() + "Unable to convert '" +
                             chunk_string + "' to float expression. " +
                             attribute.describe());
  }
}

FloatExpression::~FloatExpression() {}

FloatExpression::string_type FloatExpression::as_string() const& noexcept {
  return std::to_string(value);
}

bool FloatExpression::could_be_operator() const& noexcept { return false; }