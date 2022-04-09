#pragma once

#include <stdbool.h>

bool is_whitespace(char c);
bool is_alphanum(char c);
void inline_to_lower(char* text);
char* scan_alphanum(const char* input);
