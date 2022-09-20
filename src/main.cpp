#include <iostream>
#include <fstream>
#include <string>
#include <cassert>
#include <vector>

#include "lexer.hpp"

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

int main(int argc, char** argv)
{
    assert(argc >= 2);

    std::string contents = read_file(argv[1]);
    // std::cout << contents << std::endl;
    std::vector<Lexer::Token> tokens = Lexer::lex_file(contents);

    for (Lexer::Token token : tokens) {
        std::cout << token.str << "\t- " << token.col << " \t: " << token.line << "\n";
    }

    return 0;
}
