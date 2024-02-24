#include "vessel.hpp"

#include <sstream>

Vessel Vessel::from(const slice_type<entry_type> &p_entries) try {
  // Check entries availability.
  if (p_entries.size() == 0)
    throw std::invalid_argument("Unable to build Vessel from empty entries.");

  // Retrieve Identifier.
  const auto &identifier_entry = p_entries[0];
  auto identifier = Identifier::from(identifier_entry);

  // Check type operator availability.
  if (p_entries.size() == 1) {
    const auto &attribute = std::get<attribute_type>(identifier_entry);
    throw std::runtime_error(std::string() + "Expecting a type operator (`" +
                             std::string(TYPE_OPERATOR) + "`). " +
                             attribute.describe());
  }

  // Check type operator.
  const auto &type_operator_entry = p_entries[1];
  const auto &type_operator_chunk = std::get<chunk_type>(type_operator_entry);
  const auto &type_operator_attribute =
      std::get<attribute_type>(type_operator_entry);
  const auto type_operator_chunk_string_slice =
      string_slice_type(type_operator_chunk.begin(), type_operator_chunk.end());
  if (type_operator_chunk_string_slice != TYPE_OPERATOR) {
    throw std::runtime_error(std::string() + "Expecting a type operator (`" +
                             std::string(TYPE_OPERATOR) + "`) but `" +
                             std::string(type_operator_chunk_string_slice) +
                             "` is given. " +
                             type_operator_attribute.describe());
  }

  // Check type availability.
  if (p_entries.size() <= 2) {
    throw std::runtime_error(std::string() + "Expecting a type. " +
                             type_operator_attribute.describe());
  }

  // Retrieve sort.
  auto sort = Sort::from(p_entries.subspan(3));

  return Vessel(std::move(identifier), std::move(sort));
} catch (...) {
  throw;
}