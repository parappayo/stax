#pragma once

const int MAX_LINE_LENGTH;
const int MAX_TOKENS;

enum stax_token_type {
	TOKEN_VALUE,
	TOKEN_LEFT_BRACKET,
	TOKEN_RIGHT_BRACKET,
	TOKEN_COMMA,
	TOKEN_INT32,
	TOKEN_INT64,
	TOKEN_UINT32,
	TOKEN_UINT64,
	TOKEN_FLOAT,
	TOKEN_DOUBLE,
	TOKEN_ADD,
	TOKEN_SUB,
	TOKEN_MUL,
	TOKEN_DIV,
	TOKEN_MOD,
	TOKEN_EMIT,
};

struct stax_token {
	enum stax_token_type type;
	char* text;
	unsigned int line_number;
};

int tokenize_file(struct stax_token* tokens, int max_token_count, char* filename);
void free_tokens(struct stax_token* tokens, int count);
