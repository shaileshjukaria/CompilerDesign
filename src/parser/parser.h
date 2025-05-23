#ifndef PARSER_H
#define PARSER_H

#include "../lexer/lexer.h"
#include "../ast/ast.h"
#include <memory>
#include <vector>

class Parser {
    private:
        std::vector<Token> tokens;
        int index = 0;

        Token peek();
        Token advance();
        bool match(TokenType type);
        bool check(TokenType type);
        Token consume(TokenType type, const std::string& message);

        // Expression parsing
        std::unique_ptr<ASTNode> parseExpression();
        std::unique_ptr<ASTNode> parseAssignment();
        std::unique_ptr<ASTNode> parseEquality();
        std::unique_ptr<ASTNode> parseComparison();
        std::unique_ptr<ASTNode> parseTerm();
        std::unique_ptr<ASTNode> parseFactor();
        std::unique_ptr<ASTNode> parsePrimary();

        // Statement parsing
        std::unique_ptr<Statement> parseStatement();
        std::unique_ptr<Statement> parseExpressionStatement();
        std::unique_ptr<Statement> parseVarDeclaration();
        std::unique_ptr<Statement> parseBlock();
        std::unique_ptr<Statement> parseIfStatement();
        std::unique_ptr<Statement> parseWhileStatement();
        std::unique_ptr<Statement> parseForStatement();
        std::unique_ptr<Statement> parsePrintStatement();

    public:
        Parser(const std::vector<Token> &tokens);
        std::vector<std::unique_ptr<Statement>> parse();
};
#endif