#include "../includes/interpreter.hpp"

#include <variant>

object_literal Interpreter::evaluate(Expr &expr) {
    return std::visit(*this, expr);
}
