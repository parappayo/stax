#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "interpreter.h"
#include "parser.h"
#include "stax_instruction.h"
#include "stax_state.h"

const int STACK_SIZE = 4096;

void stax_push(struct stax_state* state, const struct stax_data* data) {
	if (state->top >= state->stack + STACK_SIZE) {
		printf("stack overflow\n");
		exit(1);
	}
	*(state->top) = *data;
	printf("pushed: ");
	stax_print_data(state->top);
	state->top++;
}

const struct stax_data* stax_pop(struct stax_state* state) {
	if (state->top <= state->stack) {
		printf("tried to pop empty stack\n");
		exit(1);
	}
	state->top--;
	printf("popped: ");
	stax_print_data(state->top);
	return state->top;
}

void stax_exec(
		struct stax_state* state,
		const struct stax_instruction* instructions,
		int instruction_count) {

	for (int i = 0; i < instruction_count; i++) {
		const struct stax_instruction* instr = &instructions[i];

		printf("executing: ");
		stax_print_instruction(instr);

		switch (instr->type) {
			case STAX_INSTR_PUSH:
				{
					stax_push(state, &instr->data);
				}
				break;

			case STAX_INSTR_ADD:
				{
					const struct stax_data* a = stax_pop(state);
					const struct stax_data* b = stax_pop(state);
					// TODO: free a and b at the right time

					if (a->type != b->type) {
						printf(
							"could not add, types do not match: %s, %s\n",
							stax_data_type_to_string(a->type),
							stax_data_type_to_string(b->type));
						exit(1);
					}
					if (a->type != STAX_INT32) {
						printf(
							"could not add, unsupported data type: %s\n",
							stax_data_type_to_string(a->type));
						exit(1);
					}
					int32_t result = a->as_int32 + b->as_int32;
					state->top->type = STAX_INT32;
					state->top->as_int32 = result;
					state->top++;
				}
				break;

			case STAX_INSTR_EMIT:
				{
					printf("exec emit\n");
					const struct stax_data* a = stax_pop(state);
					if (a->type == STAX_INT32) {
						printf("%d\n", a->as_int32);
					}
					// TODO: free a
				}
				break;

			default:
				{
					printf("unrecognized instruction type\n");
					exit(1);
				}
				break;
		}
	}
}
