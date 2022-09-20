#include <iostream>
#include <fstream>
#include <string>
#include <cassert>
#include <vector>

#include "lexer.hpp"
#include "parser.hpp"

std::string read_file(const char* path)
{
    std::ifstream file (path);
    std::string contents, line;
    if (file.is_open()) {
        while ( file ) {
            std::getline (file, line);
            contents += line + '\n';
        }
        file.close();
    }
    return contents;
}

void print_tokens(std::vector<Lexer::Token> tokens)
{
    std::cout << "Lexer tokens\n";
    for (Lexer::Token token : tokens) {
        std::string padding (10 - token.str.length(), ' ') ;
        std::cout << token.str
                  << padding
                  << "\t- col: "
                  << token.col
                  << "\tline: "
                  << token.line
                  << "\n";
    }
}
void print_tokens(std::vector<Parser::Token> tokens) {
    std::cout << "Parser tokens\n";
    for (Parser::Token token : tokens) {
        std::string padding (10 - token.val.length(), ' ') ;
        std::cout << token.type
                  << "\t- val: "
                  << token.val
                  << padding
                  << "\t- col: "
                  << token.col
                  << "\tline: "
                  << token.line
                  << "\n";
    }
}

int main(int argc, char** argv)
{
    assert(argc >= 2);

    std::string contents = read_file(argv[1]);
    // std::cout << contents << std::endl;
    std::vector<Lexer::Token> l_tokens = Lexer::lex_file(contents);
    print_tokens(l_tokens);

    std::vector<Parser::Token> p_tokens = Parser::parse_lex_tokens(l_tokens);
    print_tokens(p_tokens);

    return 0;
}
