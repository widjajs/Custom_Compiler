#include "../includes/error.hpp"
#include "../includes/interpreter.hpp"
#include "../includes/parser.hpp"
#include "../includes/tokenizer.hpp"

#include <fstream>
#include <iostream>
#include <sstream>

void runFile(std::string path) {
    std::string contents;
    std::stringstream contents_stream;
    std::fstream input(path, std::ios::in);
    contents_stream << input.rdbuf();
    contents = contents_stream.str();

    // std::cout << contents.length() << std::endl;
    // std::cout << contents << std::endl;

    Tokenizer tokenizer(contents);
    std::vector<Token> tokens = tokenizer.scan_tokens();

    for (Token token : tokens) {
        std::cout << token.to_string() << std::endl;
    }

    Parser parser(tokens);
    Expr expression = parser.parse();

    if (!parser.had_error) {
        std::cout << "YAY" << std::endl;
    }

    Interpreter interpreter;
    object_literal result = interpreter.evaluate(expression);

    std::visit(overloaded{[](double d) { std::cout << d; },
                          [](const std::string &s) { std::cout << s; },
                          [](bool b) { std::cout << (b ? "true" : "false"); },
                          [](std::monostate) { std::cout << "null"; }},
               result);

    std::cout << std::endl;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        report_file("No File Provided");
        return 1;
    }
    if (argc > 2) {
        report_args("Too Many Arguments", argc - 2);
        return 1;
    }
    if (argc == 2) {
        std::ifstream file(argv[1]);
        if (!file.is_open()) {
            report_file("File Cannot Be Opened");
            return 1;
        }
        runFile(argv[1]);
    }
    return 0;
}
