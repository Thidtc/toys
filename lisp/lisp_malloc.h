#ifndef LISP_MALLOC_H_
#define LISP_MALLOC_H_

#include "alloc.h"
#include "lispy.h"

void *lval_alloc() {
    return norm_malloc(sizeof(lval));
}

void lval_dealloc(void *addr) {
    norm_free(addr, sizeof(lval));
}

void *lenv_alloc() {
    return norm_malloc(sizeof(lenv));
}

void lenv_dealloc(void *addr) {
    norm_free(addr, sizeof(lenv));
}

void *func_entry_alloc() {
    return norm_malloc(sizeof(func_entry));
}

void func_entry_dealloc(void *addr) {
    norm_free(addr, sizeof(func_entry));
}

#endif /* LISP_MALLOC_H_ */
