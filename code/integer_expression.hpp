#ifndef INTEGER_EXPRESSION_HPP
#define INTEGER_EXPRESSION_HPP

#include <cstdint>
#include <string>

#include "expression.hpp"

class IntegerExpression : public Expression {
 public:
  using char_type = CodeCleaver::value_type;
  using chunk_type = CodeCleaver::chunk_type;
  using attribute_type = CodeCleaver::attribute_type;
  using entry_type = CodeCleaver::entry_type;

  using value_type = uint64_t;
  using string_type = std::basic_string<char_type>;
  using string_slice_type = std::basic_string_view<char_type>;

 private:
  value_type value;

 public:
  static IntegerExpression from(const entry_type& p_entry);

  inline IntegerExpression(value_type p_value) noexcept : value(p_value) {}

  ~IntegerExpression() override;

  string_type as_string() const& noexcept override;

  bool could_be_operator() const& noexcept override;

  inline auto get() const& noexcept { return value; }

  inline operator value_type() const& noexcept { return value; }
};

#endif  // INTEGER_EXPRESSION_HPP