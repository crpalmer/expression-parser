#ifndef __EXPRESSION_H__
#define __EXPRESSION_H__

#include <assert.h>
#include "tokenizer.h"

class Expression {
public:
    virtual void print() = 0;
    virtual double evaluate() = 0;
    virtual bool can_evaluate() = 0;
};

class UnaryOperatorExpression : public Expression {
public:
    UnaryOperatorExpression(Token *op, Expression *expr) : op(op), expr(expr) {}
    void print() override;
    double evaluate() override;
    bool can_evaluate() override { return expr->can_evaluate(); }

private:
    Token *op;
    Expression *expr;
};

class BinaryOperatorExpression : public Expression {
public:
    BinaryOperatorExpression(Expression *lhs, Token *op, Expression *rhs) : lhs(lhs), op(op), rhs(rhs) {}
    void print() override;
    double evaluate() override;
    bool can_evaluate() override { return lhs->can_evaluate() && rhs->can_evaluate(); }

private:
    Token *op;
    Expression *lhs, *rhs;
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

#endif
