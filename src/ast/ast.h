#ifndef AST_H
#define AST_H

#include <memory>
#include <vector>
#include <iostream>
#include "../lexer/lexer.h"

struct ASTNode {
    virtual ~ASTNode() = default;
    virtual void print(std::ostream& out) const = 0;
};

// Expressions
struct LiteralExpr : public ASTNode {
    Token token;
    LiteralExpr(Token token) : token(token) {}
    void print(std::ostream& out) const override {
        out << token.value;
    }
};

struct BinaryExpr : public ASTNode {
    Token op;
    std::unique_ptr<ASTNode> left, right;

    BinaryExpr(Token op, std::unique_ptr<ASTNode> left, std::unique_ptr<ASTNode> right)
        : op(op), left(std::move(left)), right(std::move(right)) {}
    
    void print(std::ostream& out) const override {
        out << "(";
        left->print(out);
        out << " " << op.value << " ";
        right->print(out);
        out << ")";
    }
};

struct VariableExpr : public ASTNode {
    Token name;
    VariableExpr(Token name) : name(name) {}
    void print(std::ostream& out) const override {
        out << name.value;
    }
};

struct AssignmentExpr : public ASTNode {
    Token name;
    std::unique_ptr<ASTNode> value;
    AssignmentExpr(Token name, std::unique_ptr<ASTNode> value)
        : name(name), value(std::move(value)) {}
    
    void print(std::ostream& out) const override {
        out << name.value << " = ";
        value->print(out);
    }
};

// Statements
struct Statement : public ASTNode {};

struct ExpressionStmt : public Statement {
    std::unique_ptr<ASTNode> expression;
    ExpressionStmt(std::unique_ptr<ASTNode> expression)
        : expression(std::move(expression)) {}
    
    void print(std::ostream& out) const override {
        expression->print(out);
        out << ";";
    }
};

struct PrintStmt : public Statement {
    std::unique_ptr<ASTNode> expression;
    PrintStmt(std::unique_ptr<ASTNode> expression)
        : expression(std::move(expression)) {}
    
    void print(std::ostream& out) const override {
        out << "print ";
        expression->print(out);
        out << ";";
    }
};

struct VarDeclStmt : public Statement {
    Token name;
    std::unique_ptr<ASTNode> initializer;
    VarDeclStmt(Token name, std::unique_ptr<ASTNode> initializer)
        : name(name), initializer(std::move(initializer)) {}
    
    void print(std::ostream& out) const override {
        out << "var " << name.value << " = ";
        initializer->print(out);
        out << ";";
    }
};

struct BlockStmt : public Statement {
    std::vector<std::unique_ptr<Statement>> statements;
    BlockStmt(std::vector<std::unique_ptr<Statement>> statements)
        : statements(std::move(statements)) {}
    
    void print(std::ostream& out) const override {
        out << "{\n";
        for (const auto& stmt : statements) {
            out << "  ";
            stmt->print(out);
            out << "\n";
        }
        out << "}";
    }
};

struct IfStmt : public Statement {
    std::unique_ptr<ASTNode> condition;
    std::unique_ptr<Statement> thenBranch;
    std::unique_ptr<Statement> elseBranch;
    IfStmt(std::unique_ptr<ASTNode> condition,
           std::unique_ptr<Statement> thenBranch,
           std::unique_ptr<Statement> elseBranch)
        : condition(std::move(condition)),
          thenBranch(std::move(thenBranch)),
          elseBranch(std::move(elseBranch)) {}
    
    void print(std::ostream& out) const override {
        out << "if (";
        condition->print(out);
        out << ") ";
        thenBranch->print(out);
        if (elseBranch) {
            out << " else ";
            elseBranch->print(out);
        }
    }
};

struct WhileStmt : public Statement {
    std::unique_ptr<ASTNode> condition;
    std::unique_ptr<Statement> body;
    WhileStmt(std::unique_ptr<ASTNode> condition, std::unique_ptr<Statement> body)
        : condition(std::move(condition)), body(std::move(body)) {}
    
    void print(std::ostream& out) const override {
        out << "while (";
        condition->print(out);
        out << ") ";
        body->print(out);
    }
};

#endif