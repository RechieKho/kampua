#include <iostream>
#include "parser/parser.hpp"

using namespace Parser;

int main()
{
    const char *code = "var something := 'some string.'\nvar another_thing := 'sometext'";

    try
    {
        const auto result = parse<char>(code);
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