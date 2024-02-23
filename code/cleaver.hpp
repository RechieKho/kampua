#ifndef CLEAVER_HPP
#define CLEAVER_HPP

#include <cstddef>
#include <cstdint>
#include <span>
#include <tuple>
#include <vector>

#include "concepts.hpp"

using cleaver_option_underlying_type = uint8_t;

enum CleaverOption : cleaver_option_underlying_type {
  IGNORE = 0b00000000,
  RECORD = 0b00000001,
  INCLUSIVE = 0b00000010,
  SKIP = 0b000000100,
  CLEAVE = 0b10000000,
};

template <typename T>
concept CleaverResult = std::convertible_to<T, cleaver_option_underlying_type>;

template <typename T>
concept Cleavable = ContiguousIterable<T> && ValueTyped<T>;

template <Cleavable T, CleaverResult R>
class Cleaver {
 public:
  using container_type = T;
  using value_type = T::value_type;
  using result_type = R;
  using chunk_type = std::span<value_type>;
  using entry_type = std::tuple<chunk_type, result_type>;

 protected:
  virtual result_type process(const value_type &p_element,
                              chunk_type p_chunk) = 0;

  virtual result_type terminate(chunk_type p_chunk) = 0;

 public:
  virtual ~Cleaver() = 0;

  virtual std::vector<entry_type> operator()(T &p_container) final try {
    auto chunks = std::vector<entry_type>();
    auto start = p_container.begin();

    // Moving end of span.
    for (auto i = p_container.begin(); i != p_container.end(); i++) {
      auto current = *i;
      auto result = process(current, chunk_type(start, i));
      auto option = cleaver_option_underlying_type(result);

      // Record chunk.
      if (option & CleaverOption::CLEAVE) {
        if (option & CleaverOption::RECORD) {
          if (option & CleaverOption::INCLUSIVE)
            chunks.push_back(
                entry_type(chunk_type(start, i + 1), std::move(result)));
          else
            chunks.push_back(
                entry_type(chunk_type(start, i), std::move(result)));
        }
        if (option & CleaverOption::SKIP)
          start = i + 1;
        else
          start = i;
      }
    }

    // Record chunk.
    const auto end = p_container.end();
    if (start != end) {
      const auto chunk = chunk_type(start, end);
      chunks.push_back(entry_type(chunk, std::move(terminate(chunk))));
    }

    return chunks;
  } catch (...) {
    throw;
  }
};

template <Cleavable T, CleaverResult R>
Cleaver<T, R>::~Cleaver() {}

#endif  // CLEAVER_HPP