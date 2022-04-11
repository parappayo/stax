#include <stdio.h>
#include <stdlib.h>

#include "stax_state.h"

const int STACK_SIZE = 4096;

void stax_init_state(struct stax_state* state) {
	state->top = state->stack;
}

void stax_free_state(struct stax_state* state) {
	// do nothing
}

void stax_state_push(struct stax_state* state, const struct stax_data* data) {
	if (state->top >= state->stack + STACK_SIZE) {
		printf("error: stack overflow\n");
		exit(1);
	}
	*(state->top) = *data;
	state->top++;
}

struct stax_data* stax_state_pop(struct stax_state* state) {
	if (state->top <= state->stack) {
		printf("error: tried to pop empty stack\n");
		exit(1);
	}
	state->top--;
	return state->top;
}
