#include <stdio.h>
#include <stdlib.h>

#include "stax_instruction.h"

const char* stax_instruction_type_to_string(enum stax_instruction_type instr_type) {	
	switch (instr_type) {
		case STAX_INSTR_PUSH:  return "push";
		case STAX_INSTR_ADD:   return "add";
		case STAX_INSTR_EMIT:  return "emit";
	}

	return "unsupported";
}

void stax_print_instruction(const struct stax_instruction* instr) {
	switch (instr->type) {
		case STAX_INSTR_PUSH:
			printf("%s ", stax_instruction_type_to_string(instr->type));
			stax_print_data(&instr->data);
			break;

		default:
			printf("%s\n", stax_instruction_type_to_string(instr->type));
			break;
	}
}
