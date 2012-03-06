#ifndef MULTIDOC_MARMOTA_H
#define MULTIDOC_MARMOTA_H

#include "archs.h"

#define DEFAULT_ARCH ARCH_MIPS

typedef enum arch marmota_t;

void marmota(marmota_t arch);

void marmota_def();

#endif
