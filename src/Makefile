# Compiler
CXX = g++
CXXFLAGS = -std=c++17
TARGET = compii

# Directories
SRC_DIR = .
LEXER_DIR = lexer
PARSER_DIR = parser
AST_DIR = ast
CODEGEN_DIR = codegen

# Source files
SRCS = main.cpp \
       lexer/lexer.cpp \
       parser/parser.cpp \
       codegen/codegen.cpp \
       codegen/vm.cpp
OBJS = $(SRCS:.cpp=.o)

# Output
OUT = compii

# Build target
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean
clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
