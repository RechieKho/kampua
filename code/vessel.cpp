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
    std::stringstream message;
    const auto &result = std::get<result_type>(identifier_entry);
    message << "Expecting a type operator (`:`). " << std::to_string(result);
    throw std::runtime_error(message.str());
  }

  // Check type operator.
  const auto &type_entry = p_entries[1];
  const auto &type_chunk = std::get<chunk_type>(type_entry);
  const auto &type_result = std::get<result_type>(type_entry);
  const auto type_chunk_string_view =
      std::string_view(type_chunk.begin(), type_chunk.end());
  if (type_chunk_string_view != ":") {
    std::stringstream message;
    message << "Expecting a type operator (`:`) but `"
            << string_slice_type(type_chunk.begin(), type_chunk.end())
            << "` is given. " << std::to_string(type_result);
    throw std::runtime_error(message.str());
  }

  // Check type availability.
  if (p_entries.size() <= 2) {
    std::stringstream message;
    message << "Expecting a type. " << std::to_string(type_result);
    throw std::runtime_error(message.str());
  }

  // Retrieve type.
  auto sort = Sort::from(p_entries.subspan(3));

  return Vessel(std::move(identifier), std::move(sort));
} catch (...) {
  throw;
}