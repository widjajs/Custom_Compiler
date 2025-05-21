#pragma once

#include "../includes/token.hpp"

#include <vector>

class Tokenizer {
public:
    explicit Tokenizer(std::string src) : src(std::move(src)) {};
    std::vector<Token> scan_tokens();
    bool has_error = false;

private:
    std::string src;
    std::vector<Token> tokens;
    int cur = 0;
    int start = 0;
    int line = 1;

    char peek();
    char peek_next();
    char consume();
    void add_token(token_type, object_literal);
    void add_token(token_type);
    void scan_token();
    void read_string();
    void read_num();
    void read_identifier();
    bool match_token(char);
    bool at_end();
    bool is_digit(char);
    bool is_alpha(char);
    bool is_alphanum(char);
};
