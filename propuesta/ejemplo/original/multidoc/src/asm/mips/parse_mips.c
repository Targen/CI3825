#include <string.h>

#include "parse_mips.h"

int parse_asm_mips(const char * code) {
        if (strcmp(code, "nop" ) == 0) return 1;
        if (strcmp(code, "beqz") == 0) return 2;

        return 0;
}
