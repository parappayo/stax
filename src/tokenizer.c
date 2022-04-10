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

void add_token(
		struct stax_token* tokens,
		int token_index,
		enum stax_token_type type,
		char* text,
		unsigned int line_number) {
	struct stax_token* t = &tokens[token_index];
	t->type = type;
	t->text = text;
	t->line_number = line_number;
}

void free_tokens(struct stax_token* tokens, int count) {
	for (int i = 0; i < count; i++) {
		free(tokens[i].text);
	}
}

int tokenize_line(
		struct stax_token* tokens,
		int max_token_count,
		const char* line,
		unsigned int line_number) {
	int token_count = 0;

	const char* next_char = line;
	while (*next_char != '\0' && token_count < max_token_count) {
		const char c = *next_char;

		if (is_whitespace(c)) {
			// skip
		} else if (is_alphanum(c)) {
			char* t = scan_alphanum(next_char);
			inline_to_lower(t);

			const int reserved_word_count = sizeof(reserved_words) / sizeof(struct reserved_word_tuple);
			bool found_reserved_word = false;
			for (int i = 0; i < reserved_word_count; i++) {
				if (strcmp(t, reserved_words[i].text) == 0) {
					add_token(tokens, token_count++, reserved_words[i].token_type, t, line_number);
					found_reserved_word = true;
					break;
				}
			}

			if (!found_reserved_word) {
				add_token(tokens, token_count++, TOKEN_VALUE, t, line_number);
			}

			next_char += strlen(t) - 1;
		} else if (c == '{') {
			add_token(tokens, token_count++, TOKEN_LEFT_BRACKET, NULL, line_number);
		} else if (c == '}') {
			add_token(tokens, token_count++, TOKEN_RIGHT_BRACKET, NULL, line_number);
		} else if (c == ',') {
			add_token(tokens, token_count++, TOKEN_COMMA, NULL, line_number);
		} else {
			printf("line %u unexpected token: %c\n", line_number, *next_char);
			exit(1);
		}
		next_char++;
	}

	return token_count;
}

int tokenize_file(struct stax_token* tokens, int max_token_count, char* filename) {
	int token_count = 0;
	char line[MAX_LINE_LENGTH];
	FILE* infile = fopen(filename, "r");

	unsigned int line_number = 0;
	while (fgets(line, sizeof(line), infile) != NULL) {
		token_count += tokenize_line(
			tokens + token_count,
			max_token_count - token_count,
			line,
			line_number++);
	}
	fclose(infile);

	return token_count;
}
