#pragma once

const int MAX_LINE_LENGTH;

enum stax_token_type {
	TOKEN_INVALID,
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

enum tokenizer_state_enum {
	STAX_TOKENIZER_STATE_NORMAL,
	STAX_TOKENIZER_STATE_COMMENT,
};

struct tokenizer_state {
	enum tokenizer_state_enum state_enum;
	struct stax_token* tokens;
	struct stax_token* next_token;
	int max_token_count;
};

void init_tokenizer_state(struct tokenizer_state* state);
void free_tokenizer_state(struct tokenizer_state* state);

int tokenize_file(struct tokenizer_state* state, char* filename);
