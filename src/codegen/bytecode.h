#ifndef BYTECODE_H
#define BYTECODE_H

#include <vector>
#include <string>
#include <variant>
#include <unordered_map>

// Bytecode instruction types
enum class OpCode {
    // Stack operations
    PUSH,       // Push value onto stack
    POP,        // Pop value from stack
    
    // Variable operations
    STORE,      // Store top of stack in variable
    LOAD,       // Load variable onto stack
    
    // Arithmetic operations
    ADD,        // Add top two values
    SUB,        // Subtract top two values
    MUL,        // Multiply top two values
    DIV,        // Divide top two values
    
    // Comparison operations
    CMP_EQ,     // Equal
    CMP_NE,     // Not equal
    CMP_LT,     // Less than
    CMP_LE,     // Less than or equal
    CMP_GT,     // Greater than
    CMP_GE,     // Greater than or equal
    
    // Control flow
    JMP,        // Unconditional jump
    JMP_IF_FALSE, // Jump if top of stack is false
    
    // I/O
    PRINT,      // Print top of stack
    
    // Program control
    HALT        // Stop execution
};

// Value types that can be stored in bytecode
using Value = std::variant<int, double, bool, std::string>;

// A single bytecode instruction
struct Instruction {
    OpCode op;
    Value operand;  // Optional operand value
    
    Instruction(OpCode op) : op(op), operand(0) {}
    Instruction(OpCode op, Value operand) : op(op), operand(operand) {}
};

// A complete bytecode program
struct BytecodeProgram {
    std::vector<Instruction> instructions;
    std::unordered_map<std::string, size_t> labels;  // For jump targets
};

#endif 