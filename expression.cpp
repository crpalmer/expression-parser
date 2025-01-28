#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <math.h>
#include "parser.h"
#include "utils.h"

void LiteralExpression::print() {
    print_number(value);
}

void VariableExpression::print() {
    var->print();
}

void UnaryOperatorExpression::print() {
    printf("(");
    op->print();
    expr->print();
    printf(")");
}

void BinaryOperatorExpression::print() {
    printf("(");
    lhs->print();
    op->print();
    rhs->print();
    printf(")");
}

double UnaryOperatorExpression::evaluate() {
    double v = expr->evaluate();
    if (op->is_operator(OP_PLUS)) return v;
    if (op->is_operator(OP_MINUS)) return -v;
    assert(0);
}

double BinaryOperatorExpression::evaluate() {
    double v1 = lhs->evaluate();
    double v2 = rhs->evaluate();
    if (op->is_operator(OP_PLUS)) return v1 + v2;
    if (op->is_operator(OP_MINUS)) return v1 - v2;
    if (op->is_operator(OP_MULTIPLY)) return v1 * v2;
    if (op->is_operator(OP_DIVIDE)) return v1 / v2;
    if (op->is_operator(OP_EXPONENT)) return pow(v1, v2);
    assert(0);
}
