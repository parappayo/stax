#pragma once

const int MAX_LINE_LENGTH;
const int MAX_TOKENS;

enum stax_token_type {
	TOKEN_VALUE,
	TOKEN_LEFT_BRACKET,
	TOKEN_RIGHT_BRACKET,
	TOKEN_COMMA,
	TOKEN_INT32,
	TOKEN_ADD,
	TOKEN_EMIT,
};

struct stax_token {
	enum stax_token_type type;
	char* text;
};

int tokenize_file(struct stax_token* tokens, int max_len, char* filename);
void free_tokens(struct stax_token* tokens, int count);
