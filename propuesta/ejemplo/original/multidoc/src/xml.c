#include <stdlib.h>

#include "xml.h"

struct XMLDoc * newXMLDoc(const char * name) {
        struct XMLDoc * x = (struct XMLDoc *)malloc(sizeof(struct XMLDoc));
        if (x == NULL) return NULL;

        x->name = name;
        x->children = NULL;

        return x;
}
