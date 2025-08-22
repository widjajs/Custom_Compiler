#include "../includes/tokenizer.hpp"
#include "../includes/error.hpp"

bool Tokenizer::at_end() {
    return cur >= src.length();
} /* at_end() */

bool Tokenizer::match_token(char expected) {
    if (cur >= src.length()) {
        return false;
    }
    if (src[cur] != expected) {
        return false;
    }
    cur++;
    return true;
} /* match_token */

char Tokenizer::peek() {
    if (src.empty() || at_end()) {
        return '\0';
    }
    return src[cur];
} /* peek() */

char Tokenizer::peek_next() {
    if (src.empty() || at_end() || cur + 1 >= src.length()) {
        return '\0';
    }
    return src[cur + 1];
}

char Tokenizer::consume() {
    cur++;
    if (src.empty() || at_end()) {
        return '\0';
    }
    return src[cur - 1];
} /* consume() */

void Tokenizer::add_token(token_type type) {
    add_token(type, std::monostate{});
} /* add_token() */

void Tokenizer::add_token(token_type type, object_literal literal) {
    std::string text = src.substr(start, cur - start);
    // std::cout << start << " " << cur << std::endl;
    tokens.push_back(Token(type, text, literal, line));
} /* add_token() */

void Tokenizer::read_string() {
    while (!at_end() && peek() != '"') {
        if (peek() == '\n') {
            line++;
        }
        consume();
    }
    // check for unclosed string
    if (at_end()) {
        has_error = true;
        report_syntax(line, "Unclosed String");
        return;
    }
    // consume closing "
    consume();
} /* read_string() */

bool Tokenizer::is_digit(char c) {
    return c >= '0' && c <= '9';
} /* is_digit() */

void Tokenizer::read_num() {
    while (!at_end() && is_digit(peek())) {
        consume();
    }
    if (peek() == '.' && is_digit(peek_next())) {
        // consume the '.'
        consume();

        while (!at_end() && is_digit(peek())) {
            consume();
        }
    }
} /* read_num() */

bool Tokenizer::is_alpha(char c) {
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c == '_');
} /* is_alpha() */

bool Tokenizer::is_alphanum(char c) {
    return is_alpha(c) || is_digit(c);
} /* is_alphanum() */

void Tokenizer::read_identifier() {
    while (!at_end() && is_alphanum(peek())) {
        consume();
    }
} /* read_identifier() */

void Tokenizer::scan_token() {
    char c = consume();
    switch (c) {
        case '(':
            add_token(OPEN_PAREN);
            break;
        case ')':
            add_token(CLOSE_PAREN);
            break;
        case '{':
            add_token(OPEN_CURLY);
            break;
        case '}':
            add_token(CLOSE_CURLY);
            break;
        case ',':
            add_token(COMMA);
            break;
        case '.':
            add_token(DOT);
            break;
        case ';':
            add_token(SEMICOLON);
            break;
        case '*':
            add_token(MULT);
            break;
        case '+':
            add_token(PLUS);
            break;
        case '-':
            add_token(MINUS);
            break;
        case '!':
            add_token(match_token('=') ? NOT_EQUAL : NOT);
            break;
        case '=':
            add_token(match_token('=') ? EQUAL_EQUAL : EQUAL);
            break;
        case '<':
            add_token(match_token('=') ? LESS_THAN_EQUAL : EQUAL);
            break;
        case '>':
            add_token(match_token('=') ? GREATER_THAN_EQUAL : EQUAL);
            break;
        case '/':
            if (match_token('/')) {
                while (!at_end() && peek() != '\n') {
                    consume();
                }
            } else if (match_token('*')) {
                while (!at_end() && !(peek() == '*' && peek_next() == '/')) {
                    if (peek() == '\n') {
                        line++;
                    }
                    consume();
                }
                if (peek() == '*' && peek_next() == '/') {
                    consume();
                    consume();
                } else {
                    report_syntax(line, "Unclosed Multi-line Comment");
                }
            } else {
                add_token(DIV);
            }
            break;
        case '"':
            read_string();
            if (!has_error) {
                add_token(STRING, src.substr(start + 1, cur - start - 2));
            }
            break;
        case '\0':
            break;
        case ' ':
            break;
        case '\r':
            break;
        case '\t':
            break;
        case '\n':
            line++;
            break;
        default:
            // digit and keyword checkers here
            // making cases for them would be quite erm annoying
            if (is_digit(c)) {
                read_num();
                std::string num_str = src.substr(start, cur - start);
                double num_value = std::stod(num_str);
                add_token(NUMBER, num_value);
            } else if (is_alpha(c)) {
                read_identifier();
                std::string token = src.substr(start, cur - start);
                if (identifiers.find(token) == identifiers.end()) {
                    add_token(IDENTIFIER, token);
                } else {
                    add_token(identifiers.at(token), token);
                }
            } else {
                report_syntax(line, "Invalid Character");
                has_error = true;
            }
            break;
    }
} /* scan_token() */

std::vector<Token> Tokenizer::scan_tokens() {
    while (!at_end()) {
        start = cur;
        scan_token();
    }
    tokens.push_back(Token(END_FILE, "", std::monostate{}, line));
    return tokens;
} /* scan_tokens() */
