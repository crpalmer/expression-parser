#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <math.h>
#include "parser.h"
#include "utils.h"

Expression *parse_expression0(Tokenizer *tokenizer);

Expression *error(const char *str, Token *token) {
    printf("%s: ", str);
    token->print();
    printf("\n");
    return NULL;
}

Expression *parse_literal_expression(Tokenizer *tokenizer) {
    Token *token;
    if (! tokenizer->peek(&token)) return NULL;
    if (token->get_type() == TOK_NUMBER) {
	tokenizer->pop();
	NumberToken *number = (NumberToken *) token;
	return new LiteralExpression(number->get_value());
    }
    if (token->get_type() == TOK_VARIABLE) {
	tokenizer->pop();
	VariableToken *var = (VariableToken *) token;
	return new VariableExpression(var);
    }
    if (token->is_operator(OP_MINUS) || token->is_operator(OP_PLUS)) {
	tokenizer->pop();
	Expression *expr = parse_literal_expression(tokenizer);
	if (expr) expr = new UnaryOperatorExpression(token, expr);
	return expr;
    }
    if (token->is_operator(OP_OPEN_PAREN)) {
	tokenizer->pop();
	Expression *expr = parse_expression0(tokenizer);
	if (! tokenizer->pop(&token) || ! token->is_operator(OP_CLOSE_PAREN)) {
	    return error("Missing closing parenthesis", token);
	}
	return expr;
    }
    return error("Invalid token", token);
}

Expression *parse_exponent_expression(Tokenizer *tokenizer) {
    Token *token;
    Expression *lhs;

    lhs = parse_literal_expression(tokenizer);
    if (tokenizer->peek(&token) && token->is_operator(OP_EXPONENT)) {
	tokenizer->pop();
	Expression *rhs = parse_exponent_expression(tokenizer);
	if (rhs) return new BinaryOperatorExpression(lhs, token, rhs);
	else return NULL;
    }
    return lhs;
}

Expression *parse_multiplication_expression(Tokenizer *tokenizer) {
    Token *token;
    Expression *lhs;

    lhs = parse_exponent_expression(tokenizer);

    while (tokenizer->peek(&token) && (token->is_operator(OP_MULTIPLY) || token->is_operator(OP_DIVIDE))) {
	tokenizer->pop();
	Expression *rhs = parse_exponent_expression(tokenizer);
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

Expression *parse_expression0(Tokenizer *tokenizer) {
    return parse_addition_expression(tokenizer);
}

Expression *parse_expression(Tokenizer *tokenizer) {
    Token *token;
    Expression *expr = parse_expression0(tokenizer);
    if (tokenizer->pop(&token)) return error("Extra input", token);
    return expr;
}
