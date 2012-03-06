#include <unistd.h>
#include <stdio.h>

#include "args.h"

void parse_opts(int argc, char *argv[]) {
        int opt;
        while ((opt = getopt(argc, argv, "nt:")) != -1) {
                switch (opt) {
                        case 'n': puts("Opción n!"); break;
                        case 't': puts("Opción t!"); break;
                }
        }
}
