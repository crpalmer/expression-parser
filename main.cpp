#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "parser.h"
#include "tokenizer.h"
#include "utils.h"

static char line[100*1024];

int main(int argc, char **argv) {
    while (fgets(line, sizeof(line), stdin) != NULL) {
	Tokenizer *tokenizer = new Tokenizer(line);
	bool first = true;
	Token *token;

	printf("token stream: ");
	while (tokenizer->pop(&token)) {
	    if (! first) {
		printf(", ");
	    }
	    first = false;
	    token->print();
	}
	printf("\n");

	tokenizer->reset();
	Expression *expr = parse_statement(new Tokenizer(line));
	if (expr != NULL) {
	    if (expr->can_evaluate()) {
		expr->print();
		printf(" = ");
		double value = expr->evaluate();
		print_number(value);
	    } else {
		expr->simplify();
		expr->print();
	    }
	    printf("\n");
	}
    }
    return 0;
}
