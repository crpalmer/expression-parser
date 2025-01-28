#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "tokenizer.h"

static char line[100*1024];

int main(int argc, char **argv) {
    while (fgets(line, sizeof(line), stdin) != NULL) {
	Tokenizer *tokenizer = new Tokenizer(line);
	Token *token;

	while (tokenizer->pop(&token)) {
	    token->print();
	}
	printf("\n");
    }
    return 0;
}
