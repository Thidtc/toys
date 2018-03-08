/*
 * Open address implementation of map, referencing the implementation
 * of map in python 2.7
 */
#ifndef MAP_H_
#define MAP_H_

#include <stdlib.h>

#define MAP_MINSIZE 8
#define PERTURB_SHIFT 5

typedef struct {
    /* Store hash value of key */
    long hash;
    void *key;
    void *value;
} map_entry;

/* Dummy value */
static char *dummy = "< dummy key >";

typedef struct map map;

typedef long (*hash_func)(void *key);
typedef int (*cmp_func)(void *lhs, void *rhs);
typedef void *(*copy_func)(void *val);
typedef void (*free_func)(void *val);
typedef void (*print_func)(void *val);

/*
 * There are three kinds of slots in the table:
 * # Unused. key == NULL && value == NULL
 * # Active. key != NULL && value != NULL
 * # Dummy. key == dummy && value == NULL
 */

struct map {
    /* # Active entry + Dummy entry */
    size_t nfill;
    /* # Active entry */
    size_t nused;
    /* Total entry - 1 */
    size_t mask;
    map_entry *table;
    map_entry smalltable[MAP_MINSIZE];

    /* Functions */
    hash_func hash;
    cmp_func key_cmp;
    cmp_func value_cmp;
    copy_func key_copy;
    copy_func value_copy;
    free_func key_free;
    free_func value_free;
};

map *map_new();
void map_free(map *mp);
void map_clear(map *mp);
map *map_copy(map *mp);
void *map_find(map *mp, void *key);
int map_insert(map *mp, void *key, void *value);
void *map_delete(map *mp, void *key);
int map_next(map *mp, size_t *ppos, void **pkey, void **pvalue);

static void *do_map_delete(map *mp, void *key);
static int do_map_insert(map *mp, void *key, void *value);
static map_entry *map_find_with_hash(map *mp, void *key, long hash);
static int map_set_by_entry(map *mp, map_entry *ent,
        void *key, void *value, long hash);
static int map_set(map *mp, void *key, void *value, long hash);
static int map_resize(map *mp, size_t minsize);
#endif /* MAP_H_ */
