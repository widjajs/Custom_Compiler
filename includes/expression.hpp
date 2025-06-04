#include "token.hpp"

#include <memory>

struct Binary;
struct Grouping;
struct Literal;
struct Unary;

using Expr = std::variant<std::unique_ptr<Binary>, std::unique_ptr<Grouping>,
                          std::unique_ptr<Literal>, std::unique_ptr<Unary>,
                          std::monostate>;

struct Unary {
    Token op;
    Expr right;

    Unary(Token op, Expr right) : op(std::move(op)), right(std::move(right)) {};
};

struct Binary {
    Expr left;
    Token op;
    Expr right;

    Binary(Expr left, Token op, Expr right)
        : left(std::move(left)), op(op), right(std::move(right)) {};
};

struct Grouping {
    Expr expr;

    Grouping(Expr expr) : expr(std::move(expr)) {};
};

struct Literal {
    object_literal val;

    Literal(object_literal val) : val(std::move(val)) {};
};
