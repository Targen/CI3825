#ifndef MULTIDOC_XML_H
#define MULTIDOC_XML_H

#include "asm/archs.h"

typedef enum arch xml_arch_t;

struct XMLDoc {
        const char * name;
        int is_asm;
        xml_arch_t arch;
        struct XMLDoc * children;
};

struct XMLDoc * newXMLDoc(const char * name);

#endif
