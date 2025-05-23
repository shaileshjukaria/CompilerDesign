#ifndef LEXER_H
#define LEXER_H

#include "./token.h"
#include <vector>
#include <string>


class Lexer {
    public:
        explicit Lexer(const std::string &source);
        //to convert source code into tokens
        std::vector<Token> tokenize();
    private:

        std::string source;
        size_t index = 0;

        //look at current character
        char peek();
        //Move to the next character
        char advance();
        //
        char peekNext();
        //to skip whitespaces
        void skipWhiteSpace();

        //Handle variable names
        Token identifierOrKeyword();
        //Handles numeric literals
        Token number();
        //Handles string literals
        Token string();
        
};



#endif