#include "stax_state.h"

void stax_init_state(struct stax_state* state) {
	state->top = state->stack;
}

void stax_free_state(struct stax_state* state) {
	// do nothing
}
