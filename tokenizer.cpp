#include <stdio.h>
#include <assert.h>
#include <ctype.h>

#include "tokenizer.h"

void NumberToken::print() {
    printf("%f", value);
}

void OperatorToken::print() {
    switch(op) {
    case OP_PLUS: printf("+"); break;
    case OP_MINUS: printf("-"); break;
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
    } else {
	switch(input[pos]) {
	case '+': next_token = new OperatorToken(OP_PLUS); break;
	case '-': next_token = new OperatorToken(OP_MINUS); break;
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

bool Tokenizer::pop(Token **token) {
    if (next_token == NULL) return false;
    *token = next_token;
    get_next_token();
    return true;
}
