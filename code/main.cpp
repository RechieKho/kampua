#include <iostream>
#include "parser/token.hpp"

using namespace Parser;

int main()
{
    constexpr const TokenCutRule<char> cut_rules[] = {
        TokenCutRule<char>(":=", 2, TokenTypes::DEFAULT),
        TokenCutRule<char>(" \n", 0, TokenTypes::OMITTED)};
    constexpr const TokenAmassRule<char> amass_rules[] = {
        TokenAmassRule<char>('\'', '\'', TokenTypes::DEFAULT, TokenTypes::DEFAULT)};
    const char *code = "var something := 'some string.'\nvar another_thing := 'sometext'";

    try
    {
        const auto result = tokenize<char>(code, cut_rules, amass_rules);
        for (const auto &token : result)
        {
            std::cout << '\'' << token.view << "\' [column: " << token.column << "; row: " << token.row << "]." << '\n';
        }
    }
    catch (std::runtime_error e)
    {
        std::cout << "Error: " << e.what() << std::endl;
    }
    return 0;
}