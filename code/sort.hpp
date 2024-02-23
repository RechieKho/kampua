#ifndef SORT_HPP
#define SORT_HPP

#include <memory>
#include <span>
#include <vector>

#include "code_cleaver.hpp"
#include "identifier.hpp"

class Sort {
 public:
  using string_type = CodeCleaver::container_type;
  using entry_type = CodeCleaver::entry_type;
  using result_type = CodeCleaver::result_type;
  using char_type = CodeCleaver::value_type;
  using chunk_type = CodeCleaver::chunk_type;
  template <typename T>
  using list_type = std::vector<T>;
  template <typename T>
  using slice_type = std::span<T>;
  using string_slice_type = std::basic_string_view<char_type>;

  static constexpr const auto DEFAULT_MUTABILITY = false;

  static constexpr const auto MUTABILITY_KEYWORD = string_slice_type("mut");

  static constexpr const auto POINTER_KEYWORD = string_slice_type("^");

 private:
  Identifier identifier;
  list_type<bool> mutabilities;

 public:
  static Sort from(const slice_type<entry_type> &p_entries);

  inline Sort(Identifier p_identifier, list_type<bool> p_mutabilities) noexcept
      : identifier(std::move(p_identifier)),
        mutabilities(std::move(p_mutabilities)) {}

  inline bool operator==(const Sort &p_type) const & noexcept = default;

  inline const auto &view_identifier() const & noexcept { return identifier; }

  inline const auto &view_mutabilities() const & noexcept {
    return mutabilities;
  }
};

#endif  // SORT_HPP