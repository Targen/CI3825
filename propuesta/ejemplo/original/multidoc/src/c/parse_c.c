#include <string.h>

#include "parse_c.h"

int parse_c(const char * code) {
        if (strcmp(code, "if"  ) == 0) return 1;
        if (strcmp(code, "else") == 0) return 2;

        return 0;
}
