# Compiler Design Project

A simple compiler built using Lex and Yacc that demonstrates key phases of a compiler pipeline, including lexical analysis, parsing, syntax tree generation, and intermediate code output. This project was developed as part of a university Compiler Design course.

---

## 🚀 Features

- Lexical Analysis using **Lex/Flex**
- Syntax Parsing using **Yacc/Bison**
- Grammar for a custom programming language (or subset of C)
- Generation of a **Parse Tree** and **Intermediate Representation**
- Error detection and reporting
- Simple command-line interface for input/output

---

## 🧱 Compiler Architecture

1. **Lexical Analyzer (`lex.l`)**
   - Tokenizes the source code into identifiers, literals, operators, and keywords.

2. **Syntax Analyzer (`parser.y`)**
   - Parses the stream of tokens to check for syntactic correctness.
   - Implements grammar rules and generates parse trees.

3. **Semantic Analysis (basic)**
   - Type checking and symbol validation (optional in basic versions).

4. **Intermediate Code Generation**
   - Outputs intermediate code (e.g., 3-address code).

5. **(Optional) Code Optimization and Final Code Generation**

---

## 🛠️ Getting Started

### Prerequisites
- `flex`
- `bison` or `yacc`
- `gcc`

### Build & Run

```bash
# Clone the repository
git clone https://github.com/KanyalMohit/Compiler.git
cd Compiler

# Compile using Flex and Bison
flex lex.l
bison -d parser.y
gcc lex.yy.c parser.tab.c -o compiler

# Run the compiler with input file
./compiler < input.txt
