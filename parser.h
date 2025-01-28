#ifndef __PARSER_H__
#define __PARSER_H__

#include "tokenizer.h"

class Expression {
public:
    virtual void print() = 0;
    virtual double evaluate() = 0;
};

class OperatorExpression : public Expression {
public:
    OperatorExpression(Expression *lhs, OperatorToken *op, Expression *rhs) : lhs(lhs), op(op), rhs(rhs) {}
    void print() override;
    double evaluate() override;

private:
    OperatorToken *op;
    Expression *lhs, *rhs;
};

class LiteralExpression : public Expression {
public:
    LiteralExpression(double value) : value(value) {}
    void print() override;
    double evaluate() override { return value; }

private:
    double value;
};

Expression *parse_expression(Tokenizer *tokenizer);

#endif
