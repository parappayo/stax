#pragma once

#include "stax_data.h"

struct stax_instruction;

struct stax_state {
	struct stax_data* top;
	struct stax_data stack[4096];
};

void stax_init_state(struct stax_state* state);
void stax_free_state(struct stax_state* state);
void stax_exec(struct stax_state* state, const struct stax_instruction* instructions, int instruction_count);
