#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stax_instruction.h"
#include "parser.h"
#include "tokenizer.h"

enum stax_parser_state {
	STATE_ROOT,
	STATE_PARSING_INT32,
	STATE_PARSING_INT64,
	STATE_PARSING_UINT32,
	STATE_PARSING_UINT64,
	STATE_PARSING_FLOAT,
	STATE_PARSING_DOUBLE,
};

void parse_value(
		enum stax_parser_state state,
		struct stax_instruction* dest,
		const struct stax_token* src) {

	dest->type = STAX_INSTR_PUSH;

	char* str_end;
	const int base = 10;

	switch (state) {
		case STATE_PARSING_INT32:
			dest->data.type = STAX_INT32;
			dest->data.as_int32 = (int32_t) strtol(src->text, &str_end, base);
			break;

		case STATE_PARSING_INT64:
			dest->data.type = STAX_INT64;
			dest->data.as_int64 = strtol(src->text, &str_end, base);
			break;

		case STATE_PARSING_UINT32:
			dest->data.type = STAX_UINT32;
			dest->data.as_uint32 = (uint32_t) strtoul(src->text, &str_end, base);
			break;

		case STATE_PARSING_UINT64:
			dest->data.type = STAX_UINT64;
			dest->data.as_uint64 = strtol(src->text, &str_end, base);
			break;

		case STATE_PARSING_FLOAT:
			dest->data.type = STAX_FLOAT;
			dest->data.as_float = strtof(src->text, &str_end);
			break;

		case STATE_PARSING_DOUBLE:
			dest->data.type = STAX_DOUBLE;
			dest->data.as_double = strtod(src->text, &str_end);
			break;

		default:
			printf("error: line %u unrecognized data type %s\n", src->line_number, src->text);
			exit(1);
	}
}

void check_start_of_value(
		const struct stax_token* current_token,
		const struct stax_token* tokens,
		const int token_count) {

	const int tokens_left = token_count - (current_token - tokens);

	if (tokens_left < 3) {
		printf("error: unexpected end of input\n");
		exit(1);
	}

	if ((current_token + 1)->type != TOKEN_LEFT_BRACKET) {
		printf("error: line %u expected left bracket after data type\n", current_token->line_number);
		exit(1);
	}
}

void set_non_value_instruction(
		struct stax_instruction* instr,
		enum stax_instruction_type instr_type) {
	instr->type = instr_type;
	instr->data.type = STAX_VOID;
	instr->data.as_int64 = 0;
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
				printf("error: line %u unexpected left bracket\n", t->line_number);
				exit(1);
				break;

			case TOKEN_RIGHT_BRACKET:
				// TODO: ensure the state is one of the data parsing states
				state = STATE_ROOT;
				break;

			case TOKEN_COMMA:
				// TODO: ensure the state is one of the data parsing states
				break;

			case TOKEN_INT32:
				{
					check_start_of_value(t++, tokens, token_count);
					state = STATE_PARSING_INT32;
				}
				break;

			case TOKEN_INT64:
				{
					check_start_of_value(t++, tokens, token_count);
					state = STATE_PARSING_INT64;
				}
				break;

			case TOKEN_UINT32:
				{
					check_start_of_value(t++, tokens, token_count);
					state = STATE_PARSING_UINT32;
				}
				break;

			case TOKEN_UINT64:
				{
					check_start_of_value(t++, tokens, token_count);
					state = STATE_PARSING_UINT64;
				}
				break;

			case TOKEN_FLOAT:
				{
					check_start_of_value(t++, tokens, token_count);
					state = STATE_PARSING_FLOAT;
				}
				break;

			case TOKEN_DOUBLE:
				{
					check_start_of_value(t++, tokens, token_count);
					state = STATE_PARSING_DOUBLE;
				}
				break;

			case TOKEN_ADD:
				set_non_value_instruction(&instructions[instruction_count++], STAX_INSTR_ADD);
				break;

			case TOKEN_SUB:
				set_non_value_instruction(&instructions[instruction_count++], STAX_INSTR_SUB);
				break;

			case TOKEN_MUL:
				set_non_value_instruction(&instructions[instruction_count++], STAX_INSTR_MUL);
				break;

			case TOKEN_DIV:
				set_non_value_instruction(&instructions[instruction_count++], STAX_INSTR_DIV);
				break;

			case TOKEN_MOD:
				set_non_value_instruction(&instructions[instruction_count++], STAX_INSTR_MOD);
				break;

			case TOKEN_EMIT:
				set_non_value_instruction(&instructions[instruction_count++], STAX_INSTR_EMIT);
				break;

			default:
				printf("error: unrecognized token\n");
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
