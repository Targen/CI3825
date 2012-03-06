#ifndef MULTIDOC_PARSE_X86_H
#define MULTIDOC_PARSE_X86_H

#include "opcodes_x86.h"

typedef opcode_t token_t[];

int parse_asm_x86(const char * code);

#endif
