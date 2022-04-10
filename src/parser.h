#pragma once

#include <stdint.h>

#include "stax_data.h"

struct stax_token;

enum stax_instruction_type {
	STAX_INSTR_PUSH,
	STAX_INSTR_ADD,
	STAX_INSTR_EMIT,
};

struct stax_instruction {
	enum stax_instruction_type type;
	struct stax_data data;
};

enum stax_parser_state {
	STATE_ROOT,
	STATE_PARSING_INT32,
};

int parse_tokens(
	struct stax_instruction* instructions,
	const int instructions_size,
	const struct stax_token* tokens,
	const int token_count);

void free_instructions(
	struct stax_instruction* instructions,
	const int instruction_count);
