/*
 * This is a sample code for generating shared object file
 * which the lisp program can dynamic load
 * All the function needs to deal with memory occupied by 
 * second parameter
 */

/* Inlucde the lispy head file */
#include "lispy.h"

/* Function needs to be of type builtin_func */
lval *func_hello(lenv *e, lval *v) {
    lval_free(v);
    return lval_str("hello");
}

/* Export lists */
func_entry exports[] = {
    {"test", func_hello},
    {NULL, NULL},
};
