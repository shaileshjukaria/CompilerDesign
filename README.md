# 🛠️ MyCompiler – A Simple Compiler Project in Python

This project is a simple compiler built using Python and PLY (Python Lex-Yacc). It performs lexical analysis (tokenization), syntax analysis (parsing), and basic error handling for a simple custom language.

> 🚀 Originally based on an open-source template, customized and maintained by **SHAILESH JUKARIA**.

---

## 📦 Features

- Lexical Analysis (Token Generation)
- Syntax Parsing (Using CFG)
- Error Detection
- Modular Code Structure

---

## 📁 Project Structure

```plaintext
├── lexer.py        # Contains lexical rules using PLY (Lex)
├── parser.py       # Contains parsing rules using PLY (Yacc)
├── main.py         # Entry point to run the compiler
├── input.txt       # Source code file to be compiled
├── README.md       # Project documentation


```
🚀 How to Run the Project
✅ Prerequisites
Python 3.x installed

ply library

🔧 Install Dependencies
Open terminal (or Git Bash) and run:
```plaintext
pip install ply
```
▶️ Run the Compiler
Make sure your source code is written inside input.txt.
Then, run the compiler using:
```plaintext
python main.py
```
The program will:
Read the code from input.txt
Tokenize and parse it
Print the result and any syntax/lexical errors in the terminal.

🧑‍💻 Author
This version is customized and maintained by:

SHAILESH JUKARIA
[github.com/shaileshjukaria](https://github.com/shaileshjukaria)




