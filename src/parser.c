#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"
#include "tokenizer.h"

void parse_value(
		enum stax_parser_state state,
		struct stax_instruction* dest,
		const struct stax_token* src) {

	dest->type = STAX_INSTR_PUSH;

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

int parse_tokens(
		struct stax_instruction* instructions,
		const int instructions_size,
		const struct stax_token* tokens,
		const int token_count) {
	int instruction_count = 0;
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
				{
					struct stax_instruction* i = &instructions[instruction_count];
					i->type = STAX_INSTR_ADD;
					i->data.type = STAX_VOID;
					i->data.as_int64 = 0;
					instruction_count++;
				}
				break;

			case TOKEN_EMIT:
				{
					struct stax_instruction* i = &instructions[instruction_count];
					i->type = STAX_INSTR_EMIT;
					i->data.type = STAX_VOID;
					i->data.as_int64 = 0;
					instruction_count++;
				}
				break;

			default:
				printf("unrecognized token\n");
				exit(1);
		}

		t++;
	}
	return instruction_count;
}

void free_instructions(
		struct stax_instruction* instructions,
		const int instruction_count) {
	// for now, nothing to do
}
