#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "parser.h"
#include "tokenizer.h"
#include "utils.h"

static char line[100*1024];

int main(int argc, char **argv) {
    while (fgets(line, sizeof(line), stdin) != NULL) {
	Expression *expr = parse_expression(new Tokenizer(line));
	if (expr != NULL) {
	    expr->print();
	    printf(" = ");
	    print_number(expr->evaluate());
	    printf("\n");
	}
    }
    return 0;
}
