#ifndef __EXPRESSION_H__
#define __EXPRESSION_H__

#include <assert.h>
#include <list>
#include "tokenizer.h"

typedef enum { EXPR_NEGATION, EXPR_ONE_OVER } unary_expression_t;
typedef enum { EXPR_ADDITION, EXPR_MULTIPLICATION } nary_expression_t;
typedef enum { EXPR_EXPONENTIATION } binary_expression_t;

class Expression {
public:
    virtual void print() = 0;
    virtual bool can_evaluate() = 0;
    virtual double evaluate() = 0;
    virtual void simplify() { }
};

class UnaryOperatorExpression : public Expression {
public:
    UnaryOperatorExpression(unary_expression_t op, Expression *expr) : op(op), expr(expr) {}
    void print() override;
    bool can_evaluate() override { return expr->can_evaluate(); }
    double evaluate() override;
    void simplify() override;

private:
    unary_expression_t op;
    Expression *expr;
};

class BinaryOperatorExpression : public Expression {
public:
    BinaryOperatorExpression(Expression *lhs, binary_expression_t op, Expression *rhs) : lhs(lhs), op(op), rhs(rhs) {}
    void print() override;
    bool can_evaluate() override { return lhs->can_evaluate() && rhs->can_evaluate(); }
    double evaluate() override;
    void simplify() override;

private:
    binary_expression_t op;
    Expression *lhs, *rhs;
};

class NaryOperatorExpression : public Expression {
public:
    NaryOperatorExpression(nary_expression_t op) : op(op) {}
    void add_expression(Expression *expr);
    void print() override;
    bool can_evaluate() override;
    double evaluate() override;
    void simplify() override;

private:
    nary_expression_t op;
    std::list<Expression *> exprs;
};

class LiteralExpression : public Expression {
public:
    LiteralExpression(double value) : value(value) {}
    void print() override;
    double evaluate() override { return value; }
    bool can_evaluate() override { return true; }

private:
    double value;
};

class VariableExpression : public Expression {
public:
    VariableExpression(VariableToken *var) : var(var) {}
    void print() override;
    double evaluate() override { assert(0); }
    bool can_evaluate() override { return false; }

private: 
    VariableToken *var;
};

class EqualityExpression : public Expression {
public:
    EqualityExpression(Expression *lhs, Expression *rhs) : lhs(lhs), rhs(rhs) {}
    void print() override;
    double evaluate() override { assert(0); }
    bool can_evaluate() override { return false; }
    void simplify() override;

private:
    Expression *lhs;
    Expression *rhs;
};

#endif
