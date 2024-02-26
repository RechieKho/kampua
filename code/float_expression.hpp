#ifndef FLOAT_EXPRESSION_HPP
#define FLOAT_EXPRESSION_HPP

#include <span>
#include <string>

#include "code_cleaver.hpp"
#include "expression.hpp"

class FloatExpression : public Expression {
 public:
  using value_type = double;

 private:
  value_type value;

 public:
  static FloatExpression from(const entry_type& p_entry);

  inline FloatExpression(value_type p_value) noexcept : value(p_value) {}

  ~FloatExpression() override;

  string_type as_string() const& noexcept override;

  bool could_be_operator() const& noexcept override;

  inline auto get() const& noexcept { return value; }

  inline operator value_type() const& noexcept { return value; }
};

#endif  // FLOAT_EXPRESSION_HPP