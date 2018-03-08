#ifndef BUILTIN_FUNC_H__
#define BUILTIN_FUNC_H__
/* * Builtin function
 * Note : all Builtin function is responsible for freeing memory of
 * parameters
 */

#include "lispy.h"

#include <assert.h>

/* Return error if condition is true */
#define ASSERT(cond, action, err_msg, ...)\
    do { if (!(cond)) {action return lval_err_format(err_msg, ##__VA_ARGS__);}\
    } while (0)

/* Util functions */
/* Binary operation */
static lval *utils_bin(lenv *e, lval *params, char *op);

/* Builtin function */
/* List functions */
static lval *builtin_list(lenv *e, lval *params);
static lval *builtin_len(lenv *e, lval *params);
static lval *builtin_car(lenv *e, lval *params);
static lval *builtin_cdar(lenv *e, lval *params);
static lval *builtin_cddar(lenv *e, lval *params);
static lval *builtin_cdddar(lenv *e, lval *params);
static lval *builtin_cdr(lenv *e, lval *params);
static lval *builtin_append(lenv *e, lval *params);

/* Mathematical functions */
static lval *builtin_add(lenv *e, lval *params);
static lval *builtin_sub(lenv *e, lval *params);
static lval *builtin_mul(lenv *e, lval *params);
static lval *builtin_div(lenv *e, lval *params);

/* Logical operation functions */
static lval *builtin_and(lenv *e, lval *params);
static lval *builtin_or(lenv *e, lval *params);
static lval *builtin_not(lenv *e, lval *params);

/* Cmparison functions */
static lval *builtin_gt(lenv *e, lval *params);
static lval *builtin_lt(lenv *e, lval *params);
static lval *builtin_ge(lenv *e, lval *params);
static lval *builtin_le(lenv *e, lval *params);
static lval *builtin_eq(lenv *e, lval *params);
static lval *builtin_noteq(lenv *e, lval *params);

/* Conditions */
static lval *builtin_if(lenv *e, lval *params);
static lval *builtin_cond(lenv *, lval *params);

static lval *builtin_def(lenv *e, lval *params);
static lval *builtin_exit(lenv *e, lval *params);
static lval *builtin_lambda(lenv *e, lval *params);
static lval *builtin_load(lenv *e, lval *params);
static lval *builtin_error(lenv *e, lval *params);

lval *builtin_eval(lenv *e, lval *params);

static lval *builtin_do(lenv *e, lval *params);

static lval *builtin_let(lenv *e, lval *params);

/* IO functions */
static lval *builtin_read_str(lenv *e, lval *params);
static lval *builtin_print(lenv *e, lval *params);

/* File functions */
static lval *builtin_open(lenv *e, lval *params);
static lval *builtin_read(lenv *e, lval *params);
static lval *builtin_write(lenv *e, lval *params);
static lval *builtin_close(lenv *e, lval *params);

#endif /* BUILTIN_FUNC_H__ */
