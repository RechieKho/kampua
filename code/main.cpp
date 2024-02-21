#include <iostream>
#include <span>
#include <string>

#include "code_cleaver.hpp"
#include "vessel.hpp"

int main() {
  auto code = std::string("vessel # This is a comment # : ^^mut Type");
  auto cleaver = CodeCleaver();

  try {
#if true
    auto entries = cleaver(code);
    for (const auto& entry : entries) {
      auto chunk = std::get<CodeCleaver::chunk_type>(entry);
      auto result = std::get<CodeCleaver::result_type>(entry);
      std::cout << std::string(chunk.begin(), chunk.end()) << " "
                << make_position_description(result.get_row(),
                                             result.get_column())
                << std::endl;
    }
    auto vessel = Vessel::from(
        Vessel::slice_type<Vessel::entry_type>(entries.begin(), entries.end()));
    std::cout << vessel.view_identifier().get() << std::endl;
#endif
  } catch (std::runtime_error e) {
    std::cout << "Error: " << e.what() << std::endl;
  }
  return 0;
}