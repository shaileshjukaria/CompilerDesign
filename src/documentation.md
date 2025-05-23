# Compii Language Implementation

## Overview
Compii is a simple programming language implemented in C++. The implementation consists of four main components:

1. Lexer (Token Generation)
2. Parser (AST Generation)
3. Code Generator (Bytecode Generation)
4. Virtual Machine (Execution)

## Component Details

### 1. Lexer (`lexer/lexer.cpp`)
- Converts source code into tokens
- Handles:
  - Keywords (var, if, while, print)
  - Identifiers
  - Literals (numbers, strings)
  - Operators (+, -, *, /, ==, !=, etc.)
  - Whitespace and comments

### 2. Parser (`parser/parser.cpp`)
- Converts tokens into Abstract Syntax Tree (AST)
- Implements recursive descent parsing
- Handles:
  - Variable declarations
  - Expressions
  - Control flow (if, while)
  - Print statements

### 3. Code Generator (`codegen/codegen.cpp`)
- Converts AST into bytecode
- Manages:
  - Variable scopes
  - Jump targets
  - Instruction generation

### 4. Virtual Machine (`codegen/vm.cpp`)
- Executes bytecode
- Features:
  - Stack-based execution
  - Variable storage
  - Type handling
  - Error handling

## Bytecode Instructions

### Stack Operations
- `PUSH`: Push value onto stack
- `POP`: Pop value from stack

### Variable Operations
- `STORE`: Store value in variable
- `LOAD`: Load value from variable

### Arithmetic Operations
- `ADD`: Addition
- `SUB`: Subtraction
- `MUL`: Multiplication
- `DIV`: Division

### Comparison Operations
- `CMP_EQ`: Equal
- `CMP_NE`: Not equal
- `CMP_LT`: Less than
- `CMP_LE`: Less than or equal
- `CMP_GT`: Greater than
- `CMP_GE`: Greater than or equal

### Control Flow
- `JMP`: Unconditional jump
- `JMP_IF_FALSE`: Conditional jump

### I/O
- `PRINT`: Print value

### Program Control
- `HALT`: Stop execution

## Example Program Flow

1. Source Code:
```cpp
var x = 5;
var y = 10;
var sum = x + y;
print(sum);
```

2. Tokens:
```
[VAR] [IDENTIFIER "x"] [=] [NUMBER 5] [;]
[VAR] [IDENTIFIER "y"] [=] [NUMBER 10] [;]
[VAR] [IDENTIFIER "sum"] [=] [IDENTIFIER "x"] [+] [IDENTIFIER "y"] [;]
[PRINT] [IDENTIFIER "sum"] [;]
```

3. AST:
```
Program
├── VarDecl (x = 5)
├── VarDecl (y = 10)
├── VarDecl (sum = BinaryExpr(x + y))
└── PrintStmt (sum)
```

4. Bytecode:
```
PUSH 5
STORE 0    ; x
PUSH 10
STORE 1    ; y
LOAD 0     ; x
LOAD 1     ; y
ADD
STORE 2    ; sum
LOAD 2     ; sum
PRINT
HALT
```

## Building and Running

1. Compile:
```bash
make
```

2. Run:
```bash
./compii
```

## Error Handling

The implementation includes error handling at multiple levels:

1. Lexer: Invalid tokens
2. Parser: Syntax errors
3. VM: Runtime errors (division by zero, etc.)

## Future Improvements

1. Add more language features:
   - Functions
   - Arrays
   - More data types

2. Performance optimizations:
   - Bytecode optimization
   - Better error messages

3. Tooling:
   - Debugger
   - Better error reporting 