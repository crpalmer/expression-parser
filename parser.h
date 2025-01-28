#ifndef __PARSER_H__
#define __PARSER_H__

#include "tokenizer.h"

class Expression {
public:
    virtual void print() = 0;
    virtual double evaluate() = 0;
};

class UnaryOperatorExpression : public Expression {
public:
    UnaryOperatorExpression(Token *op, Expression *expr) : op(op), expr(expr) {}
    void print() override;
    double evaluate() override;

private:
    Token *op;
    Expression *expr;
};

class BinaryOperatorExpression : public Expression {
public:
    BinaryOperatorExpression(Expression *lhs, Token *op, Expression *rhs) : lhs(lhs), op(op), rhs(rhs) {}
    void print() override;
    double evaluate() override;

private:
    Token *op;
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
