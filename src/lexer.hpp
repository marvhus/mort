#ifndef LEXER_H_
#define LEXER_H_

#include <stdint.h>
#include <string>
#include <vector>

class Lexer {
    public:
        typedef struct {
            std::string str;
            uint32_t col;
            uint32_t line;
        } Token;

        static std::vector<Lexer::Token> lex_file(std::string file);
    private:
        static std::vector<std::string> split_lines(std::string file);
        static void lex_substring(std::vector<Lexer::Token>* tokens, std::string str, size_t prev);
        static std::vector<Lexer::Token> lex_line(std::string line);
        static Lexer::Token new_token(std::string str, uint32_t col, uint32_t line);
};

#endif // LEXER_H_
