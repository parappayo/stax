#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tokenizer.h"

enum stax_data_type {
	STAX_BYTE,
	STAX_INT32,
	STAX_INT64,
	STAX_UINT32,
	STAX_UINT64,
	STAX_FLOAT_32,
	STAX_FLOAT_64,
	STAX_TYPE,
};

enum stax_instruction_type {
	INSTR_PUSH,
	INSTR_ADD,
	INSTR_EMIT,
};

struct stax_data {
	enum stax_data_type type;

	union {
		int8_t as_byte;
		int16_t as_int16;
		int32_t as_int32;
		int64_t as_int64;
		uint16_t as_uint16;
		uint32_t as_uint32;
		uint64_t as_uint64;
		// TODO: type
	};
};

struct stax_instruction {
	enum stax_instruction_type type;
	struct stax_data data;
};

uint64_t parse_tokens(
		struct stax_instruction* instructions,
		const int instructions_size,
		struct token* tokens,
		const int token_count) {
	uint64_t instruction_count = 0;
	for (const struct token* t = tokens; (t - tokens) < token_count; ) {
		switch (t->type) {
			case TOKEN_VALUE:
				printf("parsing value: %s\n", t->text);
				break;

			case TOKEN_LEFT_BRACKET:
			case TOKEN_RIGHT_BRACKET:
				printf("parsing bracket\n");
				break;

			case TOKEN_COMMA:
				printf("parsing comma\n");
				break;

			case TOKEN_INT32:
				printf("parsing int32\n");
				break;

			case TOKEN_ADD:
				printf("parsing add\n");
				break;

			case TOKEN_EMIT:
				printf("parsing emit\n");
				break;

			default:
				printf("unrecognized token\n");
				exit(1);
		}

		t++;
	}
	return instruction_count;
}

int main(int argc, char* argv[]) {
	if (argc < 2) {
		printf("path to stax source file expected as a command-line argument\n");
		exit(1);
	}

	struct token tokens[MAX_TOKENS];
	printf("sizeof(tokens) = %zu\n", sizeof(tokens));
	int token_count = tokenize_file(tokens, MAX_TOKENS, argv[1]);
	printf("token count = %d\n", token_count);

	const int MAX_INSTRUCTIONS = 4096;
	struct stax_instruction instructions[MAX_INSTRUCTIONS];
	printf("sizeof(instructions) = %zu\n", sizeof(instructions));
	uint64_t instruction_count = parse_tokens(instructions, MAX_INSTRUCTIONS, tokens, token_count);
	printf("instruction count = %lu\n", instruction_count);

	free_tokens(tokens, token_count);
}
