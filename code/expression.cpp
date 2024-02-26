#include "expression.hpp"

#include <optional>
#include <stack>

#include "binary_expression.hpp"
#include "float_expression.hpp"
#include "identifier_expression.hpp"
#include "integer_expression.hpp"
#include "unary_expression.hpp"

Expression::~Expression() {}