#include "identifier.hpp"

Identifier Identifier::from(const entry_type &p_entry) try {
  const auto &chunk = std::get<chunk_type>(p_entry);
  return Identifier(string_type(chunk.begin(), chunk.end()));
} catch (const std::exception &e) {
  auto message = std::stringstream();
  const auto &result = std::get<attribute_type>(p_entry);
  message << e.what() << " " << result;
  throw std::runtime_error(message.str());
}

bool Identifier::is_valid_identifier(const string_type &p_label) noexcept {
  // Empty label is not a valid identifier.
  if (p_label.size() == 0) return false;

  // First character should only be alphebet or underscore.
  const auto &first = p_label[0];
  if (!isalpha(first) && first != char_type('_')) return false;

  // Rest should be alphanumeric or underscore.
  const auto rest =
      std::span<const char_type>(std::next(p_label.cbegin()), p_label.cend());
  for (const auto &character : rest)
    if (!isalnum(character) && character != char_type('_')) return false;

  return true;
}

Identifier::Identifier(string_type p_label) : label() {
  if (!is_valid_identifier(p_label)) {
    std::stringstream message;
    message << "`" << p_label << "` is not a valid Identifier.";
    // Raise error.
    throw std::runtime_error(message.str());
  }

  label = std::move(p_label);
}