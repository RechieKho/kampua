#ifndef CODE_CLEAVER_HPP
#define CODE_CLEAVER_HPP

#include <cstddef>

#include "kind_cleaver.hpp"

class CodePosition {
 public:
  using position_type = std::size_t;

 private:
 public:
  position_type row;
  position_type column;

  inline CodePosition() : row(1), column(1) {}

  inline CodePosition(position_type p_row, position_type p_column)
      : row(p_row), column(p_column) {}

  inline bool operator==(const CodePosition& p_result) const& noexcept =
      default;

  inline auto describe() const& noexcept {
    return std::string() + "[row: " + std::to_string(row) +
           ", column: " + std::to_string(column) + "]";
  }
};

class CodeCleaverResult {
 public:
 private:
  cleaver_option_underlying_type option;
  CodePosition position;

 public:
  inline CodeCleaverResult(cleaver_option_underlying_type p_option,
                           CodePosition p_position)
      : option(p_option), position(std::move(p_position)) {}

  inline auto get_row() const& noexcept { return position.row; }

  inline auto get_column() const& noexcept { return position.column; }

  inline auto describe() const& noexcept { return position.describe(); }

  inline operator cleaver_option_underlying_type() const& noexcept {
    return option;
  }
};

class CodeCleaver : public KindCleaver<std::string, CodeCleaverResult> {
 public:
  using base_type = KindCleaver<std::string, CodeCleaverResult>;

 private:
  static const value_type NEWLINE;
  static const list_type<Unifier> DEFAULT_UNIFIERS;
  static const list_type<Kind> DEFAULT_KINDS;

  CodePosition position;

 protected:
  result_type make_result(cleaver_option_underlying_type p_option,
                          chunk_type p_chunk) override;

  result_type process(const value_type& p_value, chunk_type p_chunk) override;

  result_type terminate(chunk_type p_chunk) override;

 public:
  inline CodeCleaver(list_type<Kind> p_kinds = DEFAULT_KINDS,
                     list_type<Unifier> p_unifiers = DEFAULT_UNIFIERS)
      : position(), base_type(std::move(p_kinds), std::move(p_unifiers)) {}
};

#endif  // CODE_CLEAVER_HPP