#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "parser.h"
#include "utils.h"

Expression *error(const char *str, Token *token) {
    printf("%s: ", str);
    token->print();
    printf("\n");
    return NULL;
}

void LiteralExpression::print() {
    print_number(value);
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
    assert(0);
}

Expression *parse_literal_expression(Tokenizer *tokenizer) {
    Token *token;
    if (! tokenizer->peek(&token)) return NULL;
    if (token->get_type() == TOK_NUMBER) {
	tokenizer->pop();
	NumberToken *number = static_cast<NumberToken *>(token);
	return new LiteralExpression(number->get_value());
    }
    if (token->is_operator(OP_MINUS) || token->is_operator(OP_PLUS)) {
	tokenizer->pop();
	Expression *expr = parse_literal_expression(tokenizer);
	if (expr) expr = new UnaryOperatorExpression(token, expr);
	return expr;
    }
    if (token->is_operator(OP_OPEN_PAREN)) {
	tokenizer->pop();
	Expression *expr = parse_expression(tokenizer);
	if (! tokenizer->pop(&token) || ! token->is_operator(OP_CLOSE_PAREN)) {
	    return error("Missing closing parenthesis", token);
	}
	return expr;
    }
    return error("Invalid token", token);
}

Expression *parse_multiplication_expression(Tokenizer *tokenizer) {
    Token *token;
    Expression *lhs;

    lhs = parse_literal_expression(tokenizer);

    while (tokenizer->peek(&token) && (token->is_operator(OP_MULTIPLY) || token->is_operator(OP_DIVIDE))) {
	tokenizer->pop();
	Expression *rhs = parse_literal_expression(tokenizer);
	if (rhs == NULL) {
	    return error("Failed to parse the rhs", token);
	}
	lhs = new BinaryOperatorExpression(lhs, token, rhs);
    }
    return lhs;
}

Expression *parse_addition_expression(Tokenizer *tokenizer) {
    Token *token;
    Expression *lhs;

    lhs = parse_multiplication_expression(tokenizer);

    while (tokenizer->peek(&token) && (token->is_operator(OP_PLUS) || token->is_operator(OP_MINUS))) {
	tokenizer->pop();
	Expression *rhs = parse_multiplication_expression(tokenizer);
	if (rhs == NULL) {
	    return error("Failed to parse the rhs", token);
	}
	lhs = new BinaryOperatorExpression(lhs, token, rhs);
    }
    return lhs;
}

Expression *parse_expression(Tokenizer *tokenizer) {
    return parse_addition_expression(tokenizer);
}
