#include "hash_func.h"

#include <string.h>

long string_hash1(const char *s) {
    static long prefix = 0l;
    static long suffix = 0l;

    register int len;
    register unsigned char *p;
    register long x;

    len = strlen(s);
    if (len == 0) {
        return 0;
    }
    p = (unsigned char *)s;
    x = prefix;
    x ^= *p << 7;
    while (--len >= 0)
        x = (1000003 * x) ^ *p++;
    x ^= strlen(s);
    x ^= suffix;
    if (x == -1)
        x = -2;
    return x;
}
