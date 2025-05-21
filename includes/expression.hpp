#include "token.hpp"

struct Binary;
struct Grouping;
struct Literal;
struct Unary;

using Expr = std::variant<Binary, Grouping, Literal, Unary>;

struct Binary {
    Expr *left;
    Token op;
    Expr *right;
};

struct Grouping {
    Expr *expression;
};

struct Literal {
    object_literal val;
};

struct Unary {
    Token op;
    Expr *right;
};
