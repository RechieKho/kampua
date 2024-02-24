#ifndef VAGUE_CLEAVER_HPP
#define VAGUE_CLEAVER_HPP

#include "cleaver.hpp"

template <Cleavable T>
class VagueCleaver : public Cleaver<T, cleaver_option_underlying_type> {
 public:
 private:
  T value;
  cleaver_option_underlying_type option;

  attribute_type process(const value_type &p_value,
                         chunk_type p_chunk) override {
    if (p_value == value)
      return option;
    else
      return CleaverOption::IGNORE;
  }

  attribute_type terminate(chunk_type p_chunk) override {
    return CleaverOption::IGNORE;
  }

 public:
  inline VagueCleaver(value_type &&p_value,
                      cleaver_option_underlying_type p_option =
                          CleaverOption::CLEAVE || CleaverOption::SKIP)
      : value(std::forward<value_type>(p_value)), option(p_option) noexcept {}
};

#endif  // VAGUE_CLEAVER_HPP