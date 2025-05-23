#pragma once

#include "bytecode.h"
#include <vector>
#include <stack>
#include <unordered_map>
#include <variant>

class VirtualMachine {
public:
    VirtualMachine();
    
    // Execute a bytecode program
    void execute(const BytecodeProgram& program);
    
private:
    // Execution state
    std::vector<Value> stack;
    std::vector<Value> variables;
    size_t pc;  // Program counter
    BytecodeProgram currentProgram;  // Current program being executed
    
    // Helper methods
    void push(Value value);
    Value pop();
    Value peek();
    Value convertToNumber(const Value& value);
    
    // Instruction handlers
    void handlePush(const Instruction& instr);
    void handlePop();
    void handleStore(const Instruction& instr);
    void handleLoad(const Instruction& instr);
    void handleAdd();
    void handleSub();
    void handleMul();
    void handleDiv();
    void handleCmp(OpCode op);
    void handleJmp(const Instruction& instr);
    bool handleJmpIfFalse();  // Returns true if we jumped
    void handlePrint();
    void handleHalt();
    
    // Utility methods
    bool isTruthy(const Value& value);
    void runtimeError(const std::string& message);
}; 