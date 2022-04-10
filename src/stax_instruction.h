#pragma once

#include "stax_data.h"

enum stax_instruction_type {
	STAX_INSTR_PUSH,
	STAX_INSTR_ADD,
	STAX_INSTR_EMIT,
};

struct stax_instruction {
	enum stax_instruction_type type;
	struct stax_data data;
};

const char* stax_instruction_type_to_string(enum stax_instruction_type instr_type);
void stax_print_instruction(const struct stax_instruction* instr);
