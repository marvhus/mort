#ifndef PARSER_H_
#define PARSER_H_

#include "lexer.hpp"
#include <stdint.h>
#include <vector>

class Parser {
    public:
        enum TokenType {
            label,    // TODO: figure out a better name

            number,   // ... numbers
                      // TODO: add more stuff like strings etc

            // Scopes
            brace_start,       // {
            brace_end,         // }

            bracket_start,     // [
            bracket_end,       // ]

            parenthesis_start, // (
            parenthesis_end,   // )

            // Operators
            op_plus,     // +
            op_minus,    // -
            op_divide,   // /
            op_multiply, // *
            op_equals,   // =
            op_greater,  // >
            op_less,     // <

            // Punctuation
            semicolon,   // ;
            colon,       // :
            comma,       // ,

            // Other
            vertical_bar,  // |
            ampersand,    // &
            caret,         // ^
            doubble_quote, // "
            single_quote,  // '

            // None
            none,
        };
        typedef struct {
            TokenType type;
            std::string val;
            uint32_t col;
            uint32_t line;
        } Token;
        static std::vector<Parser::Token> parse_lex_tokens(std::vector<Lexer::Token> l_tokens);
    private:
        static Parser::Token new_token(
            Parser::TokenType type,
            std::string val,
            uint32_t col,
            uint32_t line);
};

#endif // PARSER_H_
