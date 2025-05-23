#include "vm.h"
#include <iostream>
#include <stdexcept>
#include <variant>

VirtualMachine::VirtualMachine() : pc(0) {}

void VirtualMachine::execute(const BytecodeProgram& program) {
    stack.clear();
    variables.clear();
    variables.resize(10);  // Pre-allocate space for variables
    pc = 0;
    currentProgram = program;  // Store the program
    
    while (pc < currentProgram.instructions.size()) {
        const auto& instr = currentProgram.instructions[pc];
        bool shouldIncrementPc = true;  // By default, increment pc
        
        try {
            // Execute instruction
            switch (instr.op) {
                case OpCode::PUSH:
                    handlePush(instr);
                    break;
                case OpCode::POP:
                    handlePop();
                    break;
                case OpCode::STORE:
                    handleStore(instr);
                    break;
                case OpCode::LOAD:
                    handleLoad(instr);
                    break;
                case OpCode::ADD:
                    handleAdd();
                    break;
                case OpCode::SUB:
                    handleSub();
                    break;
                case OpCode::MUL:
                    handleMul();
                    break;
                case OpCode::DIV:
                    handleDiv();
                    break;
                case OpCode::CMP_GT:
                case OpCode::CMP_LT:
                case OpCode::CMP_EQ:
                case OpCode::CMP_NE:
                case OpCode::CMP_LE:
                case OpCode::CMP_GE:
                    handleCmp(instr.op);
                    break;
                case OpCode::JMP:
                    handleJmp(instr);
                    shouldIncrementPc = false;  // pc is set in handleJmp
                    break;
                case OpCode::JMP_IF_FALSE:
                    shouldIncrementPc = !handleJmpIfFalse();  // Only increment if we didn't jump
                    break;
                case OpCode::PRINT:
                    handlePrint();
                    break;
                case OpCode::HALT:
                    handleHalt();
                    return;  // Exit the method
            }
            
            if (shouldIncrementPc) {
                pc++;
            }
            
        } catch (const std::exception& e) {
            std::cerr << "Runtime error at PC " << pc << ": " << e.what() << std::endl;
            return;
        }
    }
}

void VirtualMachine::push(Value value) {
    stack.push_back(value);
}

Value VirtualMachine::pop() {
    if (stack.empty()) {
        return 0;
    }
    Value value = stack.back();
    stack.pop_back();
    return value;
}

Value VirtualMachine::peek() {
    if (stack.empty()) {
        runtimeError("Stack underflow");
    }
    return stack.back();
}

Value VirtualMachine::convertToNumber(const Value& value) {
    if (std::holds_alternative<int>(value)) {
        return value;
    }
    if (std::holds_alternative<double>(value)) {
        return value;
    }
    if (std::holds_alternative<std::string>(value)) {
        try {
            std::string str = std::get<std::string>(value);
            if (str.find('.') != std::string::npos) {
                return std::stod(str);
            } else {
                return std::stoi(str);
            }
        } catch (...) {
            runtimeError("Invalid number format");
        }
    }
    if (std::holds_alternative<bool>(value)) {
        return std::get<bool>(value) ? 1 : 0;
    }
    runtimeError("Cannot convert value to number");
    return 0;  // Never reached
}

void VirtualMachine::handlePush(const Instruction& instr) {
    push(instr.operand);  // Push the operand directly, no need for conversion
}

void VirtualMachine::handlePop() {
    if (!stack.empty()) {
        stack.pop_back();
    }
}

void VirtualMachine::handleStore(const Instruction& instr) {
    int index = std::get<int>(instr.operand);
    Value value = pop();  // Get the value to store
    variables[index] = value;
    // Don't push anything back - store is a statement, not an expression
}

void VirtualMachine::handleLoad(const Instruction& instr) {
    int index = std::get<int>(instr.operand);
    if (index >= variables.size()) {
        runtimeError("Variable not initialized");
    }
    push(variables[index]);
}

void VirtualMachine::handleAdd() {
    Value b = pop();
    Value a = pop();
    
    // If either operand is a string, do string concatenation
    if (std::holds_alternative<std::string>(a) || std::holds_alternative<std::string>(b)) {
        std::string result;
        if (std::holds_alternative<std::string>(a)) {
            result = std::get<std::string>(a);
        } else if (std::holds_alternative<int>(a)) {
            result = std::to_string(std::get<int>(a));
        } else if (std::holds_alternative<double>(a)) {
            result = std::to_string(std::get<double>(a));
        } else if (std::holds_alternative<bool>(a)) {
            result = std::get<bool>(a) ? "true" : "false";
        }
        
        if (std::holds_alternative<std::string>(b)) {
            result += std::get<std::string>(b);
        } else if (std::holds_alternative<int>(b)) {
            result += std::to_string(std::get<int>(b));
        } else if (std::holds_alternative<double>(b)) {
            result += std::to_string(std::get<double>(b));
        } else if (std::holds_alternative<bool>(b)) {
            result += std::get<bool>(b) ? "true" : "false";
        }
        
        push(result);
        return;
    }
    
    // Otherwise, do numeric addition
    Value numA = convertToNumber(a);
    Value numB = convertToNumber(b);
    
    if (std::holds_alternative<int>(numA) && std::holds_alternative<int>(numB)) {
        push(std::get<int>(numA) + std::get<int>(numB));
    } else {
        double da = std::holds_alternative<int>(numA) ? std::get<int>(numA) : std::get<double>(numA);
        double db = std::holds_alternative<int>(numB) ? std::get<int>(numB) : std::get<double>(numB);
        push(da + db);
    }
}

void VirtualMachine::handleSub() {
    Value b = convertToNumber(pop());
    Value a = convertToNumber(pop());
    
    if (std::holds_alternative<int>(a) && std::holds_alternative<int>(b)) {
        push(std::get<int>(a) - std::get<int>(b));
    } else {
        double da = std::holds_alternative<int>(a) ? std::get<int>(a) : std::get<double>(a);
        double db = std::holds_alternative<int>(b) ? std::get<int>(b) : std::get<double>(b);
        push(da - db);
    }
}

void VirtualMachine::handleMul() {
    Value b = convertToNumber(pop());
    Value a = convertToNumber(pop());
    
    if (std::holds_alternative<int>(a) && std::holds_alternative<int>(b)) {
        push(std::get<int>(a) * std::get<int>(b));
    } else {
        double da = std::holds_alternative<int>(a) ? std::get<int>(a) : std::get<double>(a);
        double db = std::holds_alternative<int>(b) ? std::get<int>(b) : std::get<double>(b);
        push(da * db);
    }
}

void VirtualMachine::handleDiv() {
    Value b = convertToNumber(pop());
    Value a = convertToNumber(pop());
    
    if (std::holds_alternative<int>(a) && std::holds_alternative<int>(b)) {
        int ib = std::get<int>(b);
        if (ib == 0) runtimeError("Division by zero");
        push(std::get<int>(a) / ib);
    } else {
        double da = std::holds_alternative<int>(a) ? std::get<int>(a) : std::get<double>(a);
        double db = std::holds_alternative<int>(b) ? std::get<int>(b) : std::get<double>(b);
        if (db == 0) runtimeError("Division by zero");
        push(da / db);
    }
}

void VirtualMachine::handleCmp(OpCode op) {
    Value b = pop();
    Value a = pop();
    bool result = false;
    
    // If both operands are strings, do string comparison
    if (std::holds_alternative<std::string>(a) && std::holds_alternative<std::string>(b)) {
        std::string sa = std::get<std::string>(a);
        std::string sb = std::get<std::string>(b);
        switch (op) {
            case OpCode::CMP_EQ: result = (sa == sb); break;
            case OpCode::CMP_NE: result = (sa != sb); break;
            case OpCode::CMP_LT: result = (sa < sb); break;
            case OpCode::CMP_LE: result = (sa <= sb); break;
            case OpCode::CMP_GT: result = (sa > sb); break;
            case OpCode::CMP_GE: result = (sa >= sb); break;
            default: runtimeError("Invalid comparison operator");
        }
    } else {
        // Otherwise, do numeric comparison
        Value numA = convertToNumber(a);
        Value numB = convertToNumber(b);
        
        double da = std::holds_alternative<int>(numA) ? std::get<int>(numA) : std::get<double>(numA);
        double db = std::holds_alternative<int>(numB) ? std::get<int>(numB) : std::get<double>(numB);
        
        switch (op) {
            case OpCode::CMP_EQ: result = (da == db); break;
            case OpCode::CMP_NE: result = (da != db); break;
            case OpCode::CMP_LT: result = (da < db); break;
            case OpCode::CMP_LE: result = (da <= db); break;
            case OpCode::CMP_GT: result = (da > db); break;
            case OpCode::CMP_GE: result = (da >= db); break;
            default: runtimeError("Invalid comparison operator");
        }
    }
    
    push(result);
}

void VirtualMachine::handleJmp(const Instruction& instr) {
    int target = std::get<int>(instr.operand);
    pc = target;
}

bool VirtualMachine::handleJmpIfFalse() {
    if (stack.empty()) {
        throw std::runtime_error("Stack underflow in JMP_IF_FALSE");
    }
    
    Value condition = stack.back();
    stack.pop_back();
    
    bool isFalse = !isTruthy(condition);
    
    if (isFalse) {
        // Jump to the target address
        pc = std::get<int>(currentProgram.instructions[pc].operand);
        return true;
    }
    
    // If condition is true, just move to next instruction
    pc++;
    return false;
}

void VirtualMachine::handlePrint() {
    if (stack.empty()) {
        runtimeError("Stack underflow in print");
    }
    Value value = pop();
    if (std::holds_alternative<int>(value)) {
        std::cout << std::get<int>(value) << "\n";
    } else if (std::holds_alternative<double>(value)) {
        std::cout << std::get<double>(value) << "\n";
    } else if (std::holds_alternative<bool>(value)) {
        std::cout << (std::get<bool>(value) ? "true" : "false") << "\n";
    } else if (std::holds_alternative<std::string>(value)) {
        std::cout << std::get<std::string>(value) << "\n";
    }
}

bool VirtualMachine::isTruthy(const Value& value) {
    if (std::holds_alternative<bool>(value)) {
        return std::get<bool>(value);
    }
    if (std::holds_alternative<int>(value)) {
        return std::get<int>(value) != 0;
    }
    if (std::holds_alternative<double>(value)) {
        return std::get<double>(value) != 0.0;
    }
    if (std::holds_alternative<std::string>(value)) {
        return !std::get<std::string>(value).empty();
    }
    return false;
}

void VirtualMachine::runtimeError(const std::string& message) {
    throw std::runtime_error("Runtime error: " + message);
}

void VirtualMachine::handleHalt() {
    // Do nothing, execution will stop after this instruction
} 