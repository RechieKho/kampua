#include "code_cleaver.hpp"

const CodeCleaver::value_type CodeCleaver::NEWLINE =
    CodeCleaver::value_type('\n');

const std::vector<CodeCleaver::Unifier> CodeCleaver::DEFAULT_UNIFIERS =
    std::vector<Unifier>{
        Unifier('\'', '\''), Unifier('\"', '\"'),
        Unifier('#', '#', CleaverOption::CLEAVE | CleaverOption::SKIP)};

const std::vector<CodeCleaver::Kind> CodeCleaver::DEFAULT_KINDS =
    std::vector<Kind>{
        Kind(container_type(" \t\n\v\f\r"), -1, CleaverOption::CLEAVE),
        Kind(container_type(";"), 1),
        Kind(container_type("+-*/%=!"), 2),
        Kind(container_type(":"), 1),
        Kind(container_type("^"), 1),
        Kind(container_type("@"), 1),
        Kind(container_type("&"), 2),
        Kind(container_type("|"), 2),
        Kind(container_type("()"), 1),
        Kind(container_type("[]"), 1),
        Kind(container_type("{}"), 1)};

CodeCleaver::attribute_type CodeCleaver::make_result(
    cleaver_option_underlying_type p_option, chunk_type p_chunk) {
  return attribute_type(
      p_option, CodePosition(position.row, position.column - p_chunk.size()));
}

CodeCleaver::attribute_type CodeCleaver::process(const value_type& p_value,
                                                 chunk_type p_chunk) {
  const auto result = base_type::process(p_value, p_chunk);
  position.column += 1;
  if (p_value == NEWLINE) {
    position.row += 1;
    position.column = 1;
  }
  return result;
}

CodeCleaver::attribute_type CodeCleaver::terminate(chunk_type p_chunk) {
  const auto result = base_type::terminate(p_chunk);
  position = CodePosition();
  return result;
}