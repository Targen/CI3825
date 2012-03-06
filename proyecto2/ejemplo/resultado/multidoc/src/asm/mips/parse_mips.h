#ifndef MULTIDOC_PARSE_MIPS_H
#define MULTIDOC_PARSE_MIPS_H

#include "opcodes_mips.h"

typedef opcode_t token_t[];

int parse_asm_mips(const char * code);

#endif
