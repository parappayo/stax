#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

const int MAX_LINE_LENGTH = 2048;
const int MAX_TOKENS = 2048;

enum token {
	LEFT_PAREN,
	RIGHT_PAREN,
	COMMA,
};

bool is_whitespace(char c) {
	switch (c) {
		case ' ':
		case '\n':
		case '\t':
			return true;
	}
	return false;
}

bool is_alphanum(char c) {
	return (c >= '0' && c <= '9') ||
		(c >= 'a' && c <= 'z') ||
		(c >= 'A' && c <= 'Z');
}

void inline_to_lower(char* text) {
	for (char* c = text; *c != '\0'; c++) {
		if (*c >= 'A' && *c <= 'Z') {
			*c = *c + 'a' - 'A';
		}
	}
}

char* read_consecutive_alphanum(const char* input) {
	const char* end;
	for (end = input; is_alphanum(*end); end++) {}
	const size_t len = end - input;

	char* copy = malloc(len);
	memcpy(copy, input, len);
	return copy;
}

int tokenize_line(enum token* tokens, int max_len, const char* line) {
	printf("tokenizing: %s", line);

	int token_index = 0;
	const char* next_char = line;
	while (*next_char != '\0' && token_index < max_len) {
		const char c = *next_char;

		if (is_whitespace(c)) {
			// skip
		} else if (is_alphanum(c)) {
			char* t = read_consecutive_alphanum(next_char);
			inline_to_lower(t);
			printf("found token: %s\n", t);
			next_char += strlen(t) - 1;
			// TODO: instead of freeing here, we need to add a token to the list here
			free(t);
		} else if (c == '(') {
			printf("found left paren\n");
			tokens[token_index++] = LEFT_PAREN;
		} else if (c == ')') {
			printf("found right paren\n");
			tokens[token_index++] = RIGHT_PAREN;
		} else if (c == ',') {
			printf("found comma paren\n");
			tokens[token_index++] = COMMA;
		} else {
			printf("unexpected char: %c\n", *next_char);
		}
		next_char++;
	}

	printf("found %d tokens\n", token_index);
	return token_index;
}

void tokenize_file(enum token* tokens, int max_len, char* filename) {
	int token_index = 0;
	char line[MAX_LINE_LENGTH];
	FILE* infile = fopen(filename, "r");
	while (fgets(line, sizeof(line), infile) != NULL) {
		token_index += tokenize_line(
			tokens + token_index,
			max_len - token_index,
			line);
	}
	fclose(infile);	
}

int main(int argc, char* argv[]) {
	if (argc < 2) {
		exit(0);
	}

	enum token tokens[MAX_TOKENS];
	printf("sizeof(tokens) = %zu\n", sizeof(tokens));
	tokenize_file(tokens, MAX_TOKENS, argv[1]);
}
