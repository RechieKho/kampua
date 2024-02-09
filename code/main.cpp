#include <iostream>
#include "parser/token.hpp"

using namespace Parser;

int main()
{
    constexpr const TokenCutRule<char> cut_rules[] = {
        TokenCutRule<char>(":=", 2, false),
        TokenCutRule<char>(" ", 0, true)};
    constexpr const TokenAmassRule<char> amass_rules[] = {
        TokenAmassRule<char>('\'', '\'', true, false)};
    const char *code = "var  something := 'some string.'";

    try
    {
        const auto result = tokenize<char>(code, cut_rules, amass_rules);
        for (const auto &token : result)
        {
            std::cout << '\'' << token << '\'' << '\n';
        }
    }
    catch (std::exception e)
    {
        std::cout << "Error: " << e.what() << std::endl;
    }
    return 0;
}