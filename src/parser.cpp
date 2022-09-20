#include "parser.hpp"

#include "lexer.hpp"
#include <iostream>
#include <stdint.h>
#include <vector>
#include <regex>
#include <cassert>

std::vector<Parser::Token> Parser::parse_lex_tokens(std::vector<Lexer::Token> l_tokens)
{
    std::vector<Parser::Token> p_token;

    for (Lexer::Token l_token : l_tokens) {
        std::string str = l_token.str;
        uint32_t col = l_token.col;
        uint32_t line = l_token.line;

        Parser::Token tok = Parser::new_token(none, str, col, line);

        // I did it this way because for some reason
        // C++ does not support strings in switch statements

        // Scopes
        if (str == "{") {
            tok.type = brace_start;
        }
        else if (str == "}") {
            tok.type = brace_end;
        }
        else if (str == "[") {
            tok.type = bracket_start;
        }
        else if (str == "]") {
            tok.type = bracket_end;
        }
        else if (str == "(") {
            tok.type = parenthesis_start;
        }
        else if (str == ")") {
            tok.type = parenthesis_end;
        }
        // Operators
        else if (str == "+") {
            tok.type = op_plus;
        }
        else if (str == "-") {
            tok.type = op_minus;
        }
        else if (str == "/") {
            tok.type = op_divide;
        }
        else if (str == "*") {
            tok.type = op_multiply;
        }
        else if (str == "=") {
            tok.type = op_equals;
        }
        else if (str == ">") {
            tok.type = op_greater;
        }
        else if (str == "<") {
            tok.type = op_less;
        }
        // Punctuation
        else if (str == ";") {
            tok.type = semicolon;
        }
        else if (str == ":") {
            tok.type = colon;
        }
        else if (str == ",") {
            tok.type = comma;
        }
        // Other
        else if (str == "|") {
            tok.type = vertical_bar;
        }
        else if (str == "&") {
            tok.type = ampersand;
        }
        else if (str == "^") {
            tok.type = caret;
        }
        else if (str == "\"") {
            tok.type = doubble_quote;
        }
        else if (str == "'") {
            tok.type = single_quote;
        }
        else if (str == "<=") {
            tok.type = op_less_equals;
        }
        else if (str == ">=") {
            tok.type = op_greater_equals;
        }
        else if (str == "==") {
            tok.type = op_equals;
        }
        else if (str == "&=") {
            tok.type = op_and_equals;
        }
        else if (str == "*=") {
            tok.type = op_multiply_equals;
        }
        else if (str == "+=") {
            tok.type = op_plus_equals;
        }
        else if (str == "-=") {
            tok.type = op_minus_equals;
        }
        else if (str == "!=") {
            tok.type = op_not_equals;
        }
        else if (str == "^=") {
            tok.type = op_xor_equals;
        }
        else if (str == "/=") {
            tok.type = op_divide_equals;
        }
        else {
            // is float or int  (12321 or 231.1214)
            if (std::regex_match(str, std::regex("[0-9]*\\.?[0-9]*"))) {
                tok.type = number;
            }
            // is label
            else if (std::regex_match(str, std::regex("[a-zA-Z0-9_]*"))) {
                if (str[0] >= '0' && str[0] <= '9') {
                    std::cout << "Invalid token: " << str << " - first char of label can't be number\n";
                    exit(1);
                }
                tok.type = label;
            }
            else {
                std::cout << "Invalid token: '" << str << "' at col: " << col << " line: " << line << "\n";
            }
        }

        assert(tok.type != none);
        p_token.push_back(tok);
    }

    return p_token;
}

Parser::Token Parser::new_token(Parser::TokenType type, std::string val, uint32_t col, uint32_t line)
{
    Parser::Token token;
    token.type = type;
    token.val = val;
    token.col = col;
    token.line = line;

    return token;
}
