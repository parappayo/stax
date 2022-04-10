#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "interpreter.h"
#include "parser.h"
#include "stax_instruction.h"
#include "stax_state.h"

struct stax_data stax_add(const struct stax_data* a, const struct stax_data* b) {
	struct stax_data result;

	if (a->type != b->type) {
		printf(
			"could not add, types do not match: %s, %s\n",
			stax_data_type_to_string(a->type),
			stax_data_type_to_string(b->type));
		exit(1);
	}

	result.type = a->type;

	switch (a->type) {
		case STAX_INT32:
			result.as_int32 = a->as_int32 + b->as_int32;
			break;

		case STAX_INT64:
			result.as_int64 = a->as_int64 + b->as_int64;
			break;

		case STAX_UINT32:
			result.as_uint32 = a->as_uint32 + b->as_uint32;
			break;

		case STAX_UINT64:
			result.as_uint64 = a->as_uint64 + b->as_uint64;
			break;

		default:
			printf(
				"could not add, unsupported data type: %s\n",
				stax_data_type_to_string(a->type));
			exit(1);
	}

	return result;
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
					stax_state_push(state, &instr->data);
				}
				break;

			case STAX_INSTR_ADD:
				{
					const struct stax_data* a = stax_state_pop(state);
					const struct stax_data* b = stax_state_pop(state);
					// TODO: free a and b at the right time (reference types need to be freed after pop)

					struct stax_data result = stax_add(a, b);
					stax_state_push(state, &result);
				}
				break;

			case STAX_INSTR_EMIT:
				{
					printf("exec emit\n");
					const struct stax_data* a = stax_state_pop(state);
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
