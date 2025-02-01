#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <math.h>
#include "parser.h"
#include "utils.h"

bool NaryOperatorExpression::can_evaluate() {
    bool yes = true;
    for (auto expr : exprs) {
	yes = yes && expr->can_evaluate();
    }
    return yes;
}

void NaryOperatorExpression::add_expression(Expression *expr) {
    exprs.push_back(expr);
}

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
    case EXPR_EXPONENTIATION: printf("^"); break;
    }
    rhs->print();
    printf(")");
}

void NaryOperatorExpression::print() {
    printf("(");
    switch (op) {
    case EXPR_ADDITION: printf("+"); break;
    case EXPR_MULTIPLICATION: printf("*"); break;
    }
    for (auto expr : exprs) {
	printf(" ");
	expr->print();
    }
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
    case EXPR_EXPONENTIATION: return pow(v1, v2);
    }
    assert(0);
}

double NaryOperatorExpression::evaluate() {
    bool is_first = true;
    double v = 0;

    for (auto expr : exprs) {
	if (is_first) {
	    v = expr->evaluate();
	    is_first = false;
	} else {
	    switch (op) {
	    case EXPR_ADDITION: v += expr->evaluate(); break;
	    case EXPR_MULTIPLICATION: v *= expr->evaluate(); break;
	    }
	}
    }

    return v;
}

void UnaryOperatorExpression::simplify() {
    expr->simplify();
}

void BinaryOperatorExpression::simplify() {
    if (lhs->can_evaluate()) lhs = new LiteralExpression(lhs->evaluate());
    else lhs->simplify();

    if (rhs->can_evaluate()) rhs = new LiteralExpression(rhs->evaluate());
    else rhs->simplify();
}

void NaryOperatorExpression::simplify() {
    std::list<Expression *> new_exprs;
    bool first_v = true;
    double v = 0;

    for (auto expr : exprs) {
	if (! expr->can_evaluate()) {
	    expr->simplify();
	    new_exprs.push_back(expr);
	} else if (first_v) {
	    first_v = false;
	    v = expr->evaluate();
	} else {
	    switch (op) {
	    case EXPR_ADDITION: v += expr->evaluate(); break;
	    case EXPR_MULTIPLICATION: v *= expr->evaluate(); break;
	    }
	}
    }

    if (! first_v) new_exprs.push_back(new LiteralExpression(v));
    exprs = new_exprs;
}
