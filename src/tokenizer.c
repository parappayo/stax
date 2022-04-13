#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "str_util.h"
#include "tokenizer.h"

const int MAX_LINE_LENGTH = 2048;
const int MAX_TOKENS = 2048;

struct reserved_word_tuple {
	const char* text;
	enum stax_token_type token_type;
};

struct reserved_word_tuple reserved_words[] = {
	{ "int32",    TOKEN_INT32 },
	{ "int64",    TOKEN_INT64 },
	{ "uint32",   TOKEN_UINT32 },
	{ "uint64",   TOKEN_UINT64 },
	{ "float",    TOKEN_FLOAT },
	{ "double",   TOKEN_DOUBLE },
	{ "add",      TOKEN_ADD },
	{ "sub",      TOKEN_SUB },
	{ "mul",      TOKEN_MUL },
	{ "div",      TOKEN_DIV },
	{ "mod",      TOKEN_MOD },
	{ "emit",     TOKEN_EMIT },
};

void init_tokenizer_state(struct tokenizer_state* state) {
	state->state_enum = STAX_TOKENIZER_STATE_NORMAL;
	state->max_token_count = MAX_TOKENS;
	state->tokens = malloc(state->max_token_count);
	state->next_token = state->tokens;
}

/**
 *  char* text is allocated by caller, will be unallocated by free_tokenizer_state
 */
void add_token(
		struct tokenizer_state* state,
		enum stax_token_type type,
		char* text,
		unsigned int line_number) {

	if (state->next_token >= state->tokens + state->max_token_count) {
		printf("error: ran out of token space\n");
		exit(1);
	}

	printf("found token: %s\n", text);

	struct stax_token* t = state->next_token++;
	t->type = type;
	t->text = text;
	t->line_number = line_number;
}

void free_tokenizer_state(struct tokenizer_state* state) {
	for (struct stax_token* t = state->tokens; t < state->next_token; t++) {
		free(t->text);
	}

	free(state->tokens);
}

void tokenize_line(
		struct tokenizer_state* state,
		const char* line,
		unsigned int line_number) {

	const char* next_char = line;
	while (*next_char != '\0') {
		const char c = *next_char;

		switch (state->state_enum) {
			case STAX_TOKENIZER_STATE_NORMAL:
				{
					if (is_whitespace(c)) {
						// do nothing
					} else if (is_alphanum(c)) {
						char* t = scan_alphanum(next_char);
						inline_to_lower(t);

						const int reserved_word_count = sizeof(reserved_words) / sizeof(struct reserved_word_tuple);
						bool found_reserved_word = false;
						for (int i = 0; i < reserved_word_count; i++) {
							if (strcmp(t, reserved_words[i].text) == 0) {
								add_token(state, reserved_words[i].token_type, t, line_number);
								found_reserved_word = true;
								break;
							}
						}

						if (!found_reserved_word) {
							// TODO: if this is not a valid value, put TOKEN_INVALID or die here
							add_token(state, TOKEN_VALUE, t, line_number);
						}

						next_char += strlen(t) - 1;
					} else if (c == '{') {
						add_token(state, TOKEN_LEFT_BRACKET, NULL, line_number);
					} else if (c == '}') {
						add_token(state, TOKEN_RIGHT_BRACKET, NULL, line_number);
					} else if (c == ',') {
						add_token(state, TOKEN_COMMA, NULL, line_number);
					} else if (c == '#') {
						state->state_enum = STAX_TOKENIZER_STATE_COMMENT;
					} else {
						printf("line %u unexpected token: %c\n", line_number, *next_char);
						exit(1);
					}
				}
				break;

			case STAX_TOKENIZER_STATE_COMMENT:
				{
					if (c == '\n') {
						state->state_enum = STAX_TOKENIZER_STATE_NORMAL;
					}
				}
				break;
		}

		next_char++;
	}
}

int tokenize_file(struct tokenizer_state* state, char* filename) {
	char line[MAX_LINE_LENGTH];
	FILE* infile = fopen(filename, "r");

	unsigned int line_number = 0;
	while (fgets(line, sizeof(line), infile) != NULL) {
		tokenize_line(state, line, line_number++);
	}
	fclose(infile);

	const int token_count = state->next_token - state->tokens;
	return token_count;
}
