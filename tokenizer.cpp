#include <stdio.h>
#include <assert.h>
#include <ctype.h>
#include <malloc.h>
#include <string.h>

#include "tokenizer.h"

void VariableToken::print() {
    printf("%s", name);
}

void NumberToken::print() {
    printf("%f", value);
}

void OperatorToken::print() {
    switch(op) {
    case OP_PLUS: printf("+"); break;
    case OP_MINUS: printf("-"); break;
    case OP_OPEN_PAREN: printf("("); break;
    case OP_CLOSE_PAREN: printf(")"); break;
    case OP_MULTIPLY: printf("*"); break;
    case OP_DIVIDE: printf("/"); break;
    case OP_EXPONENT: printf("^"); break;
    case OP_EQUALITY: printf("="); break;
    default: assert(0);
    }
}

void Tokenizer::get_next_token() {
    next_token = NULL;

    while (isspace(input[pos])) {
	pos++;
    }

    if (! input[pos]) return;

    if (isdigit(input[pos])) {
	double number;
	int n;

	if (sscanf(&input[pos], "%lf%n", &number, &n) != 1) {
	    printf("could not parse a number at pos:\n");
	    printf("%.*s ^^^ %s\n", pos, input, &input[pos]);
	    return;
	}
	next_token = new NumberToken(number);
	pos += n;
    } else if (isalpha(input[pos]) || input[pos] == '_') {
	int start = pos;
	while (isalnum(input[pos]) || input[pos] == '_') pos++;
	char *name = (char *) malloc(pos - start + 1);
	strncpy(name, &input[start], pos - start);
	name[pos - start] = '\0';
	if (strcmp(name, "x") != 0) {
	    printf("only 'x' may be a variable.\n");
	    return;
	}
	next_token = new VariableToken(name);
    } else {
	switch(input[pos]) {
	case '+': next_token = new OperatorToken(OP_PLUS); break;
	case '-': next_token = new OperatorToken(OP_MINUS); break;
	case '(': next_token = new OperatorToken(OP_OPEN_PAREN); break;
	case ')': next_token = new OperatorToken(OP_CLOSE_PAREN); break;
	case '*': next_token = new OperatorToken(OP_MULTIPLY); break;
	case '/': next_token = new OperatorToken(OP_DIVIDE); break;
	case '^': next_token = new OperatorToken(OP_EXPONENT); break;
	case '=': next_token = new OperatorToken(OP_EQUALITY); break;
	default:
	    printf("Unknown operator '%c':\n", input[pos]);
	    printf("%.*s ^^^ %s\n", pos, input, &input[pos]);
	    return;
	}
	pos++;
    }
}

Tokenizer::Tokenizer(const char *input) : input(input) {
    get_next_token();
}

bool Tokenizer::pop() {
    if (next_token == NULL) return false;
    get_next_token();
    return true;
}

bool Tokenizer::pop(Token **token) {
    if (peek(token)) get_next_token();
    return *token != NULL;
}

bool Tokenizer::peek(Token **token) {
    *token = next_token;
    return next_token != NULL;
}

void Tokenizer::reset() {
    pos = 0;
    get_next_token();
}
