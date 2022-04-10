#pragma once

#include "stax_data.h"

struct stax_state;
struct stax_instruction;

void stax_exec(struct stax_state* state, const struct stax_instruction* instructions, int instruction_count);
