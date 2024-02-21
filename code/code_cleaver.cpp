#include "code_cleaver.hpp"

std::vector<CodeCleaver::Unifier> CodeCleaver::make_default_unifiers() {
  return std::vector<Unifier>{
      Unifier('\'', '\'', CodeTag::STRING_LITERAL),
      Unifier('\"', '\"', CodeTag::STRING_LITERAL),
      Unifier('#', '#', CodeTag::COMMENT,
              CleaverOption::CLEAVE | CleaverOption::SKIP)};
}

std::vector<CodeCleaver::Kind> CodeCleaver::make_default_kinds() {
  return std::vector<Kind>{
      Kind(container_type(" \t\n\v\f\r"), -1, CodeTag::WORD,
           CleaverOption::CLEAVE),
      Kind(container_type(";"), 1, CodeTag::TERMINATE),
      Kind(container_type("+-*/%=!"), 2, CodeTag::GENERIC_OPERATOR),
      Kind(container_type(":"), 1, CodeTag::TYPE_OPERATOR),
      Kind(container_type("^"), 1, CodeTag::POINTER_OPERATOR),
      Kind(container_type("@"), 1, CodeTag::ADDRESS_OPERATOR),
      Kind(container_type("&"), 2, CodeTag::AND_OPERATOR),
      Kind(container_type("|"), 2, CodeTag::OR_OPERATOR),
      Kind(container_type("()"), 1, CodeTag::PARENTHESIS),
      Kind(container_type("[]"), 1, CodeTag::SQUARE_BRACKET),
      Kind(container_type("{}"), 1, CodeTag::CURLY_BRACE)};
}

CodeCleaver::result_type CodeCleaver::process(const value_type &p_value,
                                              chunk_size_type p_size) {
  // Check new line.
  if (p_value == NEWLINE) {
    row += 1;
    column = 0;
  }
  column += 1;

  // Check unifying.
  if (current_unifier.has_value()) {
    const auto &unifier = current_unifier->get();
    // Check stop unifying.
    if (unifier.is_end_unify_mark(p_value)) {
      // Stop unifying.
      current_unifier = std::nullopt;
      return CodeCleaverResult(unifier.get_option() | CleaverOption::SKIP, row,
                               column - p_size, unifier.get_tag());
    } else
      // Grow.
      return CodeCleaverResult(CleaverOption::IGNORE, row, column - p_size,
                               CodeTag::WORD);
  }

  // Check start unifying.
  for (const auto &unifier : unifiers)
    if (unifier.is_start_unify_mark(p_value)) {
      const auto tag = current_kind.has_value() ? current_kind->get().get_tag()
                                                : CodeTag::WORD;
      const auto option = current_kind.has_value()
                              ? current_kind->get().get_option()
                              : CleaverOption::CLEAVE | CleaverOption::RECORD;
      current_unifier = std::as_const(unifier);
      return CodeCleaverResult(option | CleaverOption::SKIP, row,
                               column - p_size, tag);
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
    if (current_kind.has_value())
      if (p_size >= current_kind->get().get_max_count()) {
        const auto tag = previous_kind.has_value()
                             ? previous_kind->get().get_tag()
                             : CodeTag::WORD;
        return CodeCleaverResult(CleaverOption::CLEAVE | CleaverOption::RECORD,
                                 row, column - p_size, tag);
      }

    // Grow.
    return CodeCleaverResult(CleaverOption::IGNORE, row, column - p_size,
                             CodeTag::WORD);
  } else {
    // Switch to new Kind.
    const auto tag = previous_kind.has_value() ? previous_kind->get().get_tag()
                                               : CodeTag::WORD;
    const auto option = previous_kind.has_value()
                            ? previous_kind->get().get_option()
                            : CleaverOption::CLEAVE | CleaverOption::RECORD;
    this->current_kind = std::move(current_kind);
    return CodeCleaverResult(option, row, column - p_size, tag);
  }
}

CodeCleaver::result_type CodeCleaver::terminate(chunk_size_type p_size) {
  if (current_unifier.has_value()) {
    std::stringstream message;
    message << "Unterminated character `"
            << current_unifier->get().get_end_unify_mark() << "`. "
            << make_position_description(row, column);
    // Raise error.
    throw std::runtime_error(message.str());
  }

  const auto option = CleaverOption::IGNORE;
  const auto current_row = row;
  const auto current_column = column - p_size;
  const auto tag =
      current_kind.has_value() ? current_kind->get().get_tag() : CodeTag::WORD;

  // Clear state.
  row = 1;
  column = 0;
  current_kind = std::nullopt;
  current_unifier = std::nullopt;

  return CodeCleaverResult(
      CleaverOption::IGNORE, row, column - p_size,
      current_kind.has_value() ? current_kind->get().get_tag() : CodeTag::WORD);
}
