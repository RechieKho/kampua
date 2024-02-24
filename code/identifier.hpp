#ifndef IDENTIFIER_HPP
#define IDENTIFIER_HPP

#include <memory>
#include <span>
#include <sstream>
#include <string>

#include "code_cleaver.hpp"
#include "concepts.hpp"

class Identifier {
 public:
  using string_type = CodeCleaver::container_type;
  using entry_type = CodeCleaver::entry_type;
  using attribute_type = CodeCleaver::attribute_type;
  using char_type = CodeCleaver::value_type;
  using chunk_type = CodeCleaver::chunk_type;

 private:
  string_type label;

 public:
  static Identifier from(const entry_type &p_entry);

  static bool is_valid_identifier(const string_type &p_label) noexcept;

  inline Identifier() = default;

  Identifier(string_type p_label);

  inline auto operator<=>(const Identifier &p_identifier) const & noexcept =
      default;

  inline const auto &view() const & noexcept { return label; }
};

#endif  // IDENTIFIER_HPP