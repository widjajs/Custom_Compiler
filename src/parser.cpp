#include "../includes/parser.hpp"
#include "../includes/error.hpp"

#include <iostream>

/*
    expression → equality
    equality → comparison ( ( "!=" | "==" ) comparison )*
    comparison → term ( ( ">" | ">=" | "<" | "<=" ) term )*
    term → factor ( ( "-" | "+" ) factor )*
    factor → unary ( ( "/" | "*" ) unary )*
    unary → ( "!" | "-" ) unary | primary
    primary → NUMBER | STRING | "true" | "false" | "nil"| "(" expression ")"
 */

Token Parser::prev() {
    return tokens.at(cur - 1);
} /* prev() */

Token Parser::consume() {
    if (!at_end()) {
        cur++;
    }
    return prev();
} /* consume() */

bool Parser::at_end() {
    return tokens.at(cur).type == END_FILE;
} /* at_end() */

bool Parser::match(std::initializer_list<token_type> types) {
    for (token_type type : types) {
        if (!at_end() && tokens.at(cur).type == type) {
            consume();
            return true;
        }
    }
    return false;
} /* match() */

Expr Parser::parse() {
    return expression();
} /* parse() */

Expr Parser::primary() {
    // primary → NUMBER | STRING | "true" | "false" | "nil"| "(" expression ")"
    if (match({FALSE})) {
        return std::make_unique<Literal>(false);
    }
    if (match({TRUE})) {
        return std::make_unique<Literal>(true);
    }
    if (match({NONE})) {
        return std::make_unique<Literal>(std::monostate());
    }
    if (match({NUMBER, STRING})) {
        return std::make_unique<Literal>(prev().literal);
    }
    if (match({OPEN_PAREN})) {
        Expr expr = expression();
        Token consumed = consume();
        if (consumed.type != CLOSE_PAREN) {
            had_error = true;
            report_syntax(consumed.line, "Unclosed Parenthesis");
        }
        return std::make_unique<Grouping>(std::move(expr));
    }
    had_error = true;
    if (!at_end()) {
        report_expr(tokens.at(cur).line, "Expected Expression Error");
    } else {
        report_expr("Expected Expression Error");
    }
    // std::monostate() represents error expression in this case
    // in the context of literals std::monsotate() represents null values
    return std::monostate();
}

Expr Parser::unary() {
    // unary → ( "!" | "-" ) unary | primary
    if (match({NOT, MINUS})) {
        return std::make_unique<Unary>(std::move(prev()), std::move(unary()));
    }
    return primary();
} /* unary() */

Expr Parser::factor() {
    // factor → unary ( ( "/" | "*" ) unary )*
    Expr expr = unary();

    while (match({DIV, MULT})) {
        std::cout << "FACTOR" << std::endl;
        Token op = prev();
        Expr right = unary();
        expr = std::make_unique<Binary>(std::move(expr), op, std::move(right));
    }

    return expr;
} /* factor() */

Expr Parser::term() {
    // term → factor ( ( "-" | "+" ) factor )*
    Expr expr = factor();

    while (match({PLUS, MINUS})) {
        std::cout << "TERM" << std::endl;
        Token op = prev();
        Expr right = factor();
        expr = std::make_unique<Binary>(std::move(expr), op, std::move(right));
    }

    return expr;
} /* term() */

Expr Parser::comparison() {
    std::cout << "COMPARISON" << std::endl;
    // term → factor ( ( "-" | "+" ) factor )*
    Expr expr = term();

    while (
        match({GREATER_THAN, GREATER_THAN_EQUAL, LESS_THAN_EQUAL, LESS_THAN})) {
        Token op = prev();
        Expr right = term();
        expr = std::make_unique<Binary>(std::move(expr), op, std::move(right));
    }

    return expr;
} /* comparison() */

Expr Parser::equality() {
    std::cout << "EQUALITY" << std::endl;
    // equality -> comparison (("!=" | "==") comparison)*
    Expr expr = comparison();

    while (match({NOT_EQUAL, EQUAL_EQUAL})) {
        // match advances one forward on success, so prev = operator
        Token op = prev();
        Expr right = comparison();

        // each run the previous expression becomes left side
        expr = std::make_unique<Binary>(std::move(expr), op, std::move(right));
    }

    return expr;
} /* equality() */

Expr Parser::expression() {
    return equality();
} /* expression() */
