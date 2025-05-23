#ifndef TOKEN_H
#define TOKEN_H

#include<string>

enum class TokenType {
    // Single-character tokens
    LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE,
    COMMA, DOT, MINUS, PLUS, SEMICOLON, SLASH, STAR,
    LEFT_BRACKET, RIGHT_BRACKET, COLON,
    
    // One or two character tokens
    BANG, BANG_EQUAL,
    EQUAL, EQUAL_EQUAL,
    GREATER, GREATER_EQUAL,
    LESS, LESS_EQUAL,
    
    // Literals
    IDENTIFIER, STRING, NUMBER, BOOLEAN, NULL_TYPE,
    
    // Keywords
    AND, CLASS, ELSE, FALSE, FOR, FUN, IF, NIL, OR,
    PRINT, RETURN, SUPER, THIS, TRUE, VAR, WHILE,
    
    // Special
    ERROR, EOF_TYPE
};

struct Token {
    TokenType type;
    std::string value;
};


#endif