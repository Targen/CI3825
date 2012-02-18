#include "foo.h"
#include "foo1.h"
#include "foo2.h"

int foo(t_foo x) {
        return foo1(x) + foo2(x);
}
