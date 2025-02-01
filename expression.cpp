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
    switch (op) {
    case EXPR_NEGATION: printf("-"); break;
    case EXPR_ONE_OVER: printf("1/"); break;
    }
    expr->print();
    printf(")");
}

void BinaryOperatorExpression::print() {
    printf("(");
    lhs->print();
    switch (op) {
    case EXPR_ADDITION: printf("+"); break;
    case EXPR_MULTIPLICATION: printf("*"); break;
    case EXPR_EXPONENTIATION: printf("^"); break;
    }
    rhs->print();
    printf(")");
}

double UnaryOperatorExpression::evaluate() {
    double v = expr->evaluate();
    switch (op) {
    case EXPR_NEGATION: return -v;
    case EXPR_ONE_OVER: return 1/v;
    }
    assert(0);
}

double BinaryOperatorExpression::evaluate() {
    double v1 = lhs->evaluate();
    double v2 = rhs->evaluate();
    switch (op) {
    case EXPR_ADDITION: return v1 + v2;
    case EXPR_MULTIPLICATION: return v1 * v2;
    case EXPR_EXPONENTIATION: return pow(v1, v2);
    }
    assert(0);
}
