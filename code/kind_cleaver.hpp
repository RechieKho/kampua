#ifndef KIND_CLEAVER_HPP
#define KIND_CLEAVER_HPP

#include <cstddef>
#include <optional>
#include <sstream>
#include <vector>

#include "cleaver.hpp"

template <typename T, typename R>
class KindCleaver : public Cleaver<T, R> {
 public:
  using base_type = Cleaver<T, R>;
  using container_type = base_type::container_type;
  using value_type = base_type::value_type;
  using attribute_type = base_type::attribute_type;
  using chunk_type = base_type::chunk_type;

  template <typename U>
  using list_type = std::vector<U>;
  template <typename U>
  using reference_type = std::reference_wrapper<U>;
  template <typename U>
  using optional_type = std::optional<U>;
  using unifier_nesting_count_type = std::size_t;

  class Kind {
   public:
    using count_type = std::size_t;

   private:
    container_type characters;
    count_type max_count;
    cleaver_option_underlying_type option;

   public:
    inline Kind(container_type p_characters, count_type p_max_count,
                cleaver_option_underlying_type p_option =
                    CleaverOption::CLEAVE | CleaverOption::RECORD) noexcept
        : characters(std::move(p_characters)),
          max_count(p_max_count),
          option(p_option) {}

    inline auto operator<=>(const Kind &p_kind) const & noexcept = default;

    inline auto get_max_count() const & noexcept { return max_count; }

    inline auto get_option() const & noexcept { return option; }

    inline const auto &view_characters() const & noexcept { return characters; }
  };

  class Unifier {
   private:
    value_type start_unify_mark;
    value_type end_unify_mark;
    cleaver_option_underlying_type option;

   public:
    inline Unifier(value_type p_start_unify_mark, value_type p_end_unify_mark,
                   cleaver_option_underlying_type p_option =
                       CleaverOption::CLEAVE | CleaverOption::RECORD) noexcept
        : start_unify_mark(p_start_unify_mark),
          end_unify_mark(p_end_unify_mark),
          option(p_option) {}

    inline auto operator<=>(const Unifier &p_unifier) const & noexcept =
        default;

    inline auto is_start_unify_mark(value_type p_character) const & noexcept {
      return start_unify_mark == p_character;
    }

    inline auto is_end_unify_mark(value_type p_character) const & noexcept {
      return end_unify_mark == p_character;
    }

    inline auto get_start_unify_mark() const & noexcept {
      return start_unify_mark;
    }

    inline auto get_end_unify_mark() const & noexcept { return end_unify_mark; }

    inline auto get_option() const & noexcept { return option; }
  };

 private:
  list_type<Kind> kinds;
  list_type<Unifier> unifiers;
  optional_type<reference_type<const Kind>> current_kind;
  optional_type<reference_type<const Unifier>> current_unifier;
  unifier_nesting_count_type unifier_nesting_count;

 protected:
  virtual attribute_type make_result(cleaver_option_underlying_type p_option,
                                     chunk_type p_chunk) = 0;

  attribute_type process(const value_type &p_value,
                         chunk_type p_chunk) override {
    // Check unifying.
    if (current_unifier.has_value()) {
      const auto &unifier = current_unifier->get();
      // Check stop unifying.
      if (unifier.is_end_unify_mark(p_value)) {
        // Decrease nesting count.
        unifier_nesting_count--;
        // Check not nested.
        if (unifier_nesting_count == 0) {
          // Stop unifying.
          current_unifier = std::nullopt;
          return make_result(unifier.get_option() | CleaverOption::SKIP,
                             p_chunk);
        }
      }
      // Check nesting.
      else if (unifier.is_start_unify_mark(p_value)) {
        unifier_nesting_count++;
      } else
        // Grow.
        return make_result(CleaverOption::IGNORE, p_chunk);
    }

    // Check start unifying.
    for (const auto &unifier : unifiers)
      if (unifier.is_start_unify_mark(p_value)) {
        const auto option = current_kind.has_value()
                                ? current_kind->get().get_option()
                                : CleaverOption::CLEAVE | CleaverOption::RECORD;
        current_unifier = std::as_const(unifier);
        unifier_nesting_count++;
        return make_result(option | CleaverOption::SKIP, p_chunk);
      }

    // Determine current Kind.
    auto current_kind = optional_type<reference_type<const Kind>>(std::nullopt);
    for (const auto &Kind : kinds) {
      const auto &characters = Kind.view_characters();
      if (characters.find(p_value) != characters.npos) {
        // Found current Kind.
        current_kind = std::as_const(Kind);
        break;
      }
    }

    // Determine previous Kind
    auto &previous_kind = this->current_kind;
    auto is_equal = (current_kind.has_value() == previous_kind.has_value()) &&
                    (current_kind.has_value() && previous_kind.has_value()
                         ? current_kind->get() == previous_kind->get()
                         : true);

    // Check Kind equal.
    if (is_equal) {
      // Check max count.
      if (current_kind.has_value() &&
          p_chunk.size() >= current_kind->get().get_max_count())
        return make_result(CleaverOption::CLEAVE | CleaverOption::RECORD,
                           p_chunk);

      // Grow.
      return make_result(CleaverOption::IGNORE, p_chunk);
    } else {
      // Switch to new Kind.
      const auto option = previous_kind.has_value()
                              ? previous_kind->get().get_option()
                              : CleaverOption::CLEAVE | CleaverOption::RECORD;
      this->current_kind = std::move(current_kind);
      return make_result(option, p_chunk);
    }
  }

  attribute_type terminate(chunk_type p_chunk) override {
    if (current_unifier.has_value()) {
      std::stringstream message;
      message << "Unterminated unifier `"
              << current_unifier->get().get_end_unify_mark() << "`. ";
      // Raise error.
      throw std::runtime_error(message.str());
    }

    return make_result(CleaverOption::IGNORE, p_chunk);
  }

 public:
  inline KindCleaver(list_type<Kind> p_kinds, list_type<Unifier> p_unifiers)
      : kinds(std::move(p_kinds)),
        unifiers(std::move(p_unifiers)),
        current_kind(std::nullopt),
        current_unifier(std::nullopt),
        unifier_nesting_count(0) {}
};

#endif  // KIND_CLEAVER_HPP