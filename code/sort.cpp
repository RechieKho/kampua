#include "sort.hpp"

#include <memory>

Sort Sort::from(const slice_type<entry_type> &p_entries) {
  // Check tokens availability.
  if (p_entries.size() == 0)
    throw std::invalid_argument("Unable to build Vessel from empty tokens.");

  const auto &last = p_entries.back();
  const auto rest =
      slice_type<entry_type>(p_entries.begin(), std::prev(p_entries.end()));

  // Retrieve mutabilities.
  auto mutabilities = list_type<bool>();
  auto mutability = false;
  for (const auto &token : rest) {
    const auto &view = std::get<chunk_type>(token);
    const auto &result = std::get<result_type>(token);
    const auto string_view = string_slice_type(view.begin(), view.end());

    if (result.get_tag() == CodeTag::POINTER_OPERATOR) {
      mutabilities.push_back(mutability);
      mutability = DEFAULT_MUTABILITY;
    } else if (string_view == MUTABILITY_KEYWORD) {
      mutability = !DEFAULT_MUTABILITY;
    } else {
      std::stringstream message;
      message << "Unexpected keyword `" << string_view
              << "` during type definition. " << result;
      throw std::runtime_error(message.str());
    }
  }
  mutabilities.push_back(mutability);
  std::reverse(mutabilities.begin(), mutabilities.end());

  // Retrieve type Identifier.
  auto identifier = Identifier::from(last);

  return Sort(std::move(identifier), std::move(mutabilities));
}