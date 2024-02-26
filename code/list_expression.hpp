#ifndef LIST_EXPRESSION_HPP
#define LIST_EXPRESSION_HPP

#include <memory>
#include <vector>

#include "expression.hpp"

class ListExpression : public Expression {
 public:
  template <typename T>
  using list_type = std::vector<T>;

 private:
  list_type<object_type> list;

 public:
  inline ListExpression(list_type<object_type> p_list)
      : list(std::move(p_list)) {}

  ~ListExpression() override;

  string_type as_string() const& noexcept override;

  bool could_be_operator() const& noexcept override;

  inline const auto& view() const& noexcept { return list; }
};

#endif  // LIST_EXPRESSION_HPP