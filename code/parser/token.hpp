#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <string>
#include <vector>
#include <span>
#include <stdexcept>

namespace Parser
{

    template <typename T>
    struct TokenCutRule
    {
        std::basic_string_view<T> characters;
        size_t max_count;
        bool is_omitted;

        constexpr TokenCutRule(std::basic_string_view<T> p_characters, std::size_t p_max_count, bool p_is_omitted)
            : characters(p_characters), max_count(p_max_count), is_omitted(p_is_omitted) {}
        ~TokenCutRule() = default;
    };

    template <typename T>
    struct TokenAmassRule
    {
        T starting_mark;
        T ending_mark;
        bool is_mark_omitted;
        bool is_token_omitted;

        constexpr TokenAmassRule(T p_starting_mark, T p_ending_mark, bool p_is_mark_omitted, bool p_is_token_omitted)
            : starting_mark(p_starting_mark), ending_mark(p_ending_mark), is_mark_omitted(p_is_mark_omitted), is_token_omitted(p_is_token_omitted) {}
        ~TokenAmassRule() = default;
    };

    template <typename T>
    std::vector<std::basic_string_view<T>> tokenize(std::basic_string_view<T> p_code, std::span<const TokenCutRule<T>> p_cut_rules, std::span<const TokenAmassRule<T>> p_amass_rules)
    {
        std::vector<std::basic_string_view<T>> tokens;
        const TokenCutRule<T> *current_cut_rule_ptr = nullptr;
        const TokenAmassRule<T> *current_amass_rule_ptr = nullptr;
        std::basic_string_view<T> buffer(p_code.data(), 0);

        // Loop through code.
        for (const T &character : p_code)
        {
            if (current_amass_rule_ptr != nullptr)
            {
                // Amassing.

                // Check stop amassing.
                if (character == current_amass_rule_ptr->ending_mark)
                {
                    // Append buffer.
                    if (!current_amass_rule_ptr->is_token_omitted)
                        if (buffer.size() != 0)
                            tokens.push_back(buffer);
                    if (!current_amass_rule_ptr->is_mark_omitted)
                        tokens.push_back(std::basic_string_view<T>(&character, 1));

                    // Setup buffer.
                    buffer = std::basic_string_view<T>(&character + 1, 0);

                    // Stop amassing.
                    current_amass_rule_ptr = nullptr;
                }
                else
                    // Grow token.
                    buffer = std::basic_string_view<T>(buffer.data(), buffer.size() + 1);
            }
            else
            {
                // Not amassing.

                // Check start amassing.
                for (const TokenAmassRule<T> &rule : p_amass_rules)
                    if (character == rule.starting_mark)
                    {
                        // Append buffer.
                        if (current_cut_rule_ptr == nullptr || !current_cut_rule_ptr->is_omitted)
                            if (buffer.size() != 0)
                                tokens.push_back(buffer);
                        if (!rule.is_mark_omitted)
                            tokens.push_back(std::basic_string_view<T>(&character, 1));

                        // Setup buffer.
                        buffer = std::basic_string_view<T>(&character + 1, 0);

                        // Start amassing.
                        current_amass_rule_ptr = &rule;
                        goto next_character;
                    }

                // Check cutting.
                for (const TokenCutRule<T> &rule : p_cut_rules)
                    if (rule.characters.find(character) != rule.characters.npos)
                    {
                        // Not default group.

                        // Check in current rule.
                        if (current_cut_rule_ptr == nullptr || current_cut_rule_ptr != &rule)
                        {
                            // Not in current rule.

                            // Append buffer.
                            if (current_cut_rule_ptr == nullptr || !current_cut_rule_ptr->is_omitted)
                                if (buffer.size() != 0)
                                    tokens.push_back(buffer);

                            // Setup buffer.
                            buffer = std::basic_string_view<T>(&character, 1);

                            // Start cutting.
                            current_cut_rule_ptr = &rule;
                        }
                        else
                        {
                            // In current rule.

                            // Grow token.
                            buffer = std::basic_string_view<T>(buffer.data(), buffer.size() + 1);

                            // Check token reach maximum
                            if (rule.max_count > 0 && buffer.size() >= rule.max_count)
                            {
                                // Token reach maximum.

                                // Append buffer.
                                if (!rule.is_omitted && buffer.size() != 0)
                                    tokens.push_back(buffer);

                                // Setup buffer.
                                buffer = std::basic_string_view<T>(&character + 1, 0);
                            }
                        }
                        goto next_character;
                    }

                // Default group.

                // Check in current rule.
                if (current_cut_rule_ptr != nullptr)
                {
                    // Not in current rule.

                    // Append buffer.
                    if (!current_cut_rule_ptr->is_omitted && buffer.size() != 0)
                        tokens.push_back(buffer);

                    // Setup buffer.
                    buffer = std::basic_string_view<T>(&character, 1);

                    // Start cutting.
                    current_cut_rule_ptr = nullptr;
                }
                else
                {
                    // In current rule.

                    // Grow token.
                    buffer = std::basic_string_view<T>(buffer.data(), buffer.size() + 1);
                }
                goto next_character;
            }
        next_character:;
        }

        // Check unterminated amassing.
        if (current_amass_rule_ptr != nullptr)
        {
            // Raise error.
            throw std::runtime_error(std::string("Unterminated character. [`") + current_amass_rule_ptr->ending_mark + "`]");
        }

        // Append buffer.
        if (buffer.size() != 0)
        {
            tokens.push_back(buffer);
        }

        return tokens;
    }

} // namespace Parser

#endif // TOKEN_HPP