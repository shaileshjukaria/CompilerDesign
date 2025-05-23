#include <iostream>
#include <fstream>
#include <sstream>
#include "lexer/lexer.h"
#include "parser/parser.h"
#include "codegen/codegen.h"
#include "codegen/vm.h"

int main(int argc, char* argv[]) {
    try {
        if (argc != 2) {
            std::cerr << "Usage: " << argv[0] << " <input_file>" << std::endl;
            return 1;
        }

        // Read input from the file specified in command line argument
        std::ifstream file(argv[1]);
        if (!file.is_open()) {
            std::cerr << "Error: Could not open " << argv[1] << std::endl;
            return 1;
        }
        
        std::stringstream buffer;
        buffer << file.rdbuf();
        std::string input = buffer.str();
        file.close();

        // Lexing
        Lexer lexer(input);
        auto tokens = lexer.tokenize();

        // Parsing
        Parser parser(tokens);
        auto statements = parser.parse();

        // Code Generation
        CodeGenerator generator;
        auto block = std::make_unique<BlockStmt>(std::move(statements));
        auto program = generator.generate(block.get());

        // Execution
        VirtualMachine vm;
        vm.execute(program);

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
