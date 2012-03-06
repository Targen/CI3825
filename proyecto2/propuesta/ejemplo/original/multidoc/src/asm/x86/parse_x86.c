#include <string.h>

#include "parse_x86.h"

int parse_asm_x86(const char * code) {
        if (strcmp(code, "nop"     ) == 0) return 1;
        if (strcmp(code, "beqz"    ) == 0) return 2;
        if (strcmp(code, "beqone"  ) == 0) return 3;
        if (strcmp(code, "bgtz"    ) == 0) return 4;
        if (strcmp(code, "bgtone"  ) == 0) return 5;
        if (strcmp(code, "bifPeqNP") == 0) return 6;

        return 0;
}
