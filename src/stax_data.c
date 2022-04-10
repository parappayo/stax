#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "stax_data.h"

void stax_free_data(struct stax_data* data) {
	// TODO: handle reference types that need data cleaned up
}

const char* stax_data_type_to_string(enum stax_data_type data_type) {
	switch (data_type) {
		case STAX_VOID:      return "void";
		case STAX_BYTE:      return "byte";
		case STAX_INT32:     return "int32";
		case STAX_INT64:     return "int64";
		case STAX_UINT32:    return "uint32";
		case STAX_UINT64:    return "uint64";
		case STAX_FLOAT:     return "float";
		case STAX_DOUBLE:    return "double";
		case STAX_TYPE:      return "type";
	}

	return "unsupported";
}

void stax_print_data(const struct stax_data* data) {
	printf("%s", stax_data_type_to_string(data->type));
	switch (data->type) {
		case STAX_VOID:
			printf("\n");
			break;

		case STAX_INT32:
			printf(" %d\n", data->as_int32);
			break;

		case STAX_INT64:
			printf(" %ld\n", data->as_int64);
			break;

		case STAX_UINT32:
			printf(" %u\n", data->as_uint32);
			break;

		case STAX_UINT64:
			printf(" %lu\n", data->as_uint64);
			break;

		default:
			printf(" unsupported\n");
			break;
	}
}
