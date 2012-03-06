#include <string.h>

#include "multidoc.h"
#include "xml.h"

int is_asm(const char * filename) {
        int l = strlen(filename);
        return l > 2 && strcmp(filename + l - 2, ".s") == 0;
}

struct XMLDoc * multidoc(const char * filename) {
        struct XMLDoc * r = newXMLDoc(filename);
        r->is_asm = is_asm(filename);
        return r;
}
