#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "str_util.h"
#include "tokenizer.h"

const int MAX_LINE_LENGTH = 2048;
const int MAX_TOKENS = 2048;

void add_token(
		struct token* tokens,
		int token_index,
		enum token_type type,
		char* text) {
	struct token* t = &tokens[token_index];
	t->type = type;
	t->text = text;
}

void free_tokens(struct token* tokens, int count) {
	for (int i = 0; i < count; i++) {
		free(tokens[i].text);
	}
}

int tokenize_line(struct token* tokens, int max_len, const char* line) {
	printf("tokenizing: %s", line);

	int token_index = 0;
	const char* next_char = line;
	while (*next_char != '\0' && token_index < max_len) {
		const char c = *next_char;

		if (is_whitespace(c)) {
			// skip
		} else if (is_alphanum(c)) {
			char* t = scan_alphanum(next_char);
			inline_to_lower(t);

			if (strcmp(t, "int32") == 0) {
				add_token(tokens, token_index++, TOKEN_INT32, t);
			} else if (strcmp(t, "add") == 0) {
				add_token(tokens, token_index++, TOKEN_ADD, t);
			} else if (strcmp(t, "emit") == 0) {
				add_token(tokens, token_index++, TOKEN_EMIT, t);
			} else {
				add_token(tokens, token_index++, TOKEN_VALUE, t);
			}

			next_char += strlen(t) - 1;
		} else if (c == '{') {
			add_token(tokens, token_index++, TOKEN_LEFT_BRACKET, NULL);
		} else if (c == '}') {
			add_token(tokens, token_index++, TOKEN_RIGHT_BRACKET, NULL);
		} else if (c == ',') {
			add_token(tokens, token_index++, TOKEN_COMMA, NULL);
		} else {
			printf("unexpected token: %c\n", *next_char);
			exit(1);
		}
		next_char++;
	}

	printf("found %d tokens\n", token_index);
	return token_index;
}

int tokenize_file(struct token* tokens, int max_len, char* filename) {
	int token_count = 0;
	char line[MAX_LINE_LENGTH];
	FILE* infile = fopen(filename, "r");

	while (fgets(line, sizeof(line), infile) != NULL) {
		token_count += tokenize_line(
			tokens + token_count,
			max_len - token_count,
			line);
	}
	fclose(infile);

	return token_count;
}
