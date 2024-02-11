#ifndef PARSER_TOKEN_HPP
#define PARSER_TOKEN_HPP

#include <string>
#include <vector>
#include <span>
#include <stdexcept>
#include <sstream>

/**
 * @file
 *
 * @ingroup Parser
 *
 * @brief Generic tokenizer algorithm.
 */

namespace Parser
{

    //! @brief Builtin token's type for token classification.
    //! @see tokenize
    enum TokenTypes : std::size_t
    {
        OMITTED = 0,
        DEFAULT = 1,
        UTMOST
    };

    //! @brief Rule for specifying `tokenize` when to split.
    //! @tparam CharT Type of character.
    //! @see tokenize
    template <typename CharT>
    struct TokenCutRule
    {
        //! @brief Characters that would be together.
        std::basic_string_view<CharT> characters;

        //! @brief Maximum count for characters to be together.
        std::size_t max_count;

        //! @brief User-defined type for classification.
        //! Could be filled with `TokenTypes` value.
        //! @see TokenTypes
        std::size_t type;

        constexpr TokenCutRule(std::basic_string_view<CharT> p_characters, std::size_t p_max_count, std::size_t p_type)
            : characters(p_characters), max_count(p_max_count), type(p_type) {}
        ~TokenCutRule() = default;
    };

    //! @brief Rule for specifying `tokenize` when to group (ignore `TokenCutRule`).
    //! @tparam CharT Type of character.
    //! @see tokenize
    template <typename CharT>
    struct TokenAmassRule
    {
        //! @brief Starting character to group.
        CharT starting_mark;

        //! @brief Ending character to group.
        CharT ending_mark;

        //! @brief Type for `starting_mark` and `ending_mark`.
        //! @see starting_mark
        //! @see ending_mark
        std::size_t mark_type;

        //! @brief Type for token between `starting_mark` and `ending_mark`.
        std::size_t content_type;

        constexpr TokenAmassRule(CharT p_starting_mark, CharT p_ending_mark, std::size_t p_mark_type, std::size_t p_content_type)
            : starting_mark(p_starting_mark), ending_mark(p_ending_mark), mark_type(p_mark_type), content_type(p_content_type) {}
        ~TokenAmassRule() = default;
    };

    //! @brief Token with metadata output by `tokenize`.
    //! The lifetime depends on the `p_code` parameter of `tokenize`.
    //! @tparam CharT Type of character.
    //! @see tokenize
    template <typename CharT>
    struct Token
    {
        std::basic_string_view<CharT> view;
        std::size_t row;
        std::size_t column;
        std::size_t type;

        constexpr Token(std::basic_string_view<CharT> p_view, std::size_t p_row, std::size_t p_column, std::size_t p_type)
            : view(p_view), row(p_row), column(p_column), type(p_type) {}
        ~Token() = default;
    };

    //! @brief Tokenizer algorithm.
    //! If the characters are from two different `p_cut_rules`, they will be split into different token,
    //! else they would be in the same token until the its `TokenCutRule::max_count` is reached.
    //! Anyting between the starting and ending mark specified by `p_amass_rules` will ignore `p_cut_rules`.
    //! @tparam CharT
    //! @param p_code Code to be tokenized.
    //! @param p_cut_rules Rules regarding when to split the token.
    //! @param p_amass_rules Rules regarding when to ignore `p_cut_rules`.
    //! @return Vector of tokens.
    //! @see TokenCutRule
    //! @see TokenAmassRule
    //! @see Token
    template <typename CharT>
    std::vector<Token<CharT>> tokenize(std::basic_string_view<CharT> p_code, std::span<const TokenCutRule<CharT>> p_cut_rules, std::span<const TokenAmassRule<CharT>> p_amass_rules)
    {
        std::vector<Token<CharT>> tokens;
        const TokenCutRule<CharT> *current_cut_rule_ptr = nullptr;
        const TokenAmassRule<CharT> *current_amass_rule_ptr = nullptr;
        std::basic_string_view<CharT> view(p_code.data(), 0);
        std::size_t row = 1;
        std::size_t column = 1;

        // Loop through code.
        for (const CharT &character : p_code)
        {

            if (current_amass_rule_ptr != nullptr)
            {
                // Amassing.

                // Check stop amassing.
                if (character == current_amass_rule_ptr->ending_mark)
                {
                    // Stop amassing.

                    // Append view.
                    if (current_amass_rule_ptr->content_type != TokenTypes::OMITTED && view.size() > 0)
                        tokens.push_back(Token<CharT>(view, row, column - view.size(), current_amass_rule_ptr->content_type));
                    if (current_amass_rule_ptr->mark_type != TokenTypes::OMITTED)
                        tokens.push_back(Token<CharT>(std::basic_string_view<CharT>(&character, 1), row, column - view.size(), current_amass_rule_ptr->mark_type));

                    // Setup view.
                    view = std::basic_string_view<CharT>(&character + 1, 0);

                    // Stop amassing.
                    current_amass_rule_ptr = nullptr;
                }
                else
                    // Grow token.
                    view = std::basic_string_view<CharT>(view.data(), view.size() + 1);
            }
            else
            {
                // Not amassing.

                // Check start amassing.
                for (const TokenAmassRule<CharT> &rule : p_amass_rules)
                    if (character == rule.starting_mark)
                    {
                        // Append view.
                        if (view.size() > 0)
                        {
                            if (current_cut_rule_ptr == nullptr)
                                tokens.push_back(Token<CharT>(view, row, column - view.size(), TokenTypes::DEFAULT));
                            else if (current_cut_rule_ptr->type != TokenTypes::OMITTED)
                                tokens.push_back(Token<CharT>(view, row, column - view.size(), current_cut_rule_ptr->type));
                        }
                        if (rule.mark_type != OMITTED)
                            tokens.push_back(Token<CharT>(std::basic_string_view<CharT>(&character, 1), row, column - view.size(), rule.mark_type));

                        // Setup view.
                        view = std::basic_string_view<CharT>(&character + 1, 0);

                        // Start amassing.
                        current_amass_rule_ptr = &rule;
                        goto next_character;
                    }

                // Check cutting.
                for (const TokenCutRule<CharT> &rule : p_cut_rules)
                    if (rule.characters.find(character) != rule.characters.npos)
                    {
                        // Not default rule.

                        // Check in current rule.
                        if (current_cut_rule_ptr == nullptr || current_cut_rule_ptr != &rule)
                        {
                            // Not in current rule.

                            // Append view.
                            if (view.size() > 0)
                            {
                                if (current_cut_rule_ptr == nullptr)
                                    tokens.push_back(Token<CharT>(view, row, column - view.size(), TokenTypes::DEFAULT));
                                else if (current_cut_rule_ptr->type != TokenTypes::OMITTED)
                                    tokens.push_back(Token<CharT>(view, row, column - view.size(), current_cut_rule_ptr->type));
                            }

                            // Setup view.
                            view = std::basic_string_view<CharT>(&character, 1);

                            // Start cutting new rule.
                            current_cut_rule_ptr = &rule;
                        }
                        else
                        {
                            // In current rule.

                            // Grow token.
                            view = std::basic_string_view<CharT>(view.data(), view.size() + 1);

                            // Check token reach maximum
                            if (rule.max_count > 0 && view.size() >= rule.max_count)
                            {
                                // Token reach maximum.

                                // Append view.
                                if (rule.type != TokenTypes::OMITTED && view.size() > 0)
                                    tokens.push_back(Token<CharT>(view, row, column - view.size(), rule.type));

                                // Setup view.
                                view = std::basic_string_view<CharT>(&character + 1, 0);
                            }
                        }
                        goto next_character;
                    }

                // Default group.

                // Check in current rule.
                if (current_cut_rule_ptr != nullptr)
                {
                    // Not in current rule.

                    // Append view.
                    if (current_cut_rule_ptr->type != TokenTypes::OMITTED && view.size() > 0)
                        tokens.push_back(Token<CharT>(view, row, column - view.size(), current_cut_rule_ptr->type));

                    // Setup view.
                    view = std::basic_string_view<CharT>(&character, 1);

                    // Start cutting new rule.
                    current_cut_rule_ptr = nullptr;
                }
                else
                {
                    // In current rule.

                    // Grow token.
                    view = std::basic_string_view<CharT>(view.data(), view.size() + 1);
                }
                goto next_character;
            }
        next_character:;
            column += 1;
            if (character == CharT('\n'))
            {
                column = 1;
                row += 1;
            }
        }

        // Check unterminated amassing.
        if (current_amass_rule_ptr != nullptr)
        {
            std::stringstream message;
            message << "Unterminated character `" << current_amass_rule_ptr->ending_mark << "` at row " << row << ", column " << column << ".";
            // Raise error.
            throw std::runtime_error(message.str().c_str());
        }

        // Append view.
        if (view.size() > 0)
        {
            if (current_cut_rule_ptr == nullptr)
                tokens.push_back(Token<CharT>(view, row, column - view.size(), TokenTypes::DEFAULT));
            else if (current_cut_rule_ptr->type != TokenTypes::OMITTED)
                tokens.push_back(Token<CharT>(view, row, column - view.size(), current_cut_rule_ptr->type));
        }

        return tokens;
    }

} // namespace Parser

#endif // PARSER_TOKEN_HPP