#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "interpreter.h"
#include "parser.h"
#include "stax_instruction.h"
#include "stax_state.h"


#define integer_op_macro_body() \
	switch (instr_type) { \
		case STAX_INSTR_ADD: return a + b; \
		case STAX_INSTR_SUB: return a - b; \
		case STAX_INSTR_MUL: return a * b; \
		case STAX_INSTR_DIV: return a / b; \
		case STAX_INSTR_MOD: return a % b; \
		default: \
			printf("error: invalid arithmetic instruction state\n"); \
			exit(1); \
	}

#define floating_op_macro_body() \
	switch (instr_type) { \
		case STAX_INSTR_ADD: return a + b; \
		case STAX_INSTR_SUB: return a - b; \
		case STAX_INSTR_MUL: return a * b; \
		case STAX_INSTR_DIV: return a / b; \
		case STAX_INSTR_MOD: \
			printf("error: cannot MOD floating point data\n"); \
			exit(1); \
		default: \
			printf("error: invalid arithmetic instruction state\n"); \
			exit(1); \
	}

int32_t int32_arithmetic_op(int32_t a, int32_t b, enum stax_instruction_type instr_type) {
	integer_op_macro_body();
	return 0;
}

int64_t int64_arithmetic_op(int64_t a, int64_t b, enum stax_instruction_type instr_type) {
	integer_op_macro_body();
	return 0;
}

uint32_t uint32_arithmetic_op(uint32_t a, uint32_t b, enum stax_instruction_type instr_type) {
	integer_op_macro_body();
	return 0;
}

uint64_t uint64_arithmetic_op(uint64_t a, uint64_t b, enum stax_instruction_type instr_type) {
	integer_op_macro_body();
	return 0;
}

float float_arithmetic_op(float a, float b, enum stax_instruction_type instr_type) {
	floating_op_macro_body();
	return 0.0f;
}

double double_arithmetic_op(double a, double b, enum stax_instruction_type instr_type) {
	floating_op_macro_body();
	return 0.0f;
}

struct stax_data stax_arithmetic_op(
		const struct stax_data* a,
		const struct stax_data* b,
		enum stax_instruction_type instr_type) {
	struct stax_data result;

	if (a->type != b->type) {
		printf(
			"error: could not add, types do not match: %s, %s\n",
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

		case STAX_FLOAT:
			result.as_float = float_arithmetic_op(a->as_float, b->as_float, instr_type);
			break;

		case STAX_DOUBLE:
			result.as_double = double_arithmetic_op(a->as_double, b->as_double, instr_type);
			break;

		default:
			printf(
				"error: could not add, unsupported data type: %s\n",
				stax_data_type_to_string(a->type));
			exit(1);
	}

	return result;
}

void stax_emit(const struct stax_data* data) {
	switch (data->type) {

		case STAX_INT32:
			printf("%d\n", data->as_int32);
			break;

		case STAX_UINT32:
			printf("%u\n", data->as_uint32);
			break;

		case STAX_INT64:
			printf("%ld\n", data->as_int64);
			break;

		case STAX_UINT64:
			printf("%lu\n", data->as_uint64);
			break;

		case STAX_FLOAT:
			printf("%f\n", data->as_float);
			break;

		case STAX_DOUBLE:
			printf("%f\n", data->as_double);
			break;

		default:
			printf("error: emit unsupported for type %s\n", stax_data_type_to_string(data->type));
			break;
	}
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
					struct stax_data* a = stax_state_pop(state);
					stax_emit(a);
					stax_free_data(a);
				}
				break;

			default:
				{
					printf("error: unrecognized instruction type\n");
					exit(1);
				}
				break;
		}
	}
}
