#include <iostream>
#include "kampua/cleaver.hpp"

int main()
{
#if true
    auto code = std::basic_string<char>("var something : ^^^T = 'some string.';;;\nvar another_thing := 'sometext'");
    auto cleaver = Kampua::Cleaver<char>();

    try
    {
        for (auto chunk : Cleave::cleave(code, cleaver))
        {
            auto view = std::get<std::span<const char>>(chunk);
            auto data = std::get<Kampua::Cleaver<char>::Result>(chunk);
            std::cout << std::basic_string_view<char>(view.begin(), view.end()) << " [row :" << data.get_row() << "; column: " << data.get_column() << "]." << std::endl;
        }
    }
    catch (std::runtime_error e)
    {
        std::cout << "Error: " << e.what() << std::endl;
    }
#endif
    return 0;
}