#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "interpreter.h"
#include "parser.h"

const int STACK_SIZE = 4096;

void stax_init_state(struct stax_state* state) {
	state->top = state->stack;
}

void stax_free_state(struct stax_state* state) {
	// do nothing
}

void stax_push(struct stax_state* state, const struct stax_data* data) {
	if (state->top >= state->stack + STACK_SIZE) {
		printf("stack overflow\n");
		exit(1);
	}
	*(state->top) = *data;
	state->top++;
}

const struct stax_data* stax_pop(struct stax_state* state) {
	if (state->top <= state->stack) {
		printf("tried to pop empty stack\n");
		exit(1);
	}
	struct stax_data* result = state->top;
	state->top--;
	return result;
}

void stax_exec(
		struct stax_state* state,
		const struct stax_instruction* instructions,
		int instruction_count) {

	for (int i = 0; i < instruction_count; i++) {
		const struct stax_instruction* instr = &instructions[i];

		switch (instr->type) {
			case STAX_INSTR_PUSH:
				{
					printf("exec push\n");
					stax_push(state, &instr->data);
				}
				break;

			case STAX_INSTR_ADD:
				{
					printf("exec add\n");
					const struct stax_data* a = stax_pop(state);
					const struct stax_data* b = stax_pop(state);
					// TODO: free a and b at the right time
					if (a->type != STAX_INT32 || b->type != STAX_INT32) {
						printf("could not add, unsupported data type\n");
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
