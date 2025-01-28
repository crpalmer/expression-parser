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

void OperatorExpression::print() {
    printf("(");
    lhs->print();
    op->print();
    rhs->print();
    printf(")");
}

double OperatorExpression::evaluate() {
    double v1 = lhs->evaluate();
    double v2 = rhs->evaluate();
    switch(op->get_operator()) {
    case OP_PLUS: return v1 + v2;
    case OP_MINUS: return v1 - v2;
    default: assert(0);
    }
}

Expression *parse_literal_expression(Tokenizer *tokenizer) {
    Token *token;
    if (! tokenizer->peek(&token)) return NULL;
    if (token->get_type() == TOK_NUMBER) {
	tokenizer->pop();
	NumberToken *number = static_cast<NumberToken *>(token);
	return new LiteralExpression(number->get_value());
    }
    return error("Invalid token", token);
}

Expression *parse_addition_expression(Tokenizer *tokenizer) {
    Token *token;
    Expression *lhs;

    lhs = parse_literal_expression(tokenizer);

    while (tokenizer->peek(&token) && token->get_type() == TOK_OPERATOR) {
	OperatorToken *op = static_cast<OperatorToken *>(token);

	if (op->get_operator() != OP_PLUS && op->get_operator() != OP_MINUS) {
	    break;
	}

	tokenizer->pop();
	Expression *rhs = parse_literal_expression(tokenizer);
	if (rhs == NULL) {
	    return error("Failed to parse the rhs", token);
	}
	lhs = new OperatorExpression(lhs, op, rhs);
    }
    return lhs;
}

Expression *parse_expression(Tokenizer *tokenizer) {
    return parse_addition_expression(tokenizer);
}
