#ifndef CODEGEN_H
#define CODEGEN_H

#include "../ast/ast.h"
#include "bytecode.h"
#include <unordered_map>
#include <stack>
#include <string>

class CodeGenerator {
public:
    CodeGenerator();
    
    // Generate bytecode from AST
    BytecodeProgram generate(ASTNode* ast);
    
private:
    // Current bytecode program being generated
    BytecodeProgram program;
    
    // Symbol table for variables
    std::unordered_map<std::string, size_t> variables;
    
    // Stack of scopes for nested blocks
    std::stack<std::unordered_map<std::string, size_t>> scopes;
    
    // Helper methods for code generation
    void generateExpr(ASTNode* expr);
    void generateStmt(Statement* stmt);
    
    // Specific generation methods
    void generateLiteral(LiteralExpr* expr);
    void generateBinary(BinaryExpr* expr);
    void generateVariable(VariableExpr* expr);
    void generateAssignment(AssignmentExpr* expr);
    void generateVarDecl(VarDeclStmt* stmt);
    void generateIf(IfStmt* stmt);
    void generateWhile(WhileStmt* stmt);
    void generateBlock(BlockStmt* stmt);
    void generatePrint(PrintStmt* stmt);
    
    // Utility methods
    void emit(OpCode op);
    void emit(OpCode op, Value operand);
    size_t getVariableIndex(const std::string& name);
    void enterScope();
    void exitScope();
};

#endif 