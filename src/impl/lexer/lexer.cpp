#include "lexer/lexer.hpp"

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
            tokens.push_back(new_token(l.str, l.col, i+1));
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
    enum Status {text, none};
    Status s = none;
    //std::cout << "Looping over\n";
    std::string buff = "";
    size_t offset = 0;
    bool skip_next = false;
    for (char c : str) {
        if (skip_next) {
            skip_next = false;
            continue;
        }
        // Text
        if ((c >= 'a' && c <= 'z')
         || (c >= 'A' && c <= 'Z')
         || (c >= '0' && c <= '9')
         || (c == '_')
         || (c == '.')
        ) {
            //std::cout << "letter: " << c << "\n";
            if (s != text && buff != "") {
                tokens->push_back(Lexer::new_token(buff, prev+offset-buff.length(), 0));
                buff = "";
            }
            buff += c;
            s = text;
        }
        // Comments
        else if (
            (c == '/' && str[offset+1] == '/')
         || (c == '/' && str[offset+1] == '*')
         || (c == '*' && str[offset+1] == '/')
        ) {
            if (buff != "") {
                tokens->push_back(Lexer::new_token(buff, prev+offset-buff.length(), 0));
            }
            std::string s (1, c); s += str[++offset];
            tokens->push_back(Lexer::new_token(s, prev+offset-buff.length(), 0));
            s = none;
            skip_next = true;
        }
        // Operators
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
         || c == '/'
        ) {
            if (buff != "") {
                tokens->push_back(Lexer::new_token(buff, prev+offset-buff.length(), 0));
                buff = "";
            }
            std::string s (1, c);
            // <=  >=  ==  &=  *=  +=  -=  !=  ^=  /=
            if (str[offset+1] == '=') {
                s += std::string(1, str[++offset]);
                skip_next = true;
            }
            tokens->push_back(Lexer::new_token(s, prev+offset, 0));
            s = none;
        }
        // Other
        else if (
            c == ':'
         || c == ';'
         || c == '"'
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
