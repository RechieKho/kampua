#ifndef CODE_CLEAVER_HPP
#define CODE_CLEAVER_HPP

#include <cstddef>
#include <cstdint>
#include <format>
#include <iostream>
#include <optional>
#include <sstream>
#include <string>
#include <vector>

#include "cleaver.hpp"

using position_type = std::size_t;
using code_tag_underlying_type = uint8_t;

enum CodeTag : code_tag_underlying_type {
  WORD = 0,
  TERMINATE,
  GENERIC_OPERATOR,
  TYPE_OPERATOR,
  POINTER_OPERATOR,
  ADDRESS_OPERATOR,
  AND_OPERATOR,
  OR_OPERATOR,
  PARENTHESIS,
  SQUARE_BRACKET,
  CURLY_BRACE,
  STRING_LITERAL,
  COMMENT
};

inline auto make_position_description(position_type p_row,
                                      position_type p_column) {
  return std::string("") + "[row: " + std::to_string(p_row) +
         ", column: " + std::to_string(p_column) + "]";
}

class CodeCleaverResult {
 public:
 private:
  cleaver_option_underlying_type option;
  position_type row;
  position_type column;
  CodeTag tag;

 public:
  inline CodeCleaverResult(cleaver_option_underlying_type p_option,
                           position_type p_row, position_type p_column,
                           CodeTag p_tag) noexcept
      : option(p_option), row(p_row), column(p_column), tag(p_tag) {}

  inline auto operator<=>(const CodeCleaverResult &p_result) const & noexcept =
      default;

  inline auto get_row() const & noexcept { return row; }

  inline auto get_column() const & noexcept { return column; }

  inline auto get_tag() const & noexcept { return tag; }

  inline operator cleaver_option_underlying_type() const & noexcept {
    return option;
  }

  inline operator std::string() const & noexcept {
    return make_position_description(row, column);
  }
};

class CodeCleaver : public Cleaver<std::string, CodeCleaverResult> {
 public:
  template <typename T>
  using list_type = std::vector<T>;
  template <typename T>
  using reference_type = std::reference_wrapper<T>;
  template <typename T>
  using optional_type = std::optional<T>;

  class Kind {
   public:
    using count_type = std::size_t;

   private:
    container_type characters;
    count_type max_count;
    CodeTag tag;
    cleaver_option_underlying_type option;

   public:
    inline Kind(container_type p_characters, count_type p_max_count,
                CodeTag p_tag,
                cleaver_option_underlying_type p_option =
                    CleaverOption::CLEAVE | CleaverOption::RECORD) noexcept
        : characters(std::move(p_characters)),
          max_count(p_max_count),
          tag(p_tag),
          option(p_option) {}

    inline auto operator<=>(const Kind &p_kind) const & noexcept = default;

    inline auto get_max_count() const & noexcept { return max_count; }

    inline auto get_tag() const & noexcept { return tag; }

    inline auto get_option() const & noexcept { return option; }

    inline const auto &view_characters() const & noexcept { return characters; }
  };

  class Unifier {
   private:
    value_type start_unify_mark;
    value_type end_unify_mark;
    CodeTag tag;
    cleaver_option_underlying_type option;

   public:
    inline Unifier(value_type p_start_unify_mark, value_type p_end_unify_mark,
                   CodeTag p_tag,
                   cleaver_option_underlying_type p_option =
                       CleaverOption::CLEAVE | CleaverOption::RECORD) noexcept
        : start_unify_mark(p_start_unify_mark),
          end_unify_mark(p_end_unify_mark),
          tag(p_tag),
          option(p_option) {}

    inline auto operator<=>(const Unifier &p_unifier) const & noexcept =
        default;

    inline auto is_start_unify_mark(value_type p_character) const & noexcept {
      return start_unify_mark == p_character;
    }

    inline auto is_end_unify_mark(value_type p_character) const & noexcept {
      return end_unify_mark == p_character;
    }

    inline auto get_start_unify_mark() const & noexcept {
      return start_unify_mark;
    }

    inline auto get_end_unify_mark() const & noexcept { return end_unify_mark; }

    inline auto get_tag() const & noexcept { return tag; }

    inline auto get_option() const & noexcept { return option; }
  };

 private:
  position_type row;
  position_type column;
  list_type<Kind> kinds;
  list_type<Unifier> unifiers;
  optional_type<reference_type<const Kind>> current_kind;
  optional_type<reference_type<const Unifier>> current_unifier;

  static constexpr const auto NEWLINE = value_type('\n');

  static list_type<Unifier> make_default_unifiers();

  static list_type<Kind> make_default_kinds();

  result_type process(const value_type &p_value,
                      chunk_size_type p_size) override;

  result_type terminate(chunk_size_type p_size) override;

 public:
  inline CodeCleaver()
      : row(1),
        column(0),
        kinds(make_default_kinds()),
        unifiers(make_default_unifiers()) {}

  inline CodeCleaver(list_type<Kind> p_kinds, list_type<Unifier> p_unifiers)
      : row(1),
        column(0),
        kinds(std::move(p_kinds)),
        unifiers(std::move(p_unifiers)) {}
};

#endif  // CODE_CLEAVER_HPP