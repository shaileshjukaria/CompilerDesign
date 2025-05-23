#include "parser.h"
#include <stdexcept>
#include "../ast/ast.h"

// Constructor
Parser::Parser(const std::vector<Token>& tokens) : tokens(tokens) {}

// Helper methods
Token Parser::peek() {
    return (index < tokens.size()) ? tokens[index] : Token{TokenType::EOF_TYPE, ""};
}

Token Parser::advance() {
    return tokens[index++];
}

bool Parser::match(TokenType type) {
    if (peek().type == type) {
        advance();
        return true;
    }
    return false;
}

bool Parser::check(TokenType type) {
    return peek().type == type;
}

Token Parser::consume(TokenType type, const std::string& message) {
    if (check(type)) return advance();
    throw std::runtime_error(message);
}

// Expression parsing
std::unique_ptr<ASTNode> Parser::parseExpression() {
    return parseAssignment();
}

std::unique_ptr<ASTNode> Parser::parseAssignment() {
    auto expr = parseEquality();
    
    if (match(TokenType::EQUAL)) {
        auto value = parseAssignment();
        if (auto* var = dynamic_cast<VariableExpr*>(expr.get())) {
            return std::make_unique<AssignmentExpr>(var->name, std::move(value));
        }
        // Error: Invalid assignment target
        throw std::runtime_error("Invalid assignment target");
    }
    
    return expr;
}

std::unique_ptr<ASTNode> Parser::parseEquality() {
    auto expr = parseComparison();
    
    while (match(TokenType::EQUAL_EQUAL) || match(TokenType::BANG_EQUAL)) {
        Token op = tokens[index - 1];
        auto right = parseComparison();
        expr = std::make_unique<BinaryExpr>(op, std::move(expr), std::move(right));
    }
    
    return expr;
}

std::unique_ptr<ASTNode> Parser::parseComparison() {
    auto expr = parseTerm();
    
    while (match(TokenType::LESS) || match(TokenType::LESS_EQUAL) ||
           match(TokenType::GREATER) || match(TokenType::GREATER_EQUAL)) {
        Token op = tokens[index - 1];
        auto right = parseTerm();
        expr = std::make_unique<BinaryExpr>(op, std::move(expr), std::move(right));
    }
    
    return expr;
}

std::unique_ptr<ASTNode> Parser::parseTerm() {
    auto expr = parseFactor();
    
    while (match(TokenType::PLUS) || match(TokenType::MINUS)) {
        Token op = tokens[index - 1];
        auto right = parseFactor();
        expr = std::make_unique<BinaryExpr>(op, std::move(expr), std::move(right));
    }
    
    return expr;
}

std::unique_ptr<ASTNode> Parser::parseFactor() {
    auto expr = parsePrimary();
    
    while (match(TokenType::STAR) || match(TokenType::SLASH)) {
        Token op = tokens[index - 1];
        auto right = parsePrimary();
        expr = std::make_unique<BinaryExpr>(op, std::move(expr), std::move(right));
    }
    
    return expr;
}

std::unique_ptr<ASTNode> Parser::parsePrimary() {
    if (match(TokenType::NUMBER) || match(TokenType::STRING) || 
        match(TokenType::BOOLEAN) || match(TokenType::NULL_TYPE)) {
        return std::make_unique<LiteralExpr>(tokens[index - 1]);
    }
    
    if (match(TokenType::IDENTIFIER)) {
        return std::make_unique<VariableExpr>(tokens[index - 1]);
    }
    
    if (match(TokenType::LEFT_PAREN)) {
        auto expr = parseExpression();
        consume(TokenType::RIGHT_PAREN, "Expect ')' after expression");
        return expr;
    }
    
    throw std::runtime_error("Expect expression");
}

// Statement parsing
std::unique_ptr<Statement> Parser::parseStatement() {
    if (match(TokenType::PRINT)) return parsePrintStatement();
    if (match(TokenType::VAR)) return parseVarDeclaration();
    if (match(TokenType::LEFT_BRACE)) return parseBlock();
    if (match(TokenType::IF)) return parseIfStatement();
    if (match(TokenType::WHILE)) return parseWhileStatement();
    if (match(TokenType::FOR)) return parseForStatement();
    return parseExpressionStatement();
}

std::unique_ptr<Statement> Parser::parseExpressionStatement() {
    auto expr = parseExpression();
    consume(TokenType::SEMICOLON, "Expect ';' after expression");
    return std::make_unique<ExpressionStmt>(std::move(expr));
}

std::unique_ptr<Statement> Parser::parseVarDeclaration() {
    Token name = consume(TokenType::IDENTIFIER, "Expect variable name");
    std::unique_ptr<ASTNode> initializer;

    if (match(TokenType::EQUAL)) {
        initializer = parseExpression();
    } else {
        initializer = std::make_unique<LiteralExpr>(Token{TokenType::NULL_TYPE, "null"});
    }

    consume(TokenType::SEMICOLON, "Expect ';' after variable declaration");
    return std::make_unique<VarDeclStmt>(name, std::move(initializer));
}

std::unique_ptr<Statement> Parser::parseBlock() {
    std::vector<std::unique_ptr<Statement>> statements;
    
    while (!check(TokenType::RIGHT_BRACE) && !check(TokenType::EOF_TYPE)) {
        statements.push_back(parseStatement());
    }
    
    consume(TokenType::RIGHT_BRACE, "Expect '}' after block");
    return std::make_unique<BlockStmt>(std::move(statements));
}

std::unique_ptr<Statement> Parser::parseIfStatement() {
    consume(TokenType::LEFT_PAREN, "Expect '(' after 'if'");
    auto condition = parseExpression();
    consume(TokenType::RIGHT_PAREN, "Expect ')' after if condition");
    
    auto thenBranch = parseStatement();
    std::unique_ptr<Statement> elseBranch = nullptr;
    
    if (match(TokenType::ELSE)) {
        elseBranch = parseStatement();
    }
    
    return std::make_unique<IfStmt>(std::move(condition), std::move(thenBranch), std::move(elseBranch));
}

std::unique_ptr<Statement> Parser::parseWhileStatement() {
    consume(TokenType::LEFT_PAREN, "Expect '(' after 'while'");
    auto condition = parseExpression();
    consume(TokenType::RIGHT_PAREN, "Expect ')' after while condition");
    
    auto body = parseStatement();
    return std::make_unique<WhileStmt>(std::move(condition), std::move(body));
}

std::unique_ptr<Statement> Parser::parsePrintStatement() {
    consume(TokenType::LEFT_PAREN, "Expect '(' after 'print'");
    auto expr = parseExpression();
    consume(TokenType::RIGHT_PAREN, "Expect ')' after print expression");
    consume(TokenType::SEMICOLON, "Expect ';' after print statement");
    return std::make_unique<PrintStmt>(std::move(expr));
}

std::unique_ptr<Statement> Parser::parseForStatement() {
    consume(TokenType::LEFT_PAREN, "Expect '(' after 'for'");
    
    // Initialization
    std::unique_ptr<Statement> initializer;
    if (match(TokenType::VAR)) {
        initializer = parseVarDeclaration();
    } else if (match(TokenType::SEMICOLON)) {
        // No initializer
    } else {
        initializer = parseExpressionStatement();
    }
    
    // Condition
    std::unique_ptr<ASTNode> condition;
    if (!check(TokenType::SEMICOLON)) {
        condition = parseExpression();
    } else {
        // No condition means infinite loop
        condition = std::make_unique<LiteralExpr>(Token{TokenType::BOOLEAN, "true"});
    }
    consume(TokenType::SEMICOLON, "Expect ';' after loop condition");
    
    // Increment
    std::unique_ptr<Statement> increment;
    if (!check(TokenType::RIGHT_PAREN)) {
        auto expr = parseExpression();
        increment = std::make_unique<ExpressionStmt>(std::move(expr));
    } else {
        // No increment
        increment = std::make_unique<ExpressionStmt>(std::make_unique<LiteralExpr>(Token{TokenType::NUMBER, "0"}));
    }
    consume(TokenType::RIGHT_PAREN, "Expect ')' after for clauses");
    
    // Body
    auto body = parseStatement();
    
    // Create a block that contains:
    // 1. Initializer
    // 2. While loop with condition and body
    // 3. Increment at the end of the body
    std::vector<std::unique_ptr<Statement>> statements;
    if (initializer) {
        statements.push_back(std::move(initializer));
    }
    
    // Create a block for the body that includes the increment
    std::vector<std::unique_ptr<Statement>> bodyStatements;
    bodyStatements.push_back(std::move(body));
    bodyStatements.push_back(std::move(increment));
    auto bodyBlock = std::make_unique<BlockStmt>(std::move(bodyStatements));
    
    // Create the while loop
    auto whileLoop = std::make_unique<WhileStmt>(std::move(condition), std::move(bodyBlock));
    statements.push_back(std::move(whileLoop));
    
    return std::make_unique<BlockStmt>(std::move(statements));
}

// Main parsing method
std::vector<std::unique_ptr<Statement>> Parser::parse() {
    std::vector<std::unique_ptr<Statement>> statements;
    
    while (!check(TokenType::EOF_TYPE)) {
        statements.push_back(parseStatement());
    }
    
    return statements;
}

