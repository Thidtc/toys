#ifndef ALLOC_H_
#define ALLOC_H_

#include <stdlib.h>

#define MAX_BYTES 128

/* Normal allocator */

#define OBJ_NUM 1

typedef struct obj_block obj_block;
struct obj_block {
    struct obj_block *next;
    unsigned char *data;
};

obj_block *objects[MAX_BYTES];
unsigned char *free_lists[MAX_BYTES];

static unsigned char *obj_block_new(size_t n);
static void *norm_allocate(size_t n);
static void norm_deallocate(void *addr, size_t n);
static void norm_deinitialize();

void *norm_malloc(size_t n);
void norm_free(void *addr, size_t n);
void norm_open(size_t n);
void norm_close(size_t n);

#endif /* ALLOC_H_ */
