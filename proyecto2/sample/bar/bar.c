#include "bar.h"
#include "bar1.h"
#include "bar2.h"

int bar(int x) {
        return bar1(x) + bar2(x);
}
