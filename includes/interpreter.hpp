#pragma once
#include "expression.hpp"
#include "token.hpp"

// overloaded helper
template <class... Ts> // ... means accept any number of types
struct overloaded : Ts... {
    using Ts::operator()...; // ... means to unpack
};

// deduction guide
template <class... Ts> overloaded(Ts...) -> overloaded<Ts...>;
class Interpreter {
public:
    bool had_error = false;

    object_literal evaluate(Expr &expr);
    object_literal operator()(std::unique_ptr<Binary> &expr);
    object_literal operator()(std::unique_ptr<Unary> &expr);
    object_literal operator()(std::unique_ptr<Grouping> &expr);
    object_literal operator()(std::unique_ptr<Literal> &expr);
    object_literal operator()(std::monostate);
};
