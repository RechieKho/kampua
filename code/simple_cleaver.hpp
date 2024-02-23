#ifndef SIMPLE_CLEAVER_HPP
#define SIMPLE_CLEAVER_HPP

#include "cleaver.hpp"

template <Cleavable T>
class SimpleCleaver : public Cleaver<T, CleaverOption> {
 public:
 private:
  T value;
  CleaverOption option;

  result_type process(const value_type &p_value, chunk_type p_chunk) override {
    if (p_value == value)
      return option;
    else
      return CleaverOption::IGNORE;
  }

  result_type terminate(chunk_type p_chunk) override {
    return CleaverOption::IGNORE;
  }

 public:
  inline SimpleCleaver(value_type &&p_value,
                       CleaverOption p_option = CleaverOption::DISCARD)
      : value(std::forward<value_type>(p_value)), option(p_option) noexcept {}
};

#endif  // SIMPLE_CLEAVER_HPP