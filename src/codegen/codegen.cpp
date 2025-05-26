#include "codegen.h"
#include <stdexcept>

CodeGenerator::CodeGenerator() {
    enterScope(); // Start with global scope
}

BytecodeProgram CodeGenerator::generate(ASTNode* ast) {
    program = BytecodeProgram(); // Reset program
    
    // Handle multiple statements
    if (auto* block = dynamic_cast<BlockStmt*>(ast)) {
        generateBlock(block);
    } else {
        generateStmt(static_cast<Statement*>(ast));
    }
    
    emit(OpCode::HALT); // End program
    return program;
}

void CodeGenerator::generateExpr(ASTNode* expr) {
    if (auto* literal = dynamic_cast<LiteralExpr*>(expr)) {
        generateLiteral(literal);
    } else if (auto* binary = dynamic_cast<BinaryExpr*>(expr)) {
        generateBinary(binary);
    } else if (auto* variable = dynamic_cast<VariableExpr*>(expr)) {
        generateVariable(variable);
    } else if (auto* assignment = dynamic_cast<AssignmentExpr*>(expr)) {
        generateAssignment(assignment);
    }
}

void CodeGenerator::generateStmt(Statement* stmt) {
    if (auto* exprStmt = dynamic_cast<ExpressionStmt*>(stmt)) {
        generateExpr(exprStmt->expression.get());
        emit(OpCode::POP); // Discard result
    } else if (auto* varDecl = dynamic_cast<VarDeclStmt*>(stmt)) {
        generateVarDecl(varDecl);
    } else if (auto* ifStmt = dynamic_cast<IfStmt*>(stmt)) {
        generateIf(ifStmt);
    } else if (auto* whileStmt = dynamic_cast<WhileStmt*>(stmt)) {
        generateWhile(whileStmt);
    } else if (auto* block = dynamic_cast<BlockStmt*>(stmt)) {
        generateBlock(block);
    } else if (auto* print = dynamic_cast<PrintStmt*>(stmt)) {
        generatePrint(print);
        emit(OpCode::POP); // Pop after print statement
    }
}

void CodeGenerator::generateLiteral(LiteralExpr* expr) {
    if (expr->token.type == TokenType::NUMBER) {
        // Convert string to number
        try {
            if (expr->token.value.find('.') != std::string::npos) {
                emit(OpCode::PUSH, std::stod(expr->token.value));
            } else {
                emit(OpCode::PUSH, std::stoi(expr->token.value));
            }
        } catch (...) {
            throw std::runtime_error("Invalid number literal: " + expr->token.value);
        }
    } else {
        emit(OpCode::PUSH, expr->token.value);
    }
}

void CodeGenerator::generateBinary(BinaryExpr* expr) {
    generateExpr(expr->left.get());
    generateExpr(expr->right.get());
    
    switch (expr->op.type) {
        case TokenType::PLUS:
            emit(OpCode::ADD);
            break;
        case TokenType::MINUS:
            emit(OpCode::SUB);
            break;
        case TokenType::STAR:
            emit(OpCode::MUL);
            break;
        case TokenType::SLASH:
            emit(OpCode::DIV);
            break;
        case TokenType::EQUAL_EQUAL:
            emit(OpCode::CMP_EQ);
            break;
        case TokenType::BANG_EQUAL:
            emit(OpCode::CMP_NE);
            break;
        case TokenType::GREATER:
            emit(OpCode::CMP_GT);
            break;
        case TokenType::GREATER_EQUAL:
            emit(OpCode::CMP_GE);
            break;
        case TokenType::LESS:
            emit(OpCode::CMP_LT);
            break;
        case TokenType::LESS_EQUAL:
            emit(OpCode::CMP_LE);
            break;
        default:
            throw std::runtime_error("Unknown binary operator");
    }
}

void CodeGenerator::generateVariable(VariableExpr* expr) {
    size_t index = getVariableIndex(expr->name.value);
    emit(OpCode::LOAD, static_cast<int>(index));
}

void CodeGenerator::generateAssignment(AssignmentExpr* expr) {
    generateExpr(expr->value.get());
    size_t index = getVariableIndex(expr->name.value);
    emit(OpCode::STORE, static_cast<int>(index));
}

void CodeGenerator::generateVarDecl(VarDeclStmt* stmt) {
    if (stmt->initializer) {
        generateExpr(stmt->initializer.get());
    } else {
        emit(OpCode::PUSH, 0); // Default value
    }
    size_t index = getVariableIndex(stmt->name.value);
    emit(OpCode::STORE, static_cast<int>(index));
}

void CodeGenerator::generateIf(IfStmt* stmt) {
    generateExpr(stmt->condition.get());
    
    // Jump if false
    size_t elseJump = program.instructions.size();
    emit(OpCode::JMP_IF_FALSE, 0); // Placeholder for jump target
    emit(OpCode::POP);  // Pop condition after test
    
    // Then branch
    generateStmt(stmt->thenBranch.get());
    
    if (stmt->elseBranch) {
        // Jump over else branch
        size_t endJump = program.instructions.size();
        emit(OpCode::JMP, 0); // Placeholder for jump target
        
        // Update if-false jump
        program.instructions[elseJump].operand = static_cast<int>(program.instructions.size());
        
        // Else branch
        generateStmt(stmt->elseBranch.get());
        
        // Update end jump
        program.instructions[endJump].operand = static_cast<int>(program.instructions.size());
    } else {
        // Update if-false jump
        program.instructions[elseJump].operand = static_cast<int>(program.instructions.size());
    }
}

void CodeGenerator::generateWhile(WhileStmt* stmt) {
    size_t loopStart = program.instructions.size();
    
    // Condition
    generateExpr(stmt->condition.get());
    
    // Jump if false
    size_t exitJump = program.instructions.size();
    emit(OpCode::JMP_IF_FALSE, 0); // Placeholder for jump target
    emit(OpCode::POP);  // Pop condition after test
    
    // Body
    if (auto* block = dynamic_cast<BlockStmt*>(stmt->body.get())) {
        generateBlock(block);
    } else {
        generateStmt(stmt->body.get());
    }
    
    // Jump back to condition
    emit(OpCode::JMP, static_cast<int>(loopStart));
    
    // Update exit jump
    program.instructions[exitJump].operand = static_cast<int>(program.instructions.size());
}

void CodeGenerator::generateBlock(BlockStmt* stmt) {
    enterScope();
    for (auto& statement : stmt->statements) {
        generateStmt(statement.get());
    }
    exitScope();
}

void CodeGenerator::generatePrint(PrintStmt* stmt) {
    generateExpr(stmt->expression.get());
    emit(OpCode::PRINT);
}

void CodeGenerator::emit(OpCode op) {
    program.instructions.emplace_back(op);
}

void CodeGenerator::emit(OpCode op, Value operand) {
    program.instructions.emplace_back(op, operand);
}

size_t CodeGenerator::getVariableIndex(const std::string& name) {
    // Check if variable already exists in global scope
    auto it = variables.find(name);
    if (it != variables.end()) {
        return it->second;
    }
    
    // Add to global scope
    size_t index = variables.size();
    variables[name] = index;
    return index;
}

void CodeGenerator::enterScope() {
    scopes.push({});
}

