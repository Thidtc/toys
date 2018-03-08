#ifndef LISP_H_
#define LISP_H_

#include "map.h"
#include "hash_func.h"

#include <stdlib.h>
#include <stdio.h>

#define MAX_SYM_LEN 256ul
#define DEFAULT_EXPR_SIZE 4ul
#define DEFAULT_ENV_SIZE 10ul
#define STREAM_BUF 1024ul
#define ERRSTR_SIZE 128ul

#define LISP_VERSION "Lisp version 0.1"

enum {
    /* Basic type */
    LVAL_ERR, LVAL_NUM, LVAL_SYM,
    LVAL_STR, LVAL_FUNC, LVAL_SEXPR, LVAL_QEXPR, LVAL_LISP,
    LVAL_STATUS,
    /* Additional type */
    LVAL_FILE,
};
enum {
    ERR_INVALID_SYNTAX,
    ERR_DIVIE_ZERO,
    ERR_UNBOUND_SYMBOL,
    ERR_OTHERS,
};
enum {
    STATUS_OK,
    STATUS_EXIT,
};

typedef struct lval lval;
typedef struct lenv lenv;
typedef struct map map;
typedef lval *(*builtin)(lenv *, lval *);

/* C function entry */
typedef struct func_entry func_entry;

struct func_entry {
    const char *name;
    builtin func;
};

struct lenv {
    lenv *parent;
    map *mp;
};

struct lval{
    int type;
    union {
        long num;
        struct {
            int i;
            char *s;
        } err;
        char *sym;
        char *str;
        struct {
            lval **cell;
            size_t size;
            size_t top;
        } expr;
        struct {
            /* Builtin function if f == NULL or lambda */
            builtin f;

            lenv *env;
            lval *formals;
            lval *body;
        } func;
        int status;
        /* Additional type */
        struct {
            FILE *fp;
        } addi;
    } u;
};

inline static char *string_copy(const char *s);

lenv *lenv_new();
lenv *lenv_copy(const lenv *e);
void lenv_free(lenv *e);
static lval *lenv_get(const lenv *e, const char *key);
lval *lenv_get_sym(const lenv *e, const lval *v);
static void lenv_put(lenv *e, char *key, lval *v);
void lenv_put_sym(lenv *e, lval *key, lval *v);
void lenv_put_sym_global(lenv *e, lval *key, lval *v);
void lenv_add_builtin(lenv *e, char *name, builtin func);
void lenv_add_builtins(lenv *e);
void lenv_print(const lenv *e, int bParent);

char *strerr(int e);

const char *lval_typename(int type);
const char  *lval_funcname(lenv *e, builtin func);
/*
 * By default all create function uses deep copy
 */
lval *lval_err(int e);
lval *lval_err_format(char *fmt, ...);
lval *lval_num(long n);
lval *lval_str(const char *s);
lval *lval_str_shadow(char *s);
lval *lval_str_with_len(const char *s, size_t len);
lval *lval_sym(const char *s);
lval *lval_expr(int type);
lval *lval_func(builtin func);
lval *lval_lambda(lval *formals, lval *body);
lval *lval_status(int status);
lval *lval_file(FILE *f);
lval *lval_copy(lval *v);
int lval_equal(lval *lhs, lval *rhs);
void lval_free(lval *v);

lval *lval_eval_params(lenv *e, lval *params);

static void lval_print_expr(lval *v, int bline);
void lval_print(lval *v, int bline);

lval *lval_push(lval *v, lval *t);
lval *lval_merge(lval *v, lval *t);
lval *lval_at(lval *v, size_t i);
lval *lval_take_at(lval *v, size_t i);
lval *lval_remove(lval *v, size_t i);

lval *lval_eval(lenv *e, lval *v);
static lval *lval_eval_sexpr(lenv *e, lval *v);
lval *lval_call(lenv *e, lval *func, lval *params);

void repl();

lval *parse_str(const char *s);
lval *parse_file(const char *fname);
int yyerror(const char *s);

#endif /* LISP_H_ */
