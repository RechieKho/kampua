#include "float_expression.hpp"

#include <string>

FloatExpression FloatExpression::from(const entry_type& p_entry) {
  const auto& chunk = std::get<chunk_type>(p_entry);
  auto chunk_string_slice = string_slice_type(chunk.begin(), chunk.end());
  try {
    return FloatExpression(std::stod(chunk_string_slice));
  } catch (...) {
    const auto& attribute = std::get<attribute_type>(p_entry);
    throw std::runtime_error(std::string() + "Unable to convert '" +
                             chunk_string_slice + "' to float expression. " +
                             attribute.describe());
  }
}