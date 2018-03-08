#include "alloc.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
static size_t test_fail = 0;
static size_t test_pass = 0;

#define EXPECT_BASE(condition, format, actual, expectation)\
    do {\
        if (condition) {\
            test_pass++;\
        } else {\
            test_fail++;\
            fprintf(stderr, "%s:%d :expected value " format ", actual value " format "\n",\
                    __FILE__, __LINE__,\
                    expectation, actual);\
        }\
    } while (0)

#define EXPECT_INT(a, e)\
    EXPECT_BASE(a == e, "%d", a, e)
#define EXPECT_PTR(a, e)\
    EXPECT_BASE(a == e, "%p", a, e)
#define EXPECT_STR(a, e)\
    EXPECT_BASE(strcmp((char *)a, (char *)e) == 0, "%s", (char *)a, (char *)e)

void test_norm_allocate() {
    void *addr;
    size_t i;
    size_t j;
    norm_open(8);
    for (i = 8; i < MAX_BYTES; ++i) {
        for (j = 0; j < 12; ++j) {
            addr = norm_malloc(i);
            norm_free(addr, i);
        }
    }
    for (i = 0; i < 4; i++) {
        addr = norm_malloc(8);
    }
    norm_close(0);
}

int main() {
    test_norm_allocate();

    /* Test results */
    fprintf(stdout, "TOTAL: %zu\n", test_fail + test_pass);
    fprintf(stdout, "FAIL: %zu\n", test_fail);
    fprintf(stdout, "PASS: %zu\n", test_pass);
    return 0;
}
