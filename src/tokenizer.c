#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "str_util.h"

const int MAX_LINE_LENGTH = 2048;
const int MAX_TOKENS = 2048;

enum token_type {
	VALUE,
	LEFT_PAREN,
	RIGHT_PAREN,
	COMMA,
};

struct token {
	enum token_type type;
	char* text;
};

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

			printf("found token: %s\n", t);
			add_token(tokens, token_index++, VALUE, t);

			next_char += strlen(t) - 1;
		} else if (c == '(') {
			add_token(tokens, token_index++, LEFT_PAREN, NULL);
		} else if (c == ')') {
			add_token(tokens, token_index++, RIGHT_PAREN, NULL);
		} else if (c == ',') {
			add_token(tokens, token_index++, COMMA, NULL);
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

int main(int argc, char* argv[]) {
	if (argc < 2) {
		printf("path to stax source file expected as a command-line argument\n");
		exit(1);
	}

	struct token tokens[MAX_TOKENS];
	printf("sizeof(tokens) = %zu\n", sizeof(tokens));
	int token_count = tokenize_file(tokens, MAX_TOKENS, argv[1]);

	free_tokens(tokens, token_count);
}
