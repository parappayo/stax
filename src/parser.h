#pragma once

#include <stdint.h>

struct stax_token;

enum stax_data_type {
	STAX_VOID,
	STAX_BYTE,
	STAX_INT32,
	STAX_INT64,
	STAX_UINT32,
	STAX_UINT64,
	STAX_FLOAT_32,
	STAX_FLOAT_64,
	STAX_TYPE,
};

enum stax_instruction_type {
	INSTR_PUSH,
	INSTR_ADD,
	INSTR_EMIT,
};

struct stax_data {
	enum stax_data_type type;

	union {
		int8_t as_byte;
		int16_t as_int16;
		int32_t as_int32;
		int64_t as_int64;
		uint16_t as_uint16;
		uint32_t as_uint32;
		uint64_t as_uint64;
		// TODO: type
	};
};

struct stax_instruction {
	enum stax_instruction_type type;
	struct stax_data data;
};

enum stax_parser_state {
	STATE_ROOT,
	STATE_PARSING_INT32,
};

int parse_tokens(
	struct stax_instruction* instructions,
	const int instructions_size,
	const struct stax_token* tokens,
	const int token_count);

void free_instructions(
	struct stax_instruction* instructions,
	const int instruction_count);
