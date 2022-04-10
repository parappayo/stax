#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "interpreter.h"
#include "parser.h"
#include "stax_instruction.h"
#include "stax_state.h"

int32_t int32_arithmetic_op(int32_t a, int32_t b, enum stax_instruction_type instr_type) {
	switch (instr_type) {
		case STAX_INSTR_ADD: return a + b;
		case STAX_INSTR_SUB: return a - b;
		case STAX_INSTR_MUL: return a * b;
		case STAX_INSTR_DIV: return a / b;
		case STAX_INSTR_MOD: return a % b;
		default:
			printf("invalid arithmetic instruction state\n");
			exit(1);
	}
	return 0;
}

int64_t int64_arithmetic_op(int64_t a, int64_t b, enum stax_instruction_type instr_type) {
	switch (instr_type) {
		case STAX_INSTR_ADD: return a + b;
		case STAX_INSTR_SUB: return a - b;
		case STAX_INSTR_MUL: return a * b;
		case STAX_INSTR_DIV: return a / b;
		case STAX_INSTR_MOD: return a % b;
		default:
			printf("invalid arithmetic instruction state\n");
			exit(1);
	}
	return 0;
}

uint32_t uint32_arithmetic_op(uint32_t a, uint32_t b, enum stax_instruction_type instr_type) {
	switch (instr_type) {
		case STAX_INSTR_ADD: return a + b;
		case STAX_INSTR_SUB: return a - b;
		case STAX_INSTR_MUL: return a * b;
		case STAX_INSTR_DIV: return a / b;
		case STAX_INSTR_MOD: return a % b;
		default:
			printf("invalid arithmetic instruction state\n");
			exit(1);
	}
	return 0;
}

uint64_t uint64_arithmetic_op(uint64_t a, uint64_t b, enum stax_instruction_type instr_type) {
	switch (instr_type) {
		case STAX_INSTR_ADD: return a + b;
		case STAX_INSTR_SUB: return a - b;
		case STAX_INSTR_MUL: return a * b;
		case STAX_INSTR_DIV: return a / b;
		case STAX_INSTR_MOD: return a % b;
		default:
			printf("invalid arithmetic instruction state\n");
			exit(1);
	}
	return 0;
}

struct stax_data stax_arithmetic_op(
		const struct stax_data* a,
		const struct stax_data* b,
		enum stax_instruction_type instr_type) {
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
			result.as_int32 = int32_arithmetic_op(a->as_int32, b->as_int32, instr_type);
			break;

		case STAX_INT64:
			result.as_int64 = int64_arithmetic_op(a->as_int64, b->as_int64, instr_type);
			break;

		case STAX_UINT32:
			result.as_uint32 = uint32_arithmetic_op(a->as_uint32, b->as_uint32, instr_type);
			break;

		case STAX_UINT64:
			result.as_uint64 = uint64_arithmetic_op(a->as_uint64, b->as_uint64, instr_type);
			break;

		// TODO: case STAX_FLOAT
		// TODO: case STAX_DOUBLE

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
			case STAX_INSTR_SUB:
			case STAX_INSTR_MUL:
			case STAX_INSTR_DIV:
			case STAX_INSTR_MOD:
				{
					struct stax_data* a = stax_state_pop(state);
					struct stax_data* b = stax_state_pop(state);
					struct stax_data result = stax_arithmetic_op(a, b, instr->type);
					stax_free_data(a);
					stax_free_data(b);
					stax_state_push(state, &result);
				}
				break;

			case STAX_INSTR_EMIT:
				{
					printf("exec emit\n");
					const struct stax_data* a = stax_state_pop(state);
					// TODO: add support for numeric data types
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
