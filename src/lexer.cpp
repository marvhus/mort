#include "lexer.hpp"

#include <stdint.h>
#include <string>
#include <string_view>
#include <string.h>
#include <vector>
#include <iostream>

std::vector<Lexer::Token> Lexer::lex_file(std::string file) {
    std::vector<Lexer::Token> tokens;

    std::vector<std::string> lines = split_lines(file);

    uint32_t i = 0;
    size_t line_count = lines.size();
    while (i < line_count) {
        std::vector<Lexer::Token> line = Lexer::lex_line(lines[i]);
        for (Lexer::Token l : line) {
            tokens.push_back(new_token(l.str, l.col, i));
        }
        i++;
    }

    return tokens;
}

std::vector<std::string> Lexer::split_lines(std::string file) {
    std::vector<std::string> lines;

    size_t pos = 0;
    size_t prev = 0;

    std::string delimeter = "\n";

    while ((pos = file.find(delimeter, prev)) != std::string::npos) {
        lines.push_back(file.substr(prev, pos - prev));
        prev = pos + delimeter.size();
    }

    lines.push_back(file.substr(prev));

    return lines;
}

void Lexer::lex_substring(std::vector<Lexer::Token>* tokens, std::string str, size_t prev) {
    enum Status {letter, number, none};
    Status s = none;
    //std::cout << "Looping over\n";
    std::string buff = "";
    size_t offset = 0;
    bool first = true;
    Status lastStatus = s;
    for (char c : str) {
        if (lastStatus != s) {
            first = true;
        }
        // is letter or number or _
        if (
           ((c >= 'a' && c <= 'z')
         || (c >= 'A' && c <= 'Z')
         || (c >= '0' && c <= '9') // temporarely disabled
         || (c == '_'))
         && s != number
        ) {
            // is first number
            if (c >= '0' && c <= '9' && first) {
                buff += c;
                s = number;
                first = false;
                offset++;
                continue;
            }
            //std::cout << "letter: " << c << "\n";
            if (s != letter && buff != "") {
                tokens->push_back(Lexer::new_token(buff, prev+offset-buff.length(), 0));
                buff = "";
            }
            buff += c;
            s = letter;
            first = false;
        }
        // is number
        else if ((c >= '0' && c <= '9') || (c == '.')) {
            //std::cout << "number: " << c << "\n";
            if (s != number && buff != "") {
                tokens->push_back(Lexer::new_token(buff, prev+offset-buff.length(), 0));
                buff = "";
            }
            buff += c;
            s = number;
            first = false;
        }
        // is single letter symbol
        // TODO: add more symbols
        else if (
            c == '<'
         || c == '>'
         || c == '='
         || c == '&'
         || c == '*'
         || c == '+'
         || c == '-'
         || c == '!'
         || c == '^'
         || c == ':'
         || c == ';'
         || c == '"'
         || c == '/'
         || c == '|'
         || c == '(' || c == ')'
         || c == '[' || c == ']'
         || c == '{' || c == '}'
        ) {
            //std::cout << "symbol: " << c << "\n";
            if (buff != "") {
                tokens->push_back(Lexer::new_token(buff, prev+offset-buff.length(), 0));
                buff = "";
            }
            std::string s (1, c);
            tokens->push_back(Lexer::new_token(s, prev+offset, 0));
            s = none;
            first = false;
        }

        offset++;
    }
    if (buff != "") {
        tokens->push_back(Lexer::new_token(buff, prev+offset-buff.length(), 0));
    }
}

std::vector<Lexer::Token> Lexer::lex_line(std::string line) {
    std::vector<Lexer::Token> tokens;

    size_t pos = 0;
    size_t prev = 0;

    std::string delimeter = " ";


    while ((pos = line.find(delimeter, prev)) != std::string::npos) {
        lex_substring(&tokens, line.substr(prev, pos - prev), prev);
        prev = pos + delimeter.size();
    }
    lex_substring(&tokens, line.substr(prev), prev);

    return tokens;
}


Lexer::Token Lexer::new_token(std::string str, uint32_t col, uint32_t line) {
    Lexer::Token token;
    token.str = str;
    token.col = col;
    token.line = line;

    return token;
}
