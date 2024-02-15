#ifndef KAMPUA_CLEAVE_CLEAVER_HPP
#define KAMPUA_CLEAVE_CLEAVER_HPP

/**
 * @file
 *
 * @ingroup Kampua::Cleave
 *
 * @brief Cleaver for Kampua::Cleave code to generate tokens.
 */

#include "../../cleave/cleave.hpp"
#include "../../concepts.hpp"

#include <cstdint>
#include <string>
#include <memory>
#include <functional>
#include <optional>
#include <stdexcept>
#include <sstream>

namespace Kampua::Cleave
{

    //! @brief Cleaver for tokenize Kampua::Cleave code.
    //! @tparam T Character type.
    template <typename T>
        requires Character<T>
    class Cleaver
    {
    public:
        class Result;

        using value_type = Result;
        using PositionType = typename std::size_t;
        using TagType = uint8_t;
        using CountType = uint8_t;

        enum Tag : TagType
        {
            DEFAULT = 0,
            TERMINATE,
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
            COMMENT
        };

        //! @brief Result of cleave methods, stores metadata.
        class Result
        {
        private:
            ::Cleave::CleaveOption cleave_option;
            PositionType row;
            PositionType column;
            TagType tag;

        public:
            Result(::Cleave::CleaveOption p_cleave_option, PositionType p_row, PositionType p_column, TagType p_tag) noexcept
                : cleave_option(p_cleave_option), row(p_row), column(p_column), tag(p_tag) {}

            inline auto operator<=>(const Result &p_result) const & = default;

            inline auto get_row() const & { return row; }

            inline auto get_column() const & { return column; }

            inline auto get_tag() const & { return tag; }

            operator ::Cleave::CleaveOption() const & { return cleave_option; }
        };

        class Kind
        {
        private:
            std::basic_string<T> characters;
            CountType max_count;
            TagType tag;
            ::Cleave::CleaveOption cleave_option;

        public:
            Kind(std::basic_string<T> p_characters, CountType p_max_count, TagType p_tag, ::Cleave::CleaveOption p_cleave_option = ::Cleave::CleaveOption::BEFORE) noexcept
                : characters(std::move(p_characters)), max_count(p_max_count), tag(p_tag), cleave_option(p_cleave_option) {}

            inline auto operator<=>(const Kind &p_kind) const & = default;

            inline auto get_max_count() const & { return max_count; }

            inline auto get_tag() const & { return tag; }

            inline auto get_cleave_option() const & { return cleave_option; }

            inline const std::basic_string<T> &view_characters() const & { return characters; }
        };

        class Unifier
        {
        private:
            T start_unify_mark;
            T end_unify_mark;
            TagType tag;

        public:
            Unifier(T p_start_unify_mark, T p_end_unify_mark, TagType p_tag) noexcept
                : start_unify_mark(p_start_unify_mark), end_unify_mark(p_end_unify_mark), tag(p_tag) {}

            inline auto operator<=>(const Unifier &p_unifier) const & = default;

            inline auto is_start_unify_mark(T p_character) const & { return start_unify_mark == p_character; }

            inline auto is_end_unify_mark(T p_character) const & { return end_unify_mark == p_character; }

            inline auto get_start_unify_mark() const & { return start_unify_mark; }

            inline auto get_end_unify_mark() const & { return end_unify_mark; }

            inline auto get_tag() const & { return tag; }
        };

    private:
        PositionType row;
        PositionType column;
        std::vector<Kind> kinds;
        std::vector<Unifier> unifiers;
        std::optional<std::reference_wrapper<const Kind>> current_kind;
        std::optional<std::reference_wrapper<const Unifier>> current_unifier;

        static constexpr const auto NEWLINE = T('\n');

        static inline auto make_default_unifiers()
        {
            return std::vector<Unifier>{
                Unifier('\'', '\'', Tag::STRING_LITERAL),
                Unifier('\"', '\"', Tag::STRING_LITERAL),
                Unifier('#', '\n', Tag::COMMENT)};
        }

        static inline auto make_default_kinds()
        {
            return std::vector<Kind>{
                Kind(" \t\n\v\f\r", 0, Tag::DEFAULT, ::Cleave::CleaveOption::OMIT),
                Kind(";", 1, Tag::TERMINATE),
                Kind("+-*/%:=!", 2, Tag::GENERIC_OPERATOR),
                Kind("^", 1, Tag::POINTER_OPERATOR),
                Kind("@", 1, Tag::ADDRESS_OPERATOR),
                Kind("&", 2, Tag::AND_OPERATOR),
                Kind("|", 2, Tag::OR_OPERATOR),
                Kind("()", 1, Tag::PARENTHESIS),
                Kind("[]", 1, Tag::SQUARE_BRACKET),
                Kind("{}", 1, Tag::CURLY_BRACKET)};
        }

    public:
        Cleaver()
            : row(1), column(0), kinds(make_default_kinds()), unifiers(make_default_unifiers()) {}

        Cleaver(std::vector<Kind> p_kinds, std::vector<Unifier> p_unifiers)
            : row(1), column(0), kinds(std::move(p_kinds)), unifiers(std::move(p_unifiers)) {}

        value_type cleave(const T &p_value, ::Cleave::CleaveChunkSize p_size)
        {
            // Check new line.
            if (p_value == NEWLINE)
            {
                row += 1;
                column = 0;
            }
            column += 1;

            // Check unifying.
            if (current_unifier.has_value())
            {
                const auto &unifier = current_unifier->get();
                // Check stop unifying.
                if (unifier.is_end_unify_mark(p_value))
                {
                    // Stop unifying.
                    current_unifier = std::nullopt;
                    return Result(::Cleave::CleaveOption::OMIT, row, column - p_size, unifier.get_tag());
                }
                else
                    // Grow.
                    return Result(::Cleave::CleaveOption::IGNORE, row, column - p_size, Tag::DEFAULT);
            }

            // Check start unifying.
            for (const auto &unifier : unifiers)
                if (unifier.is_start_unify_mark(p_value))
                {
                    current_unifier = std::cref(unifier);
                    return Result(::Cleave::CleaveOption::OMIT, row, column - p_size, current_kind.has_value() ? current_kind->get().get_tag() : Tag::DEFAULT);
                }

            // Determine current kind.
            auto current_kind = std::optional<std::reference_wrapper<const Kind>>(std::nullopt);
            for (const auto &kind : kinds)
            {
                const auto &characters = kind.view_characters();
                if (characters.find(p_value) != characters.npos)
                {
                    // Found current kind.
                    current_kind = std::cref(kind);
                    break;
                }
            }

            // Determine previous kind
            auto &previous_kind = this->current_kind;
            auto is_equal = (current_kind.has_value() == previous_kind.has_value()) &&
                            (current_kind.has_value() && previous_kind.has_value() ? current_kind->get() == previous_kind->get() : true);

            // Check kind equal.
            if (is_equal)
            {
                // Check max count.
                if (current_kind.has_value())
                    if (p_size + 1 >= current_kind->get().get_max_count())
                    {
                        const auto tag = previous_kind.has_value() ? previous_kind->get().get_tag() : Tag::DEFAULT;
                        return Result(::Cleave::CleaveOption::BEFORE, row, column - p_size, tag);
                    }

                // Grow.
                return Result(::Cleave::CleaveOption::IGNORE, row, column - p_size, Tag::DEFAULT);
            }
            else
            {
                // Switch to new kind.
                const auto tag = previous_kind.has_value() ? previous_kind->get().get_tag() : Tag::DEFAULT;
                const auto cleave_option = current_kind.has_value() ? current_kind->get().get_cleave_option() : ::Cleave::CleaveOption::BEFORE;
                this->current_kind = std::move(current_kind);
                return Result(cleave_option, row, column - p_size, tag);
            }
        }

        value_type terminate(::Cleave::CleaveChunkSize p_size)
        {
            if (current_unifier.has_value())
            {
                std::stringstream message;
                message << "Unterminated character `" << current_unifier->get().get_end_unify_mark() << "` at row " << row << ", column " << column << ".";
                // Raise error.
                throw std::runtime_error(message.str().c_str());
            }

            return Result(::Cleave::CleaveOption::IGNORE, row, column - p_size, current_kind.has_value() ? current_kind->get().get_tag() : Tag::DEFAULT);
        }
    };

} // namespace Kampua::Cleave

#endif // KAMPUA_CLEAVE_CLEAVER_HPP