#include "alloc.h"

#include <assert.h>
#include <string.h>

static unsigned char *obj_block_new(size_t n) {
    register unsigned char *p;
    register obj_block *res = (obj_block *)malloc(sizeof(obj_block));
    if (res == NULL) {
        return NULL;
    }
    res->next = objects[n];
    objects[n] = res;
    res->data = (unsigned char *)malloc(n * OBJ_NUM); 
    if (res->data == NULL) {
        free(res);
        return NULL;
    }
    memset(res->data, 0, n * OBJ_NUM);
    for (p = res->data; p < res->data + n * (OBJ_NUM - 1); p += OBJ_NUM) {
        *(long *)p = (long)(p + n);
    }
    //free_lists[n] = res->data;
    return res->data;
}

static void *norm_allocate(size_t n) {
    assert(n <= MAX_BYTES);
    assert(n >= sizeof(int *));
    void *res;
    if (free_lists[n] == NULL) {
        free_lists[n] = obj_block_new(n);
    }
    res = free_lists[n];
    free_lists[n] = (unsigned char *)(*((long *)free_lists[n]));
    return res;
}

static void norm_deallocate(void *addr, size_t n) {
    assert(n <= MAX_BYTES);
    assert(n >= sizeof(long *));
    if (addr == NULL) {
        return;
    }
    *((long *)addr) = (long)free_lists[n];
    free_lists[n] = (unsigned char *)addr;
}

static void norm_deinitialize(size_t n) {
    register obj_block *current;
    register obj_block *next;
    current = objects[n];
    while (current != NULL) {
        next = current->next;
        free(current->data);
        free(current);
        current = next;
    }
}

/*
 * Allocate sapce for the n-size item
 * Params:
 *      n    : the item size
 */
void *norm_malloc(size_t n) {
    return norm_allocate(n);
}

/*
 * Free space for the n-size item address
 * Params:
 *      addr : the address to be freed
 *      n    : the corresponding item size
 */
void norm_free(void *addr, size_t n) {
    norm_deallocate(addr, n);
}

/*
 * Warm up the allocator of the specified item-size, that is, pre-allocate slots
 * Params:
 *      n : the item-size of allocator to warm up
 */
void norm_open(size_t n) {
    unsigned char *free_list = free_lists[n];
    if (free_list == NULL) {
        free_list = obj_block_new(n);
    }
}

/*
 * Free the spaces for the allocator of the specified item-size
 * Params:
 *      n : if n is 0, free allocator for all item-size allocator, otherwise,
 *      free the allocator of the specified item-size
 */
void norm_close(size_t n) {
    size_t i;
    assert(n < MAX_BYTES);
    if (n == 0) {
        for (i = 0; i < MAX_BYTES; ++i) {
            norm_deinitialize(i);
        }
    } else {
        norm_deinitialize(n);
    }
}
