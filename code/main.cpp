#include <iostream>
#include <span>
#include "kampua/cleave/cleaver.hpp"
#include "kampua/tree/basis/vessel_builder.hpp"

int main()
{
    auto code = std::basic_string<char>("vessel: mut^mut^mut Type");
    auto cleaver = Kampua::Cleave::Cleaver<char>();

    try
    {
        auto chunks = Cleave::cleave(code, cleaver);
        auto vessel = Kampua::Tree::Basis::VesselBuilder<const char>::build(std::span{chunks.begin(), chunks.end()});

#if false
        for (auto chunk : Cleave::cleave(code, cleaver))
        {
            auto view = std::get<std::span<const char>>(chunk);
            auto data = std::get<Kampua::Cleave::Cleaver<char>::Result>(chunk);
            std::cout << std::basic_string_view<char>(view.begin(), view.end()) << " [row :" << data.get_row() << "; column: " << data.get_column() << "]." << std::endl;
        }
#endif
    }
    catch (std::runtime_error e)
    {
        std::cout << "Error: " << e.what() << std::endl;
    }
    return 0;
}