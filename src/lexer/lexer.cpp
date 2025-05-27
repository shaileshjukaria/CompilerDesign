#include "lexer.h"
#include <cctype>
#include <map>

static std::map<std::string, TokenType> keywords = {
    {"var", TokenType::VAR},
    {"if", TokenType::IF},
    {"else", TokenType::ELSE},
    {"while", TokenType::WHILE},
    {"for", TokenType::FOR},
    {"true", TokenType::BOOLEAN},
    {"false", TokenType::BOOLEAN},
    {"null", TokenType::NULL_TYPE},
    {"print", TokenType::PRINT}
};

Lexer::Lexer(const std::string &source) : source(source), index(0) {}

char Lexer::peek()
{
    return index < source.length() ? source[index] : '\0';
}

char Lexer::peekNext()
{
    return (index + 1 < source.length()) ? source[index + 1] : '\0';
}

char Lexer::advance()
{
    return source[index++];
}

void Lexer::skipWhiteSpace()
{
    while (index < source.length()) {
        char c = peek();
        if (c == ' ' || c == '\t' || c == '\r' || c == '\n') {
            advance();
        } else {
            break;
        }
    }
}

Token Lexer::identifierOrKeyword()
{
    std::string value;
    while (isalnum(peek()) || peek() == '_')
    {
        value += advance();
    }

    auto it = keywords.find(value);
    if (it != keywords.end())
    {
        return {it->second, value};
    }

    return {TokenType::IDENTIFIER, value};
}

Token Lexer::number()
{
    std::string value;
    bool hasDecimal = false;
    
    while (isdigit(peek()) || (!hasDecimal && peek() == '.'))
    {
        if (peek() == '.') {
            hasDecimal = true;
        }
        value += advance();
    }
    
    // Convert string to number
    try {
        if (hasDecimal) {
            double d = std::stod(value);
            return {TokenType::NUMBER, std::to_string(d)};
        } else {
            int i = std::stoi(value);
            return {TokenType::NUMBER, std::to_string(i)};
        }
    } catch (...) {
        return {TokenType::ERROR, "Invalid number"};
    }
}

Token Lexer::string()
{
    advance(); // Skip the opening quote
    std::string value;

    while (peek() != '"' && peek() != '\0')
    {
        value += advance();
    }

    if (peek() == '"')
    {
        advance();
        return {TokenType::STRING, value};
    }

    return {TokenType::ERROR, "Unterminated string"};
}

std::vector<Token> Lexer::tokenize()
{
    std::vector<Token> tokens;
    while (index < source.length())
    {
        skipWhiteSpace();

        if (peek() == '\0') break;

        // Handle comments first
        if (peek() == '/' && peekNext() == '/')
        {
            // Single-line comment
            while (peek() != '\n' && peek() != '\0')
            {
                advance();
            }
            continue;
        }
        if (peek() == '/' && peekNext() == '*')
        {
            advance();
            advance(); // Skip `/*`
            while (!(peek() == '*' && peekNext() == '/') && peek() != '\0')
            {
                advance();
            }
            if (peek() == '*' && peekNext() == '/')
            {
                advance();
                advance(); // Skip `*/`
            }
            continue;
        }

        // Handle other tokens
        if (isalpha(peek()) || peek() == '_')
        {
            tokens.push_back(identifierOrKeyword());
        }
        else if (isdigit(peek()))
        {
            tokens.push_back(number());
        }
        else if (peek() == '"')
        {
            tokens.push_back(string());
        }
        else
        {
            char c = peek();
            switch (c)
            {
                case '(':
                    tokens.push_back({TokenType::LEFT_PAREN, "("});
                    advance();
                    break;
                case ')':
                    tokens.push_back({TokenType::RIGHT_PAREN, ")"});
                    advance();
                    break;
                case '{':
                    tokens.push_back({TokenType::LEFT_BRACE, "{"});
                    advance();
                    break;
                case '}':
                    tokens.push_back({TokenType::RIGHT_BRACE, "}"});
                    advance();
                    break;
                case '[':
                    tokens.push_back({TokenType::LEFT_BRACKET, "["});
                    advance();
                    break;
                case ']':
                    tokens.push_back({TokenType::RIGHT_BRACKET, "]"});
                    advance();
                    break;
                case '=':
                    if (peekNext() == '=')
                    {
                        tokens.push_back({TokenType::EQUAL, "=="});
                        advance();
                        advance();
                    }
                    else
                    {
                        tokens.push_back({TokenType::EQUAL, "="});
                        advance();
                    }
                    break;
                case ';':
                    tokens.push_back({TokenType::SEMICOLON, ";"});
                    advance();
                    break;
                case '+':
                    tokens.push_back({TokenType::PLUS, "+"});
                    advance();
                    break;
                case '-':
                    tokens.push_back({TokenType::MINUS, "-"});
                    advance();
                    break;
                case '*':
                    tokens.push_back({TokenType::STAR, "*"});
                    advance();
                    break;
                case '/':
                    tokens.push_back({TokenType::SLASH, "/"});
                    advance();
                    break;
                case '<':
                    if (peekNext() == '=')
                    {
                        tokens.push_back({TokenType::LESS_EQUAL, "<="});
                        advance();
                        advance();
                    }
                    else
                    {
                        tokens.push_back({TokenType::LESS, "<"});
                        advance();
                    }
                    break;
                case '>':
                    if (peekNext() == '=')
                    {
                        tokens.push_back({TokenType::GREATER_EQUAL, ">="});
                        advance();
                        advance();
                    }
                    else
                    {
                        tokens.push_back({TokenType::GREATER, ">"});
                        advance();
                    }
                    break;
                case '!':
                    if (peekNext() == '=')
                    {
                        tokens.push_back({TokenType::BANG_EQUAL, "!="});
                        advance();
                        advance();
                    }
                    else
                    {
                        tokens.push_back({TokenType::BANG, "!"});
                        advance();
                    }
                    break;
                case '&':
                    if (peekNext() == '&')
                    {
                        tokens.push_back({TokenType::AND, "&&"});
                        advance();
                        advance();
                    }
                    else
                    {
                        tokens.push_back({TokenType::ERROR, "Expected '&&'"});
                        advance();
                    }
                    break;
                case '|':
                    if (peekNext() == '|')
                    {
                        tokens.push_back({TokenType::OR, "||"});
                        advance();
                        advance();
                    }
                    else
                    {
                        tokens.push_back({TokenType::ERROR, "Expected '||'"});
                        advance();
                    }
                    break;
                default:
                    tokens.push_back({TokenType::ERROR, std::string(1, c)});
                    advance();
            }
        }
    }

    tokens.push_back({TokenType::EOF_TYPE, ""});
    return tokens;
}
































//exical tokenization is conversion of a text into (semantically or syntactically) meaningful lexical tokens belonging to categories defined by a "lexer" program. In case of a natural language, those categories include nouns, verbs, adjectives, punctuations etc. In case of a programming language, the categories include identifiers, operators, grouping symbols, data types and language keywords. Lexical tokenization is related to the type of tokenization used in large language models (LLMs) but with two differences. First, lexical tokenization is usually based on a lexical grammar, whereas LLM tokenizers are usually probability-based. Second, LLM tokenizers perform a second step that converts the tokens into numerical values.

//Rule-based programs
//Lexical tokenization is conversion of a text into (semantically or syntactically) meaningful lexical tokens belonging to categories defined by a "lexer" program. In case of a natural language, those categories include nouns, verbs, adjectives, punctuations etc. In case of a programming language, the categories include identifiers, operators, grouping symbols, data types and language keywords. Lexical tokenization is related to the type of tokenization used in large language models (LLMs) but with two differences. First, lexical tokenization is usually based on a lexical grammar, whereas LLM tokenizers are usually probability-based. Second, LLM tokenizers perform a second step that converts the tokens into numerical values.

/*Rule-based programs
Disambiguation of "lexeme"
Lexical token and lexical tokenization
Lexical grammar
Details
Obstacles
Lexer generator
Phrase structure
Context-sensitive lexing
See also
References
External links
Last edited 2 days ago by OAbot
Related articles
Lex (software)
Lexical analyzer generator
Syntax (programming languages)
Set of rules defining correctly structured programs
Integer literal
Wikipedia
Wikimedia Foundation
Powered by MediaWiki
Content is available under CC BY-SA 4.0 unless otherwise noted.
Privacy policy Contact Wikipedia*/
