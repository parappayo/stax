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

enum stax_parser_state {
	STATE_ROOT,
	STATE_PARSING_INT32,
};

void parse_value(
		enum stax_parser_state state,
		struct stax_instruction* dest,
		const struct stax_token* src) {

	dest->type = INSTR_PUSH;

	switch (state) {
		case STATE_PARSING_INT32:
			dest->data.type = STAX_INT32;
			dest->data.as_int32 = atoi(src->text);
			printf("found int32 with value %d\n", dest->data.as_int32);
			break;

		default:
			printf("invalid parser state: value has no type\n");
			exit(1);
	}
}

uint64_t parse_tokens(
		struct stax_instruction* instructions,
		const int instructions_size,
		struct stax_token* tokens,
		const int token_count) {
	uint64_t instruction_count = 0;
	enum stax_parser_state state = STATE_ROOT;

	for (const struct stax_token* t = tokens; (t - tokens) < token_count; ) {
		switch (t->type) {
			case TOKEN_VALUE:
				parse_value(state, instructions + instruction_count, t);
				instruction_count++;
				break;

			case TOKEN_LEFT_BRACKET:
				printf("unexpected left bracket\n");
				exit(1);
				break;

			case TOKEN_RIGHT_BRACKET:
				// TODO: ensure the state is one of the data parsing states
				state = STATE_ROOT;
				break;

			case TOKEN_COMMA:
				printf("parsing comma\n");
				break;

			case TOKEN_INT32:
				{
					const int tokens_left = token_count - (t - tokens);
					printf("tokens left = %d\n", tokens_left);
					if (tokens_left < 3) {
						printf("unexpected end of input\n");
						exit(1);
					}
					t++;
					if (t->type != TOKEN_LEFT_BRACKET) {
						printf("expected left bracket after data type\n");
						exit(1);
					}
					state = STATE_PARSING_INT32;
				}
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

	struct stax_token tokens[MAX_TOKENS];
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
