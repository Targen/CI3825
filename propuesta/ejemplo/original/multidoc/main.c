#include <stdio.h>

#include "main.h"
#include "args.h"

void foo() {
        puts("foo!");
}

int main(int argc, char *argv[]) {
        puts("main!");
        foo();
        parse_opts(argc, argv);
        return 0;
}
