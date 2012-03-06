#include <string.h>

#include "parse_java.h"

int parse_java(const char * code) {
        if (strcmp(code, "public") == 0) return 1;
        if (strcmp(code, "static") == 0) return 2;
        if (strcmp(code, "void"  ) == 0) return 3;

        return 0;
}
