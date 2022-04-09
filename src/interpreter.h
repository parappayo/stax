#pragma once

struct stax_instruction;

void stax_exec(const struct stax_instruction* instructions, int instruction_count);
