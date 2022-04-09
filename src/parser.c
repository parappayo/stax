#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tokenizer.h"

// TODO: add some funcs that capture the BNF rules

int main(int argc, char* argv[]) {
	if (argc < 2) {
		printf("path to stax source file expected as a command-line argument\n");
		exit(1);
	}

	struct token tokens[MAX_TOKENS];
	printf("sizeof(tokens) = %zu\n", sizeof(tokens));
	int token_count = tokenize_file(tokens, MAX_TOKENS, argv[1]);

	free_tokens(tokens, token_count);
}
