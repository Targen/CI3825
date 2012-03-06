#include <stdio.h>
#include <stdlib.h>

#include "foo/foo.h"
#include "bar/bar.h"
#include "f.h"

int main() {
        printf("foo == %d\n", foo(2));
        printf("bar == %d\n", bar(2));
        printf("f   == %d\n", f  (2));

        exit(EXIT_SUCCESS);
}
