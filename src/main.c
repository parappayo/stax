#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "interpreter.h"
#include "parser.h"
#include "stax_instruction.h"
#include "stax_state.h"
#include "tokenizer.h"

/**
 *  For now the design is tokenize -> parse -> interpret.
 *  Eventually there should be a separate tool that compiles bytecode,
 *  then the interpreter will use bytecode instead of parsed instructions.
 */
int main(int argc, char* argv[]) {
	if (argc < 2) {
		printf("path to stax source file expected as a command-line argument\n");
		exit(1);
	}

	struct stax_token tokens[MAX_TOKENS];
	printf("sizeof(tokens) = %zu\n", sizeof(tokens));
	int token_count = tokenize_file(tokens, MAX_TOKENS, argv[1]);
	printf("token count = %d\n", token_count);

	const int MAX_INSTRUCTIONS = 4096;
	struct stax_instruction instructions[MAX_INSTRUCTIONS];
	printf("sizeof(instructions) = %zu\n", sizeof(instructions));
	int instruction_count = parse_tokens(instructions, MAX_INSTRUCTIONS, tokens, token_count);
	printf("instruction count = %d\n", instruction_count);

	free_tokens(tokens, token_count);

	struct stax_state state;
	printf("sizeof(state) = %zu\n", sizeof(state));
	stax_init_state(&state);
	stax_exec(&state, instructions, instruction_count);
	stax_free_state(&state);

	free_instructions(instructions, instruction_count);
}
