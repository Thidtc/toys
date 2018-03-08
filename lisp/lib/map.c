#include "map.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>

map *map_new() {
    register map *mp;
    mp = (map *)malloc(sizeof(map));
    if (mp == NULL) {
        return NULL;
    }
    mp->nused = 0;
    mp->nfill = 0;
    memset(mp->smalltable, 0, sizeof(map_entry) * MAP_MINSIZE);
    mp->table = mp->smalltable;
    mp->mask = MAP_MINSIZE - 1;

    /* Initialize functions */
    mp->hash = NULL;
    mp->key_cmp = NULL;
    mp->value_cmp = NULL;
    mp->key_copy = NULL;
    mp->value_copy = NULL;
    mp->key_free = NULL;
    mp->value_free = NULL;

    return mp;
}

void map_free(map *mp) {
    map_clear(mp);
    free(mp);
}

void map_clear(map *mp) {
    register size_t i= mp->nfill;
    register map_entry *ent = mp->table;
    assert(mp->table != NULL);

    /* Free the key and value in table */
    for (; i > 0; ent++) {
        if (ent->value != NULL) {
            --i;
            mp->key_free(ent->key);
            mp->value_free(ent->value);
        } else if (ent->key != NULL) {
            --i;
            assert(ent->key == dummy);
        }
    }

    /* Free the table */
    if (mp->table != mp->smalltable) {
        free(mp->table);
    }
    mp->table = mp->smalltable;
}

map *map_copy(map *mp) {
    register map *newmp = (map *)malloc(sizeof(map));
    register map_entry *toent, *froment;
    register size_t i;
    if (newmp == NULL) {
        return NULL;
    }
    /* Copy the contents */
    memcpy(newmp, mp, sizeof(map));

    if (mp->table == mp->smalltable) {
        newmp->table = newmp->smalltable;
    } else {
        newmp->table = (map_entry *)malloc(sizeof(map_entry) *
                (newmp->mask + 1));
    }
    memset(newmp->table, 0, sizeof(map_entry) * (newmp->mask + 1));
    /* Copy the map content */
    for (i = 0; i < newmp->mask + 1; ++i) {
        toent = newmp->table + i;
        froment = mp->table + i;
        toent->hash = froment->hash;
        if (froment->value) {
            toent->value = newmp->value_copy(froment->value);
        } else {
            froment->value = NULL;
        }
        if (froment->key && froment->key != dummy) {
            toent->key = newmp->key_copy(froment->key);
        } else {
            toent->key = froment->key;
        }
    }
    return newmp;
}

void *map_find(map *mp, void *key) {
    map_entry *ent = map_find_with_hash(mp, key, mp->hash(key));
    if (ent->value) {
        return ent->value;
    }
    return NULL;
}

static map_entry *map_find_with_hash(map *mp, void *key, register long hash) {
    register size_t mask = mp->mask;
    register map_entry *ent;
    register map_entry *freeslot;
    register size_t perturb;
    register size_t i;

    i = (size_t)hash & mask;
    ent = mp->table + i;
    if (ent->key == NULL || mp->key_cmp(ent->key, key) == 0) {
        return ent;
    }

    if (ent->key == dummy) {
        freeslot = ent;
    } else {
        freeslot = NULL;
    }
    
    for (perturb = hash; ; perturb >>= PERTURB_SHIFT) {
        /* Probe */
        i = (i << 2) + i + perturb + 1;
        ent = mp->table + (i & mask);
        if (ent->key == NULL) {
            return freeslot == NULL ? ent : freeslot;
        }
        if (ent->hash == hash && mp->key_cmp(ent->key, key) == 0) {
            return ent;
        }
        if (ent->key == dummy && freeslot == NULL) {
            freeslot = ent;
        }
    }
    /* Not reached */
    assert(0);
    return NULL;
}

/*
 * insert key and value into the map
 */
int map_insert(map *mp, void *key, void *value) {
    return do_map_insert(mp, key, value);
}

/*
 * Delete the key in the map, and return the value
 */
void *map_delete(map *mp, void *key) {
    return do_map_delete(mp, key);
}

/*
 * Iterate over the map. Use like so
 *  size_t i;
 *  key_t *key;
 *  value_t *value;
 *  i = 0;
 *  while (map_next(the_map, &i, &key, &value)) {
 *      Refer to the key and value
 *  }
 */
int map_next(map *mp, size_t *ppos, void **pkey, void **pvalue) {
    register size_t i;
    register size_t mask;
    register map_entry *ent;

    i = *ppos;
    ent = mp->table;
    mask = mp->mask;
    /* Skip the unsed or dummy slot */
    while (i <= mask && (ent[i].value == NULL || ent[i].key == dummy))
        i++;
    *ppos = i+1;
    if (i > mask) {
        return 0;
    }
    if (pkey) {
        *pkey = ent[i].key;
    }
    if (pvalue) {
        *pvalue = ent[i].value;
    }
    return 1;
}

static void *do_map_delete(map *mp, void *key) {
    register map_entry *ent;
    void *old_value;

    ent = map_find_with_hash(mp, key, mp->hash(key));
    if (ent == NULL) {
        /* No such key */
        return NULL;
    }
    if (ent->value == NULL) {
        return NULL;
    }
    mp->key_free(ent->key);
    ent->key = dummy;
    old_value = ent->value;
    ent->value = NULL;
    mp->nused--;

    return old_value;
}

static int do_map_insert(map *mp, void *key, void *value) {
    register size_t nused = mp->nused;
    assert(mp->nfill <= mp->mask);
    map_set(mp, mp->key_copy(key), mp->value_copy(value),
            mp->hash(key));
    /* If Nothing is inserted to map or fill < 2/3 size, Nothing happened */
    if (!(mp->nused > nused && mp->nfill * 3 >= (mp->mask + 1) * 2))
        return 0;
    /* If the map is small, quadrupling the size, else double it */
    return map_resize(mp, (mp->nused > 50000 ? 2 : 4) * mp->nused);
}

static int map_set_by_entry(map *mp, map_entry *ent,
        void *key, void *value, long hash) {
    ent->hash = hash;
    if (ent->value != NULL) {
        /* There is an entry in map that has the same key */
        mp->value_free(ent->value);
        ent->value = value;
        mp->key_free(key);
    } else {
        if (ent->key == NULL) {
            mp->nfill++;
        } else {
            assert(ent->key == dummy);
        }
        ent->key = key;
        ent->value = value;
        ent->hash = hash;
        mp->nused++;
    }
    return 0;
}

static int map_set(map *mp, void *key, void *value, long hash) {
    register map_entry *ent;
    ent = map_find_with_hash(mp, key, hash);
    if (ent == NULL) {
        return -1;
    }
    return map_set_by_entry(mp, ent, key, value, hash);
}

static int map_resize(map *mp, size_t minsize) {
    size_t newsize;
    size_t i;
    map_entry *oldtable, *newtable;
    map_entry *ent;
    int is_oldtable_malloced;
    static map_entry small_copy[MAP_MINSIZE];

    assert(newsize >= 0);

    /* Find the smallest table size > minsize */
    for (newsize = MAP_MINSIZE; newsize <= minsize && newsize > 0;
            newsize <<= 1)
        ;
    
    /* Malloc space for new table */
    oldtable = mp->table;
    assert(oldtable != NULL);
    is_oldtable_malloced = oldtable != mp->smalltable;
    if (newsize == MAP_MINSIZE) {
        newtable = mp->smalltable;
        if (newtable == oldtable) {
            if (mp->nfill == mp->nused) {
                /* No dummies, so do nothing */
                return 0;
            }
            /* Rebuild the table to purge old dummy entries,
             * copy the contents to a temporary table*/
            assert(mp->nfill > mp->nused);
            memcpy(small_copy, oldtable, sizeof(small_copy));
            oldtable = small_copy;
        }
    } else {
        newtable = (map_entry *)malloc(sizeof(map_entry) * newsize);
        if (newtable == NULL) {
            return -1;
        }
    }

    assert(newtable != oldtable);
    mp->table = newtable;
    mp->mask = newsize - 1;
    memset(newtable, 0, sizeof(map_entry) * newsize);
    mp->nused = 0;
    i = mp->nfill;
    mp->nfill = 0;
    
    /* Copy the data over */
    for (ent = oldtable; i > 0; ent++) {
        if (ent->value != NULL) {
            /* Active entry */
            --i;
            map_set(mp, ent->key, ent->value, ent->hash);
        } else if (ent->key != NULL) {
            --i;
            assert(ent->key == dummy);
        }
    }
    if (is_oldtable_malloced) {
        free(oldtable);
    }
    return 0;
}
