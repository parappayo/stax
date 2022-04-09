#pragma once

const int MAX_LINE_LENGTH;
const int MAX_TOKENS;

enum token_type {
	TOKEN_VALUE,
	TOKEN_LEFT_BRACKET,
	TOKEN_RIGHT_BRACKET,
	TOKEN_COMMA,
	TOKEN_INT32,
	TOKEN_ADD,
	TOKEN_EMIT,
};

struct token {
	enum token_type type;
	char* text;
};

int tokenize_file(struct token* tokens, int max_len, char* filename);
void free_tokens(struct token* tokens, int count);
