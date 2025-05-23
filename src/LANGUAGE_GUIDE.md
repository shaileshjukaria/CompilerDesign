# Compii Language Guide

## Overview
Compii is a simple, statically-typed programming language with a C-like syntax. This guide will help you understand how to write programs in Compii.

## Basic Syntax

### Comments
```compii
// This is a single-line comment

/* This is a
   multi-line comment */
```

### Variables
Variables must be declared before use with the `var` keyword:
```compii
var x = 5;           // Integer
var y = 10.5;        // Float
var name = "Hello";  // String
var flag = true;     // Boolean
var empty = null;    // Null value
```

### Data Types
- `int`: Integer numbers (e.g., 5, -10, 0)
- `float`: Floating-point numbers (e.g., 3.14, -0.5)
- `string`: Text enclosed in double quotes (e.g., "hello")
- `bool`: Boolean values (true or false)
- `null`: Represents the absence of a value

### Arithmetic Operations
```compii
var sum = x + y;      // Addition
var diff = x - y;     // Subtraction
var product = x * y;  // Multiplication
var quotient = x / y; // Division
```

### Comparison Operations
```compii
var isEqual = x == y;        // Equality
var isNotEqual = x != y;     // Inequality
var isGreater = x > y;       // Greater than
var isLess = x < y;          // Less than
var isGreaterEqual = x >= y; // Greater than or equal
var isLessEqual = x <= y;    // Less than or equal
```

### Control Flow

#### If-Else Statements
```compii
if (x > y) {
    print("x is greater than y");
} else {
    print("x is not greater than y");
}
```

#### While Loops
```compii
var i = 0;
while (i < 10) {
    print(i);
    i = i + 1;
}
```

### Print Statement
```compii
print("Hello, World!");  // Print a string
print(x);               // Print a variable
print(x + y);           // Print an expression
```

## Example Programs

### Basic Calculator
```compii
var x = 5;
var y = 10;
var sum = x + y;
var product = x * y;
print("Sum: " + sum);
print("Product: " + product);
```

### Number Guessing Game
```compii
var target = 42;
var guess = 0;
var attempts = 0;

while (guess != target) {
    print("Enter your guess: ");
    // Note: Input functionality would be added in future versions
    guess = 20;  // Example input
    attempts = attempts + 1;
    
    if (guess < target) {
        print("Too low!");
    } else if (guess > target) {
        print("Too high!");
    }
}

print("Congratulations! You found the number in " + attempts + " attempts.");
```

### Temperature Converter
```compii
var celsius = 25;
var fahrenheit = (celsius * 9/5) + 32;
print(celsius + "°C is " + fahrenheit + "°F");
```

## Best Practices

1. **Variable Naming**
   - Use descriptive names
   - Start with lowercase letters
   - Use camelCase for multi-word names
   ```compii
   var userName = "John";
   var itemCount = 5;
   ```

2. **Code Organization**
   - Use whitespace for readability
   - Group related statements together
   - Use comments to explain complex logic

3. **Error Prevention**
   - Always initialize variables
   - Check for division by zero
   - Handle edge cases in loops

## Limitations and Future Features

Current limitations:
- No function definitions
- No arrays or other data structures
- No input handling
- No file I/O

Planned features:
- Function definitions
- Arrays and structures
- Input/output operations
- File handling
- More data types
- Error handling

## Getting Started

1. Write your program in a text editor
2. Save it with a `.compii` extension
3. Compile and run using the Compii compiler
4. Debug using print statements

## Common Errors and Solutions

1. **Missing Semicolon**
   ```compii
   // Error
   var x = 5
   // Correct
   var x = 5;
   ```

2. **Undeclared Variable**
   ```compii
   // Error
   y = 10;
   // Correct
   var y = 10;
   ```

3. **Type Mismatch**
   ```compii
   // Error
   var x = "5" + 10;
   // Correct
   var x = 5 + 10;
   ```

## Conclusion

This guide covers the basics of writing programs in Compii. As the language evolves, this documentation will be updated with new features and best practices. Happy coding! 