#include "expression.hpp"
#include "token.hpp"

#include <initializer_list>
#include <vector>

class Parser {
public:
    explicit Parser(std::vector<Token> tokens) : tokens(std::move(tokens)) {};
    Expr parse();
    bool had_error = false;

private:
    int cur = 0;
    std::vector<Token> tokens;

    Expr expression();
    Expr equality();
    Expr comparison();
    Expr term();
    Expr factor();
    Expr unary();
    Expr primary();

    bool match(std::initializer_list<token_type>);
    bool at_end();
    Token consume();
    Token prev();
};
