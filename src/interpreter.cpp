#include "../includes/interpreter.hpp"
#include "../includes/error.hpp"

#include <variant>

// overloaded helper
template <class... Ts> // ... means accept any number of types
struct overloaded : Ts... {
    using Ts::operator()...; // ... means to unpack
};

// deduction guide
template <class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

object_literal Interpreter::evaluate(Expr &expr) {
    return std::visit(*this, expr);
} /* evaluate() */

object_literal Interpreter::operator()(std::unique_ptr<Binary> &expr) {
    auto left = evaluate(expr->left);
    auto right = evaluate(expr->right);

    // comparison edge case where two things are either error or null
    if (left.index() != right.index()) {
        // both are null
        if (std::holds_alternative<std::monostate>(left) &&
            std::holds_alternative<std::monostate>(right)) {
            return true;
        }

        // one is null
        if (std::holds_alternative<std::monostate>(left) ||
            std::holds_alternative<std::monostate>(right)) {
            return false;
        }
        return std::monostate();
    }

    return std::visit(
        overloaded{
            // numerical ops
            [&](double left, double right) -> object_literal {
                switch (expr->op.type) {
                    case MINUS:
                        return left - right;
                    case PLUS:
                        return left + right;
                    case DIV:
                        return left / right;
                    case MULT:
                        return left * right;
                    case GREATER_THAN_EQUAL:
                        return left >= right;
                    case LESS_THAN_EQUAL:
                        return left <= right;
                    case LESS_THAN:
                        return left < right;
                    case GREATER_THAN:
                        return left > right;
                    case EQUAL_EQUAL:
                        return left == right;
                    case NOT_EQUAL:
                        return left != right;
                    default:
                        report_expr(expr->op.line,
                                    "Invalid Operand For Number Type");
                        return std::monostate();
                }
            },
            // string ops
            [&](std::string left, std::string right) -> object_literal {
                switch (expr->op.type) {
                    case PLUS:
                        return left + right;
                    case EQUAL_EQUAL:
                        return left == right;
                    case LESS_THAN:
                        return left < right;
                    case GREATER_THAN:
                        return left > right;
                    default:
                        report_expr(expr->op.line,
                                    "Invalid Operand For String Type");
                        return std::monostate();
                }
                return std::monostate();
            },
            // bool ops
            [&](bool left, bool right) -> object_literal {
                switch (expr->op.type) {
                    case EQUAL_EQUAL:
                        return left == right;
                    case NOT_EQUAL:
                        return left != right;
                    default:
                        report_expr(expr->op.line,
                                    "Invalid Operand For Boolean Type");
                        return std::monostate();
                }
            },
            // null comparisons
            [&](std::monostate left, std::monostate right) -> object_literal {
                switch (expr->op.type) {
                    case EQUAL_EQUAL:
                        return true;
                    case NOT_EQUAL:
                        return false;
                    default:
                        report_expr(expr->op.line,
                                    "Invalid Operand For Null Type");
                        return std::monostate();
                }
            },
            // more null comparisons
            [&](std::monostate left, auto right) -> object_literal {
                switch (expr->op.type) {
                    case EQUAL_EQUAL:
                        return false;
                    case NOT_EQUAL:
                        return true;
                    default:
                        report_expr(expr->op.line, "Invalid Null Comparison");
                        return std::monostate();
                }
            },
            // even more null comparisons
            [&](auto left, std::monostate right) -> object_literal {
                switch (expr->op.type) {
                    case EQUAL_EQUAL:
                        return false;
                    case NOT_EQUAL:
                        return true;
                    default:
                        report_expr(expr->op.line, "Invalid Null Comparison");
                        return std::monostate();
                }
            },
            // throw error invalid expression
            [&](auto left, auto right) -> object_literal {
                report_expr(expr->op.line, "Invalid Comparison Types");
                return std::monostate();
            }},
        left, right);
} /* operator() - Binary Expression */

object_literal Interpreter::operator()(std::unique_ptr<Unary> &expr) {
    auto right = evaluate(expr->right);
}

object_literal Interpreter::operator()(std::unique_ptr<Grouping> &expr) {
    return evaluate(expr->expr);
}

object_literal Interpreter::operator()(std::unique_ptr<Literal> &expr) {
}

object_literal Interpreter::operator()(std::monostate) {
}
