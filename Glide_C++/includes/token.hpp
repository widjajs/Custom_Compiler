#pragma once

#include <sstream>
#include <string>
#include <unordered_map>
#include <variant>

typedef enum token_type {
    // one char tokens
    OPEN_PAREN,
    CLOSE_PAREN,
    OPEN_CURLY,
    CLOSE_CURLY,
    COMMA,
    DOT,
    MINUS,
    PLUS,
    SEMICOLON,
    DIV,
    MULT,

    // one or two char tokens
    NOT,
    NOT_EQUAL,
    EQUAL,
    EQUAL_EQUAL,
    GREATER_THAN,
    GREATER_THAN_EQUAL,
    LESS_THAN,
    LESS_THAN_EQUAL,

    // literal types
    IDENTIFIER,
    STRING,
    NUMBER,

    // reserved words
    AND,
    OR,
    TRUE,
    FALSE,
    IF,
    ELSE,
    FOR,
    WHILE,
    CLASS,
    THIS,
    SUPER,
    RETURN,
    FUNC,
    LET,
    NONE,
    PRINT,
    END_FILE
} token_type;

static const std::unordered_map<std::string, token_type> identifiers = {
    {"and", AND},     {"or", OR},     {"true", TRUE},   {"false", FALSE},
    {"if", IF},       {"else", ELSE}, {"for", FOR},     {"while", WHILE},
    {"class", CLASS}, {"this", THIS}, {"super", SUPER}, {"return", RETURN},
    {"func", FUNC},   {"let", LET},   {"none", NONE},   {"print", PRINT},
};

typedef std::variant<double, std::string, bool, std::monostate> object_literal;

class Token {
public:
    token_type type;
    std::string token;
    std::variant<double, std::string, bool, std::monostate> literal;
    int line;
    bool has_error;

    // constructor
    Token(token_type type, std::string token, object_literal literal, int line)
        : type(type), token(std::move(token)), literal(std::move(literal)), line(line) {};

    // check the type and return the oject literal in it's "true" form
    std::string get_literal(object_literal value) {
        return std::visit(
            [](const auto &v) -> std::string {
                using T = std::decay_t<decltype(v)>;
                if constexpr (std::is_same_v<T, std::string>) {
                    return v;
                } else if constexpr (std::is_same_v<T, bool>) {
                    return v ? "true" : "false";
                } else if constexpr (std::is_same_v<T, double>) {
                    return std::to_string(v);
                } else if constexpr (std::is_same_v<T, std::monostate>) {
                    return "none";
                }
            },
            value);
    }

    std::string to_string() {
        std::stringstream output;
        output << "type: " << type << ", token: " << token << ", literal: " << get_literal(literal);
        return output.str();
    }
};
