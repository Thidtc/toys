#include "map.h"
#include "hash_func.h"

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
            fprintf(stderr, "%s:%d :expecation value " format ", actual value " format "\n",\
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
#define EXPECT_MAP_USED(res, e)\
    EXPECT_BASE(res->nused == e, "%zu", res->nused, (size_t)e)
#define EXPECT_MAP_FILL(res, e)\
    EXPECT_BASE(res->nfill == e, "%zu", res->nfill, (size_t)e)


/* Functions */
long string_hash(void *key) {
    return (long)((char *)key)[0];
}
int string_cmp(void *lhs, void *rhs) {
    return strcmp((char *)lhs, (char *)rhs);
}
void *string_copy(void *val) {
    void *res = malloc(strlen((char *)val) + 1);
    memcpy(res, val, strlen((char *)val));
    ((char *)res)[strlen((char *)val)] = 0;
    return res;
}
void string_free(void *val) {
    free(val);
}

void test_string_key_string_value() {
    int res;
    void *value;
    /* Initialize map with string key and string value */
    map *mp = map_new();
    mp->hash = (hash_func)string_hash1;
    mp->key_cmp = string_cmp;
    mp->value_cmp = string_cmp;
    mp->key_copy = string_copy;
    mp->value_copy = string_copy;
    mp->key_free = string_free;
    mp->value_free = string_free;

    res = map_insert(mp, "a", "a");
    EXPECT_INT(res, 0);
    EXPECT_MAP_USED(mp, 1);
    value = map_find(mp, "a7");
    EXPECT_PTR(value, NULL);
    res = map_insert(mp, "b", "b");
    EXPECT_INT(res, 0);
    EXPECT_MAP_USED(mp, 2);
    res = map_insert(mp, "a1", "a1");
    EXPECT_INT(res, 0);
    EXPECT_MAP_USED(mp, 3);
    value = map_find(mp, "a1");
    EXPECT_STR(value, "a1");
    res = map_insert(mp, "a2", "a2");
    EXPECT_INT(res, 0);
    EXPECT_MAP_USED(mp, 4);
    res = map_insert(mp, "a3", "a3");
    EXPECT_INT(res, 0);
    EXPECT_MAP_USED(mp, 5);
    res = map_insert(mp, "a4", "a4");
    EXPECT_INT(res, 0);
    EXPECT_MAP_USED(mp, 6);
    res = map_insert(mp, "b1", "b1");
    EXPECT_INT(res, 0);
    EXPECT_MAP_USED(mp, 7);
    res = map_insert(mp, "b1", "b1");
    EXPECT_INT(res, 0);
    EXPECT_MAP_USED(mp, 7);
    value = map_find(mp, "b1");
    EXPECT_STR(value, "b1");
    value = map_find(mp, "a2");
    EXPECT_STR(value, "a2");
    value = map_find(mp, "a4");
    EXPECT_STR(value, "a4");
    value = map_find(mp, "a");
    EXPECT_STR(value, "a");
    value = map_find(mp, "b");
    EXPECT_STR(value, "b");
    value = map_delete(mp, "a1");
    EXPECT_STR(value, "a1");
    EXPECT_MAP_USED(mp, 6);
    EXPECT_MAP_FILL(mp, 7);
    mp->value_free(value);
    value = map_find(mp, "a1");
    EXPECT_PTR(value, NULL);

    map *newmp = map_copy(mp);
    value = map_find(newmp, "b");
    EXPECT_STR(value, "b");
    value = map_delete(newmp, "b");
    EXPECT_STR(value, "b");
    newmp->value_free(value);
    value = map_find(newmp, "b");
    EXPECT_PTR(value, NULL);
    value = map_find(mp, "b");
    EXPECT_STR(value, "b");

    /* Iterate over the map */
    /*
    size_t i = 0;
    char *k;
    char *v;
    while (map_next(mp, &i, (void **)&k, (void **)&v)) {
        fprintf(stderr, "%s:%s\n", k, v);
    }
    */

    map_free(newmp);
    /* Free map */
    map_free(mp);
}

int main() {
    test_string_key_string_value();

    /* Test results */
    fprintf(stdout, "TOTAL: %zu\n", test_fail + test_pass);
    fprintf(stdout, "FAIL: %zu\n", test_fail);
    fprintf(stdout, "PASS: %zu\n", test_pass);

    return 0;
}
