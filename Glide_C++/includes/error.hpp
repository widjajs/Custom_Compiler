#pragma once

#include <string>

// TODO add more error messages and types once compiler is more developed
void report_syntax(int, std::string);
void report_syntax(std::string);
void report_file(std::string);
void report_args(std::string, int);
void report_expr(int, std::string msg);
void report_expr(std::string msg);
