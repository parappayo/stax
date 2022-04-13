#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

char* scan_alphanum(const char* input) {
	const char* end;
	for (end = input; is_alphanum(*end); end++) {}
	const size_t len = end - input;

	char* copy = malloc(len + 1);
	memcpy(copy, input, len);
	copy[len] = '\0';
	return copy;
}
