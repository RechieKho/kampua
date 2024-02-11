#ifndef PARSER_HPP
#define PARSER_HPP

#include "token.hpp"

/**
 * @file
 *
 * @ingroup Parser
 *
 * @brief Parse Kampua code, with given tokenizer rules.
 */

namespace Parser
{
    //! @brief Extra parser token types for token classification.
    //! @see TokenTYpes
    enum ParserTokenTypes : std::size_t
    {
        TERMINATE = TokenTypes::UTMOST,
        GENERIC_OPERATOR,
        ASSIGNMENT_OPERATOR,
        POINTER_OPERATOR,
        ADDRESS_OPERATOR,
        AND_OPERATOR,
        OR_OPERATOR,
        PARENTHESIS,
        SQUARE_BRACKET,
        CURLY_BRACKET,
        STRING_LITERAL,
    };

    //! @brief Cut rules for Kampua code.
    //! @tparam CharT Type of character.
    //! @see TokenCutRule
    template <typename CharT>
    constexpr const TokenCutRule<CharT> CUT_RULES[] = {
        TokenCutRule<CharT>(" \t\n\v\f\r", 0, TokenTypes::OMITTED),
        TokenCutRule<CharT>(";", 1, ParserTokenTypes::TERMINATE),
        TokenCutRule<CharT>("+-*/%:=!", 2, ParserTokenTypes::GENERIC_OPERATOR),
        TokenCutRule<CharT>("^", 1, ParserTokenTypes::POINTER_OPERATOR),
        TokenCutRule<CharT>("@", 1, ParserTokenTypes::ADDRESS_OPERATOR),
        TokenCutRule<CharT>("&", 2, ParserTokenTypes::AND_OPERATOR),
        TokenCutRule<CharT>("|", 2, ParserTokenTypes::OR_OPERATOR),
        TokenCutRule<CharT>("()", 1, ParserTokenTypes::PARENTHESIS),
        TokenCutRule<CharT>("[]", 1, ParserTokenTypes::SQUARE_BRACKET),
        TokenCutRule<CharT>("{}", 1, ParserTokenTypes::CURLY_BRACKET)};

    //! @brief Amass rule for Kampua code.
    //! @tparam CharT Type of character.
    //! @see TokenAmassRule
    template <typename CharT>
    constexpr const TokenAmassRule<CharT> AMASS_RULES[] = {
        TokenAmassRule<CharT>( // Single quoted string literal.
            '\'',
            '\'',
            TokenTypes::OMITTED,
            ParserTokenTypes::STRING_LITERAL),
        TokenAmassRule<CharT>( // Double quoted string literal.
            '\"',
            '\"', TokenTypes::OMITTED,
            ParserTokenTypes::STRING_LITERAL),
        TokenAmassRule<CharT>( // Single line comment.
            '#',
            '\n',
            TokenTypes::OMITTED,
            TokenTypes::OMITTED)};

    //! @brief Parse kampua code.
    //! A simple wrapper around `tokenize` with kampua code's tokenizer rules.
    //! @tparam CharT Type of character.
    //! @param p_code Kampua code.
    //! @return Vector of tokens.
    //! @see tokenize
    template <typename CharT>
    inline std::vector<Token<CharT>> parse(std::basic_string_view<CharT> p_code)
    {
        return tokenize<CharT>(p_code, CUT_RULES<CharT>, AMASS_RULES<CharT>);
    }
} // namespace Parser

#endif // PARSER_HPP