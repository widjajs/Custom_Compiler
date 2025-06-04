#include "expression.hpp"
#include "token.hpp"

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
