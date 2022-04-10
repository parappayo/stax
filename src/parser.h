#pragma once

#include <stdint.h>

struct stax_token;

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
