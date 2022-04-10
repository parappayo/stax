#pragma once

#include <stdint.h>

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

const char* stax_data_type_to_string(enum stax_data_type data_type);
void stax_print_data(const struct stax_data* data);
