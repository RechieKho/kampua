#ifndef IDENTIFIER_EXPRESSION_HPP
#define IDENTIFIER_EXPRESSION_HPP

#include <memory>

#include "expression.hpp"
#include "identifier.hpp"

class IdentifierExpression : public Expression {
 public:
  using value_type = Identifier;

 private:
  value_type value;

 public:
  inline static IdentifierExpression from(const entry_type& p_entry) {
    return IdentifierExpression(value_type::from(p_entry));
  }

  inline IdentifierExpression(value_type p_value) noexcept
      : value(std::move(p_value)) {}

  inline auto operator<=>(
      const IdentifierExpression& p_expression) const& noexcept = default;

  inline const auto& view() const& noexcept { return value; }
};

#endif  // IDENTIFIER_EXPRESSION_HPP