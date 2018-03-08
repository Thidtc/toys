#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdarg.h>

#include "lispy.h"
#include "builtin_func.h"
#include "lisp_malloc.h"

/* Copy a string, including the ending '\0' */
inline static char *string_copy(const char *s) {
    char *res;
    res = (char *)malloc(sizeof(char) * (strlen(s) + 1));
    memcpy(res, s, sizeof(char) * (strlen(s) + 1));
    return res;
}

lenv *lenv_new() {
    lenv *res = (lenv *)malloc(sizeof(lenv));
    if (res == NULL) {
        return NULL;
    }
    res->parent = NULL;
    res->mp = map_new();
    res->mp->hash = (hash_func)string_hash1;
    res->mp->key_cmp = (cmp_func)strcmp;
    res->mp->value_cmp = (cmp_func)NULL;
    res->mp->key_copy = (copy_func)string_copy;
    res->mp->value_copy = (copy_func)lval_copy;
    res->mp->key_free = (free_func)free;
    res->mp->value_free = (free_func)lval_free;

    return res;
}

lenv *lenv_copy(const lenv *e) {
    size_t i;
    lenv *res = (lenv *)malloc(sizeof(lenv));
    if (res == NULL) {
        return NULL;
    }

    /* Shadow copy for parent env */
    res->parent = e->parent;

    res->mp = map_copy(e->mp);

    return res;
}

void lenv_free(lenv *e) {
    map_free(e->mp);
    free(e);
}

static lval *lenv_get(const lenv *e, const char *key) {
    size_t i;
    lval *res;
    if (e == NULL) {
        return NULL;
    }
    res = (lval *)map_find(e->mp, (void *)key);
    if (res) {
        return lval_copy(res);;
    }
    /* Find in parent environment */
    if (e->parent) {
        return lenv_get(e->parent, key);
    }
    return NULL;
}

lval *lenv_get_sym(const lenv *e, const lval *v) {
    size_t i;
    lval *res;
    if (v == NULL) {
        return lval_err_format("null sym");
    }
    if (v->type != LVAL_SYM) {
        return lval_err_format("invalid sym");
    }
    if ((res = lenv_get(e, (const char *)v->u.sym)) != NULL)
        return res;
    return lval_err_format("Unbounded symbol : %s", v->u.sym);
}

static void lenv_put(lenv *e, char *key, lval *v) {
    map_insert(e->mp, (void *)key, (void *)v);
}

void lenv_put_sym(lenv *e, lval *key, lval *v) {
    lenv_put(e, key->u.sym, v);
}

/* Put symbol in topmost environment, that is, global environtment */
void lenv_put_sym_global(lenv *e, lval *key, lval *v) {
    if (e == NULL)
        return;
    while (e->parent)
        e = e->parent;
    lenv_put_sym(e, key, v);
}

void lenv_add_builtin(lenv *e, char *name, builtin func) {
    lval *v = lval_func(func);
    lenv_put(e, name, v);
    lval_free(v);
}

/* Evaluate parameters */
lval *lval_eval_params(lenv *e, lval *params) {
    size_t i;
    for (i = 0; i < params->u.expr.top; ++i) {
        params->u.expr.cell[i] = lval_eval(e, params->u.expr.cell[i]);
    }

    for (i = 0; i < params->u.expr.top; ++i)
        if (params->u.expr.cell[i]->type == LVAL_ERR)
            return lval_take_at(params, i);
    params->type == LVAL_SEXPR;
    return NULL;
}


void lenv_print(const lenv *e, int bParent) {
    size_t i;
    char *key;
    lval *value;

    i = 0;
    while (map_next(e->mp, &i, (void **)&key, (void **)&value)) {
        fprintf(stdout, "%s: ", key);
        lval_print(value, 0);
        fprintf(stdout, "\n");
    }
}

char *strerr(int e) {
    switch (e) {
        case ERR_INVALID_SYNTAX:
            return "Invalide syntax";
        case ERR_DIVIE_ZERO : 
            return "Divide by zero";
        case ERR_UNBOUND_SYMBOL :
            return "Unbound symbol";
        default :
            return "Other error";
    }
}

const char *lval_typename(int type) {
    switch (type) {
        case LVAL_ERR : return "Error";
        case LVAL_NUM : return "Number";
        case LVAL_SYM : return "Symbol";
        case LVAL_STR : return "String";
        case LVAL_FUNC : return "Function";
        case LVAL_SEXPR : return "S-expression";
        case LVAL_QEXPR : return "Q-expression";
        case LVAL_STATUS : return "Status";
        default : return "Unknown";
    }
}

const char *lval_funcname(lenv *e, builtin func) {
    size_t i;
    /*
    for (i = 0; i < e->top; ++i) {
        if (e->tbl[i].val->type == LVAL_FUNC &&
                e->tbl[i].val->u.func.f == func) {
            return e->tbl[i].sym;
        }
    }
    */
    return "Unknown function";
}

/* Deprecated, use lval_err_format instead */
lval *lval_err(int e) {
    lval *v = (lval *)malloc(sizeof(lval));
    if (v == NULL) {
        return NULL;
    }
    v->u.err.i = e;
    v->u.err.s = NULL;
    v->type = LVAL_ERR;
    return v;
}

lval *lval_err_format(char *fmt, ...) {
    lval *v = (lval *)malloc(sizeof(lval));

    va_list va;
    va_start(va, fmt);

    v->u.err.s = (char *)malloc(sizeof(char) * ERRSTR_SIZE);
    v->u.err.s[ERRSTR_SIZE - 1] = 0;
    vsnprintf(v->u.err.s, ERRSTR_SIZE, fmt, va);
    v->u.err.s = realloc(v->u.err.s, strlen(v->u.err.s) + 1);
    va_end(va);

    v->u.err.i = ERR_OTHERS;
    v->type = LVAL_ERR;

    return v;
}

lval *lval_num(long n) {
    lval *v = (lval *)malloc(sizeof(lval));
    if (v == NULL) {
        return NULL;
    }
    v->u.num = n;
    v->type = LVAL_NUM;
    return v;
}

lval *lval_str(const char *s) {
    if (s == NULL) {
        return NULL;
    }
    lval *v = (lval *)malloc(sizeof(lval));
    if (v == NULL) {
        return NULL;
    }
    v->u.str = (char *)malloc(strlen(s) + 1);
    if (v->u.str == NULL) {
        free(v);
        return NULL;
    }
    memcpy(v->u.str, s, strlen(s) + 1); /* Include ending '\0' */
    v->type = LVAL_STR;
    return v;
}

/* Create string with shadow copy */
lval *lval_str_shadow(char *s) {
    if (s == NULL) {
        return NULL;
    }
    lval *v = (lval *)malloc(sizeof(lval));
    if (v == NULL) {
        return NULL;
    }
    v->u.str = s;
    v->type = LVAL_STR;
    return v;
}

/*
 * Create string with specified length(not including '\o')
 */
lval *lval_str_with_len(const char *s, size_t len) {
    if (s == NULL) {
        return NULL;
    }
    lval *v = (lval *)malloc(sizeof(lval));
    if (v == NULL) {
        return NULL;
    }
    v->u.str = (char *)malloc(len + 1);
    if (v->u.str == NULL) {
        free(v);
        return NULL;
    }
    memcpy(v->u.str, s, len);
    v->u.str[len] = 0;
    v->type = LVAL_STR;
    return v;
}

lval *lval_sym(const char *s) {
    if (s == NULL) {
        return NULL;
    }
    lval *v = (lval *)malloc(sizeof(lval));
    if (v == NULL) {
        return NULL;
    }
    v->u.sym = (char *)malloc(strlen(s) + 1);
    if (v == NULL) {
        lval_free(v);
        return NULL;
    }
    memcpy(v->u.sym, s, strlen(s) + 1);
    v->type = LVAL_SYM;
    return v;
}

lval *lval_expr(int type) {
    lval *v = (lval *)malloc(sizeof(lval));
    if (v == NULL) {
        return NULL;
    }
    v->u.expr.cell = NULL;
    v->u.expr.size = v->u.expr.top = 0;
    v->type = type;
    return v;
}

lval *lval_func(builtin func) {
    lval *v = (lval *)malloc(sizeof(lval));
    v->type = LVAL_FUNC;
    v->u.func.f = func;
    return v;
}

lval *lval_lambda(lval *formals, lval *body) {
    lval *v = (lval *)malloc(sizeof(lval));
    if (v == NULL) {
        return NULL;
    }
    v->u.func.f = NULL;
    v->u.func.env = lenv_new();
    v->u.func.formals = formals;
    v->u.func.body = body;
    v->type = LVAL_FUNC;

    return v;
}

lval *lval_status(int status) {
    lval *v = (lval *)malloc(sizeof(lval));
    if (v == NULL) {
        return NULL;
    }
    v->u.status = status;
    v->type = LVAL_STATUS;

    return v;
}

lval *lval_file(FILE *f) {
    lval *v = (lval *)malloc(sizeof(lval));
    if (v == NULL) {
        return NULL;
    }
    v->u.addi.fp = f;
    v->type = LVAL_FILE;

    return v;
}

/* Deep copy */
lval *lval_copy(lval *v) {
    size_t i;
    lval *res = (lval *)malloc(sizeof(lval));
    if (res == NULL) {
        return NULL;
    }
    res->type = v->type;
    switch(res->type) {
        case LVAL_ERR :
            res->u.err.i = v->u.err.i;
            res->u.err.s = v->u.err.s;
            break;
        case LVAL_NUM :
            res->u.num = v->u.num;
            break;
        case LVAL_SYM :
            res->u.sym = string_copy(v->u.sym);
            break;
        case LVAL_STR :
            res->u.str = string_copy(v->u.str);
            break;
        case LVAL_FUNC :
            if (v->u.func.f) {
                res->u.func.f = v->u.func.f;
            } else {
                res->u.func.f = NULL;
                res->u.func.env = lenv_copy(v->u.func.env);
                res->u.func.formals = lval_copy(v->u.func.formals);
                res->u.func.body = lval_copy(v->u.func.body);
            }
            break;
        case LVAL_SEXPR :
        case LVAL_QEXPR :
            res->u.expr.size = v->u.expr.size;
            res->u.expr.top = v->u.expr.top;
            res->u.expr.cell = (lval **)malloc(sizeof(lval *)
                    * res->u.expr.size);
            for (i = 0; i < res->u.expr.top; ++i) {
                res->u.expr.cell[i] = lval_copy(v->u.expr.cell[i]);
            }
            break;
        case LVAL_STATUS :
            res->u.status = v->u.status; 
            break;
        case LVAL_FILE :
            res->u.addi.fp = v->u.addi.fp;
            break;
        default :
            break;
    }
    return res;
lexit:
    lval_free(res);
    return NULL;
}

int lval_equal(lval *lhs, lval *rhs) {
    if (lhs->type != rhs->type)
        return 0;
    switch (lhs->type) {
        case LVAL_NUM :
            return lhs->u.num == rhs->u.num;
        case LVAL_STR :
            return strcmp(lhs->u.str, rhs->u.str) == 0;
        case LVAL_SYM :
            return strcmp(lhs->u.sym, rhs->u.sym) == 0;
        case LVAL_ERR :
            return (lhs->u.err.i == rhs->u.err.i &&
                    strcmp(lhs->u.err.s, rhs->u.err.s) == 0);
        case LVAL_FUNC :
            if (lhs->u.func.f && rhs->u.func.f) {
                return lhs->u.func.f == rhs->u.func.f;
            } else if (!lhs->u.func.f && !rhs->u.func.f) {
                return (lval_equal(lhs->u.func.formals, rhs->u.func.formals) &&
                    lval_equal(lhs->u.func.body, rhs->u.func.body));
            } else {
                return 0;
            }
        case LVAL_QEXPR :
        case LVAL_SEXPR :
            if (lhs->u.expr.top != rhs->u.expr.top) {
                return 0;
            }
            for (int i = 0; i < lhs->u.expr.top; ++i) {
                if (!lval_equal(lhs->u.expr.cell[i], rhs->u.expr.cell[i])) {
                    return 0;
                }
            }
            return 1;
        default :
            break;
    }
    return 0;
}

void lval_free(lval *v) {
    size_t i;
    switch (v->type) {
        case LVAL_NUM : break;
        case LVAL_STR : free(v->u.str); break;
        case LVAL_SYM : free(v->u.sym); break;
        case LVAL_QEXPR :
        case LVAL_SEXPR : 
            for (i = 0; i < v->u.expr.top; ++i) lval_free(v->u.expr.cell[i]);
            free(v->u.expr.cell);
            break;
        case LVAL_ERR :
            free(v->u.err.s); break;
        case LVAL_FUNC :
            /* Free if is a lambda function */
            if (!v->u.func.f) {
                lenv_free(v->u.func.env);
                lval_free(v->u.func.formals);
                lval_free(v->u.func.body);
            }
            break;
        case LVAL_STATUS :
            break;
        default:
            break;
    }
    free(v);
}

lval *lval_push(lval *v, lval *t) {
    assert(v->type == LVAL_SEXPR);
    if (v->u.expr.size == 0) {
        v->u.expr.size = DEFAULT_EXPR_SIZE;
        v->u.expr.cell = (lval **)realloc(v->u.expr.cell,
                sizeof(lval *) * v->u.expr.size);
    }
    if (v->u.expr.top++ >= v->u.expr.size) {
        v->u.expr.size *= 2;
        v->u.expr.cell = (lval **)realloc(v->u.expr.cell,
                sizeof(lval *) * v->u.expr.size);
    }
    v->u.expr.cell[v->u.expr.top - 1] = t;
    return v;
}

lval *lval_merge(lval *v, lval *t) {
    size_t i;
    if (v->u.expr.size == 0) {
        v->u.expr.size = DEFAULT_EXPR_SIZE;
        v->u.expr.cell = (lval **)realloc(v->u.expr.cell,
                sizeof(lval *) * v->u.expr.size);
    }
    if (v->u.expr.top + t->u.expr.top > v->u.expr.size) {
        v->u.expr.size = t->u.expr.top > v->u.expr.top ?
            t->u.expr.top * 2 : v->u.expr.top * 2;
        v->u.expr.cell = (lval **)realloc(v->u.expr.cell,
                sizeof(lval *) * v->u.expr.size);
    }
    memcpy(v->u.expr.cell + v->u.expr.top, t->u.expr.cell,
            t->u.expr.top * sizeof(lval *));
    v->u.expr.top += t->u.expr.top;
    t->u.expr.top = 0;
    lval_free(t);
    return v;
}

lval *lval_at(lval *v, size_t i) {
    assert(v->type == LVAL_SEXPR);
    if (i >= v->u.expr.top)
        return NULL;
    else
        return v->u.expr.cell[i];
}

/* Take the i-th lval in v and delete v */
lval *lval_take_at(lval *v, size_t i) {
    assert(v->type == LVAL_SEXPR || v->type == LVAL_QEXPR);
    if (i >= v->u.expr.top) {
        /* Free original expression */
        lval_free(v);
        return lval_err_format("Index longer than the length of expression");
    }
    lval *res = lval_remove(v, i);
    lval_free(v);
    return res;
}

lval *lval_remove(lval *v, size_t i) {
    if (v == NULL || i >= v->u.expr.top)
        return NULL;
    lval *res = v->u.expr.cell[i];
    
    /* remove the i-th element from the cell */
    memmove(v->u.expr.cell + i, v->u.expr.cell + i + 1,
            sizeof(lval *) * (v->u.expr.top-- - i - 1));
    return res;
}

/* Print indent */
static size_t indent = 0;

#define INDENT do { indent+=2; } while(0)
#define UNINDENT do { indent -= 2; } while (0)
#define PRINT_INDENT do {\
    for (i = 0; i < indent; ++i) fprintf(stdout, " ");\
    } while (0)

static void lval_print_expr(lval *v, int bline) {
    size_t i;
    if (bline) {
        PRINT_INDENT;
        fprintf(stdout, v->type == LVAL_SEXPR ? "(\n" : "`(\n");
    } else {
        fprintf(stdout, v->type == LVAL_SEXPR ? "(" : "`(");
    }
    for (i = 0; i < v->u.expr.top; ++i) {
        lval_print(v->u.expr.cell[i], bline);
        if (bline) {
            fprintf(stdout, "\n");
        } else {
            fprintf(stdout, " ");
        }
    }
    if (bline) {
        PRINT_INDENT;
        fprintf(stdout, ")\n");
    } else {
        fprintf(stdout, ")");
    }
}

/* 
 * Print the AST of program 
 * if bline is 1, print the indent
 */
void lval_print(lval *v, int bline) {
    size_t i;
    if (v == NULL) {
        fprintf(stdout, "NULL Object");
        return;
    }
    if (bline)
        INDENT;
    switch (v->type) {
        case LVAL_ERR :
            fprintf(stdout, "errno %d: %s\n", v->u.err.i, v->u.err.s);
            break;
        case LVAL_NUM :
            PRINT_INDENT;
            fprintf(stdout, "%ld", v->u.num);
            break;
        case LVAL_STR :
            PRINT_INDENT;
            fprintf(stdout, "\"%s\"", v->u.str);
            break;
        case LVAL_SYM :
            PRINT_INDENT;
            fprintf(stdout, "%s", v->u.sym);
            break;
        case LVAL_SEXPR :
        case LVAL_QEXPR :
            lval_print_expr(v, bline);
            break;
        case LVAL_FUNC :
            if (v->u.func.f) {
                fprintf(stdout, "<builtin>");
            } else {
                fprintf(stdout, "lambda");
                lval_print_expr(v->u.func.formals, bline);
                fprintf(stdout, " ");
                lval_print_expr(v->u.func.body, bline);
            }
            break;
        case LVAL_STATUS :
            PRINT_INDENT;
            fprintf(stdout, "%d", v->u.status);
            break;
        case LVAL_FILE :
            PRINT_INDENT;
            fprintf(stdout, "<FILE>");
            break;
        default :
            fprintf(stdout, "Parsing Exception\n");
            break;
    }
    fflush(stdout);
    if (bline) {
        UNINDENT;
    }
}

lval *lval_eval(lenv *e, lval *v) {
    size_t i;
    switch (v->type) {
        case LVAL_SEXPR :
        {
            return lval_eval_sexpr(e, v);
        }
        case LVAL_SYM :
        {
            lval *res = lenv_get_sym(e, v);
            lval_free(v);
            return res;
        }
        case LVAL_QEXPR :
        case LVAL_ERR :
        case LVAL_NUM :
        case LVAL_STR :
        case LVAL_FUNC :
        case LVAL_STATUS :
        case LVAL_FILE :
        {
            return v;
        }
        default:
            return NULL;
    }
}

static lval *lval_eval_sexpr(lenv *e, lval *v) {
    size_t i;

    /* Empty expression */
    if (v->u.expr.top == 0) {
        return v;
    }
    /*
    for (i = 0; i < v->u.expr.top; ++i) {
        v->u.expr.cell[i] = lval_eval(e, v->u.expr.cell[i]);
    }

    for (i = 0; i < v->u.expr.top; ++i)
        if (v->u.expr.cell[i]->type == LVAL_ERR)
            return lval_take_at(v, i);
    */

    /* Evalute func */
    v->u.expr.cell[0] = lval_eval(e, v->u.expr.cell[0]);
    if (v->u.expr.cell[0]->type == LVAL_ERR) {
        return lval_take_at(v, 0);
    }

    /* 
     * Remove the first element in s-expr as function,
     * the rest of v is parameters
     */
    lval *func = lval_remove(v, 0);
    if (func->type == LVAL_FUNC) {
        lval *res = lval_call(e, func, v);
        lval_free(func);
        return res;
    }

    /* Single expression */
    if (v->u.expr.top == 0) {
        /* Return the first lval */
        lval_free(v);
        return lval_eval(e, func);
    }
}

lval *lval_call(lenv *e, lval *func, lval *params) {
    size_t i, j;
    if (func->u.func.f) {
        return func->u.func.f(e, params);
    }

    /* Evaluate parameters */
    lval *err = lval_eval_params(e, params);
    if (err && err->type == LVAL_ERR) {
        return err;
    }

    lval *formals = func->u.func.formals;

    /* Count of formal parameters & actual parameters */
    size_t nformal = formals->u.expr.top;
    size_t nactual = params->u.expr.top;

    /* Free the params, note that all the data in env is deep copy */
    while (params->u.expr.top) {
        if (formals->u.expr.top == 0) {
            lval_free(params);
            return lval_err_format(
                    "Function '%s' needs %d parameters, %d is given",
                    "lambda", nformal, nactual);
        }

        if (formals->u.expr.top > 0 &&
                strncmp(".", formals->u.expr.cell[0]->u.sym,
                    strlen(formals->u.expr.cell[0]->u.sym)) == 0) {
            /* '.' must be the last but one formal parameter */
            if (formals->u.expr.top != 2) {
                lval_free(params);
                return lval_err_format("Function format error,"
                        " '.' must be the last but one formal parameter");
            }

            /* Delete '.' */
            lval_free(lval_remove(formals, 0));

            /* Get the varargs */
            lval *sym = lval_remove(formals, 0);
            
            lenv_put_sym(func->u.func.env, sym, params);
            lval_free(sym);

            break;
        }

        lval *sym = lval_remove(formals, 0);
        lval *val = lval_remove(params, 0);

        lenv_put_sym(func->u.func.env, sym, val);
        lval_free(sym);
        lval_free(val);
    }

    lval_free(params);

    /* 
     * If the formal parameters are all filled, evaluate and return,
     * otherwise, return a partial filled lambda function object
     */
    if (formals->u.expr.top == 0) {
        func->u.func.env->parent = e;
        return builtin_eval(func->u.func.env, lval_copy(func->u.func.body));
    } else {
        return lval_copy(func);
    }
}
