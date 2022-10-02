#include "lexer/lex_parser.hpp"

#include "lexer/lexer.hpp"
#include <iostream>
#include <stdint.h>
#include <vector>
#include <regex>
#include <cassert>

std::vector<LexParser::Token> LexParser::parse_lex_tokens(std::vector<Lexer::Token> l_tokens)
{
    std::vector<LexParser::Token> p_token;

    uint32_t comment_line = 0;
    bool is_comment_line = 0;

    bool is_block_comment = false;
    uint32_t block_comment_count = 0;

    for (Lexer::Token l_token : l_tokens) {
        std::string str = l_token.str;
        uint32_t col = l_token.col;
        uint32_t line = l_token.line;

        if (line == comment_line && is_comment_line) {
            continue;
        }
        if (is_block_comment && block_comment_count > 0) {
            if (str == "/*") {
                block_comment_count++;
            }
            if (str == "*/") {
                block_comment_count--;
                is_block_comment = block_comment_count > 0;
            }
            continue;
        }

        LexParser::Token tok = LexParser::new_token(none, str, col, line);

        // I did it this way because for some reason
        // C++ does not support strings in switch statements

        if (str == "//") {
            is_comment_line = true;
            comment_line = line;
            continue;
        }
        else if (str == "/*") {
            is_block_comment = true;
            block_comment_count++;
            continue;
        }
        // Scopes
        else if (str == "{") {
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
                tok.type = identifier;
            }
            else {
                std::cout << "Invalid token: '" << str << "' at col: " << col << " line: " << line << "\n";
                exit(1);
            }
        }
        if (tok.type == none) {
            std::cout << "Token type is none. str: '" << str << "' at col: " << col << " line: " << line << "\n";
            exit(1);
        }
        p_token.push_back(tok);
    }

    return p_token;
}

LexParser::Token LexParser::new_token(LexParser::TokenType type, std::string val, uint32_t col, uint32_t line)
{
    LexParser::Token token;
    token.type = type;
    token.val = val;
    token.col = col;
    token.line = line;

    return token;
}
