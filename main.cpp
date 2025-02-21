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

	printf("tokens    : ");
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
	    printf("expression: ");
	    expr->print();
	    printf("\n");
	    if (expr->can_evaluate()) {
		printf("answer    : ");
		double value = expr->evaluate();
		print_number(value);
	    } else {
		expr->simplify(expr);
		printf("simplified: ");
		expr->print();
	    }
	    printf("\n");
	}
    }
    return 0;
}
