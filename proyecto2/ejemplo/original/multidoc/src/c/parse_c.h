#ifndef MULTIDOC_PARSE_C_H
#define MULTIDOC_PARSE_C_H

#include "keywords_c.h"

typedef keyword_t token_t[];

int parse_c(const char * code);

#endif
