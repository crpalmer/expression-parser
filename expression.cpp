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

void EqualityExpression::print() {
    lhs->print();
    printf(" = ");
    rhs->print();
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

Expression *UnaryOperatorExpression::simplify(Expression *top) {
    expr->simplify(top);
    return this;
}

Expression *BinaryOperatorExpression::simplify(Expression *top) {
    if (lhs->can_evaluate()) lhs = new LiteralExpression(lhs->evaluate());
    else lhs = lhs->simplify(top);

    if (rhs->can_evaluate()) rhs = new LiteralExpression(rhs->evaluate());
    else rhs = rhs->simplify(top);

    return this;
}

Expression *NaryOperatorExpression::simplify(Expression *top) {
    std::list<Expression *> new_exprs;
    bool first_v = true;
    bool did_something = false;
    double v = 0;

    for (auto expr : exprs) {
	if (! expr->can_evaluate()) {
	    did_something = true;
	    new_exprs.push_back(expr->simplify(top));
	} else if (first_v) {
	    first_v = false;
	    v = expr->evaluate();
	} else {
	    did_something = true;
	    switch (op) {
	    case EXPR_ADDITION: v += expr->evaluate(); break;
	    case EXPR_MULTIPLICATION: v *= expr->evaluate(); break;
	    }
	}
    }

    if (did_something) {
	if (new_exprs.empty()) printf("Evaluate: ");
	else printf("partial ev: ");
	print();
	printf("\n");
    }

    if (! first_v) new_exprs.push_back(new LiteralExpression(v));
    exprs = new_exprs;

    if (did_something) {
	printf("simplified: ");
	top->print();
	printf("\n");
    }

    if (exprs.size() == 1) return exprs.front();
    else return this;
}

Expression *EqualityExpression::simplify(Expression *top) {
    while (1) {
	if (! lhs->can_evaluate()) {
            printf("Trying to simplify the LHS:\n");
            lhs->simplify(top);
	    printf("\n");
	} else if (! lhs->is_literal()) {
	    printf("Evaluating the LHS = ");
	    lhs = new LiteralExpression(lhs->evaluate());
	    lhs->print();
	    printf("\n");
	}

	if (! rhs->can_evaluate()) {
	    printf("Trying to simplify the RHS:\n");
	    rhs->simplify(top);
	    printf("\n");
	} else if (! rhs->is_literal()) {
	    printf("Evaluating the RHS = ");
	    rhs = new LiteralExpression(rhs->evaluate());
	    rhs->print();
	    printf("\n");
	}
	break;
    }
    return this;
}
