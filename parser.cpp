#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <math.h>
#include "parser.h"
#include "utils.h"

static Expression *parse_expression(Tokenizer *tokenizer);

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
    if (token->is_operator(OP_PLUS)) {
	tokenizer->pop();
	return parse_literal_expression(tokenizer);
    }
    if (token->is_operator(OP_MINUS)) {
	tokenizer->pop();
	Expression *expr = parse_literal_expression(tokenizer);
	if (expr) expr = new UnaryOperatorExpression(EXPR_NEGATION, expr);
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

Expression *parse_exponent_expression(Tokenizer *tokenizer) {
    Token *token;
    Expression *lhs;

    lhs = parse_literal_expression(tokenizer);
    if (tokenizer->peek(&token) && token->is_operator(OP_EXPONENT)) {
	tokenizer->pop();
	Expression *rhs = parse_exponent_expression(tokenizer);
	if (rhs) return new BinaryOperatorExpression(lhs, EXPR_EXPONENTIATION, rhs);
	else return NULL;
    }
    return lhs;
}

Expression *parse_multiplication_expression(Tokenizer *tokenizer) {
    Token *token;
    Expression *lhs;
    NaryOperatorExpression *expr = NULL;

    lhs = parse_exponent_expression(tokenizer);

    while (tokenizer->peek(&token) && (token->is_operator(OP_MULTIPLY) || token->is_operator(OP_DIVIDE))) {
	tokenizer->pop();
	if (expr == NULL) {
	    expr = new NaryOperatorExpression(EXPR_MULTIPLICATION);
	    expr->add_expression(lhs);
	}
	Expression *rhs = parse_exponent_expression(tokenizer);
	if (rhs == NULL) {
	    return error("Failed to parse the rhs", token);
	}
	if (token->is_operator(OP_DIVIDE)) {
	     rhs = new UnaryOperatorExpression(EXPR_ONE_OVER, rhs);
	}
	expr->add_expression(rhs);
    }
    return expr ? expr : lhs;
}

Expression *parse_addition_expression(Tokenizer *tokenizer) {
    Token *token;
    Expression *lhs;
    NaryOperatorExpression *expr = NULL;

    lhs = parse_multiplication_expression(tokenizer);

    while (tokenizer->peek(&token) && (token->is_operator(OP_PLUS) || token->is_operator(OP_MINUS))) {
	tokenizer->pop();
	if (expr == NULL) {
	    expr = new NaryOperatorExpression(EXPR_ADDITION);
	    expr->add_expression(lhs);
	}
	Expression *rhs = parse_multiplication_expression(tokenizer);
	if (rhs == NULL) {
	    return error("Failed to parse the rhs", token);
	}
	if (token->is_operator(OP_MINUS)) {
	     rhs = new UnaryOperatorExpression(EXPR_NEGATION, rhs);
	}
	expr->add_expression(rhs);
    }
    return expr ? expr : lhs;
}

static Expression *parse_expression(Tokenizer *tokenizer) {
    return parse_addition_expression(tokenizer);
}

Expression *parse_statement(Tokenizer *tokenizer) {
    Token *token;
    Expression *expr = parse_expression(tokenizer);
    if (tokenizer->peek(&token) && token->is_operator(OP_EQUALITY)) {
	tokenizer->pop();
	Expression *rhs = parse_expression(tokenizer);
	expr = new EqualityExpression(expr, rhs);
    }
    if (tokenizer->pop(&token)) return error("Extra input", token);
    return expr;
}
