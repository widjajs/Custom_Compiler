#include "../includes/error.hpp"

#include <iostream>

void report_syntax(int line, std::string msg) {
    std::cerr << "Syntax Error on line " << line << ": " << msg << std::endl;
}

void report_file(std::string msg) {
    std::cerr << msg << std::endl;
}

void report_args(std::string msg, int extra_args) {
    std::cerr << extra_args << " " << msg << std::endl;
}

void report_expr(int line, std::string msg) {
    std::cerr << "Expression Error on line " << line << ": " << msg
              << std::endl;
}

void report_expr(std::string msg) {
    std::cerr << "Expression Error at end: " << msg << std::endl;
}

void report_type_mismatch(std::string msg) {
}
