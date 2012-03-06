#include <stdio.h>

#include "marmota.h"

void marmota(marmota_t marmota) {
        switch (marmota) {
                case ARCH_MIPS: puts("MIPS!"   ); break;
                case ARCH_X86 : puts("x86!"    ); break;
                default       : puts("Marmota!"); break;
        }
}

void marmota_def() {
        marmota(DEFAULT_ARCH);
}
