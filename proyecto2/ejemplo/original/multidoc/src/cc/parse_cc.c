#include <string.h>

#include "parse_cc.h"

int parse_cc(const char * code) {
        if (strcmp(code, "if"      ) == 0) return 1;
        if (strcmp(code, "else"    ) == 0) return 2;
        if (strcmp(code, "operator") == 0) return 3;

        return 0;
}
