#ifndef EXPRESSION_HPP
#define EXPRESSION_HPP

#include <cstdint>
#include <map>
#include <memory>
#include <span>

#include "code_cleaver.hpp"

class Expression {
 public:
  using chunk_type = CodeCleaver::chunk_type;
  using attribute_type = CodeCleaver::attribute_type;
  using entry_type = CodeCleaver::entry_type;
  using char_type = CodeCleaver::value_type;

  template <typename T>
  using slice_type = std::span<T>;
  template <typename T>
  using list_type = std::vector<T>;
  template <typename K, typename V>
  using map_type = std::unordered_map<K, V>;
  using string_type = std::basic_string<char_type>;
  using string_slice_type = std::basic_string_view<char_type>;
  using object_type = std::unique_ptr<Expression>;
  using operator_type = string_type;

 private:
 public:
  virtual string_type as_string() const& noexcept = 0;

  virtual bool could_be_operator() const& noexcept = 0;

  virtual ~Expression() = 0;
};

#endif  // EXPRESSION_HPP