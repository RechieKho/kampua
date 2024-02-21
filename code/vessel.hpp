#ifndef VESSEL_HPP
#define VESSEL_HPP

#include <memory>

#include "identifier.hpp"
#include "sort.hpp"

class Vessel {
 public:
  using string_type = CodeCleaver::container_type;
  using entry_type = CodeCleaver::entry_type;
  using result_type = CodeCleaver::result_type;
  using char_type = CodeCleaver::value_type;
  using chunk_type = CodeCleaver::chunk_type;

  template <typename T>
  using list_type = std::vector<T>;
  template <typename T>
  using slice_type = std::span<T>;
  using string_slice_type = std::basic_string_view<char_type>;

 private:
  Identifier identifier;
  Sort sort;

 public:
  static Vessel from(const slice_type<entry_type> &p_entries);

  inline Vessel(Identifier p_identifier, Sort p_sort) noexcept
      : identifier(std::move(p_identifier)), sort(std::move(p_sort)) {}

  inline bool operator==(const Vessel &p_vessel) const & noexcept = default;

  inline const auto &view_identifier() const & noexcept { return identifier; }

  inline const auto &view_sort() const & noexcept { return sort; }
};

#endif  // VESSEL_HPP