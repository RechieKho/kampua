#ifndef FLOAT_EXPRESSION_HPP
#define FLOAT_EXPRESSION_HPP

#include <span>
#include <string>

#include "code_cleaver.hpp"
#include "expression.hpp"

class FloatExpression : public Expression {
 public:
  using char_type = CodeCleaver::value_type;
  using chunk_type = CodeCleaver::chunk_type;
  using attribute_type = CodeCleaver::attribute_type;
  using entry_type = CodeCleaver::entry_type;

  using value_type = double;
  using string_slice_type = std::basic_string_view<char_type>;

 private:
  value_type value;

 public:
  static FloatExpression from(const entry_type& p_entry);

  inline FloatExpression(value_type p_value) noexcept : value(p_value) {}

  inline auto operator<=>(value_type p_value) const& noexcept = default;

  inline auto get() const& noexcept { return value; }

  inline operator value_type() const& noexcept { return value; }
};

#endif  // FLOAT_EXPRESSION_HPP