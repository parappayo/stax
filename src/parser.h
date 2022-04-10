#pragma once

#include <stdint.h>

struct stax_token;

int parse_tokens(
	struct stax_instruction* instructions,
	const int instructions_size,
	const struct stax_token* tokens,
	const int token_count);

void free_instructions(
	struct stax_instruction* instructions,
	const int instruction_count);
