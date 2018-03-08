#include "lispy.h"
#include "builtin_func.h"
#include "builtin_func_map.h"

#include <string.h>
#include <dlfcn.h>

/* Input buffer */
char buf[STREAM_BUF + 1]; 

void lenv_add_builtins(lenv *e) {
    size_t i;
    for (i = 0; builtin_func_map[i].name != NULL;
            ++i) {
        lenv_add_builtin(e, (char *)builtin_func_map[i].name,
                builtin_func_map[i].func);
    }
}

static lval *utils_bin(lenv *e, lval *params, char *op) {
    lval *err = lval_eval_params(e, params);
    if (err && err->type == LVAL_ERR) {
        return err;
    }
    ASSERT(params->u.expr.top == 2, {lval_free(params);},
            "Function '%s' need 2 parameters", op);
    long res = 0;
    lval *lhs = params->u.expr.cell[0];
    lval *rhs = params->u.expr.cell[1];
    /* Remove parameters */
    params->u.expr.top = 0;
    /* Both sides should have same type */
    ASSERT(rhs->type == lhs->type, {lval_free(params);},
            "Function '%s' need parameters of same type", op);
    /* Number comparison */
    if (lhs->type == LVAL_NUM) {
        if (strncmp(op, ">", strlen(op)) == 0) {
            res = (lhs->u.num > rhs->u.num);
        } else if (strncmp(op, "<", strlen(op)) == 0) {
            res = (lhs->u.num < rhs->u.num);
        } else if (strncmp(op, ">=", strlen(op)) == 0) {
            res = (lhs->u.num >= rhs->u.num);
        } else if (strncmp(op, "<=", strlen(op)) == 0) {
            res = (lhs->u.num <= rhs->u.num);
        } else if (strncmp(op, "==", strlen(op)) == 0) {
            res = (lhs->u.num == rhs->u.num);
        } else if (strncmp(op, "!=", strlen(op)) == 0) {
            res = (lhs->u.num != rhs->u.num);
        } else if (strncmp(op, "&&", strlen(op)) == 0) {
            res = (lhs->u.num && rhs->u.num);
        } else if (strncmp(op, "||", strlen(op)) == 0) {
            res = (lhs->u.num || rhs->u.num);
        }
    } else if (lhs->type == LVAL_STR) {
        if (strncmp(op, ">", strlen(op)) == 0) {
            res = strcmp(lhs->u.str, rhs->u.str) > 0;
        } else if (strncmp(op, "<", strlen(op)) == 0) {
            res = strcmp(rhs->u.str, lhs->u.str) > 0;
        } else if (strncmp(op, ">=", strlen(op)) == 0) {
            res = strcmp(lhs->u.str, rhs->u.str) >= 0;
        } else if (strncmp(op, "<=", strlen(op)) == 0) {
            res = strcmp(rhs->u.str, lhs->u.str) >= 0;
        } else if (strncmp(op, "==", strlen(op)) == 0) {
            res = strcmp(lhs->u.str, rhs->u.str) == 0;
        } else if (strncmp(op, "!=", strlen(op)) == 0) {
            res = strcmp(lhs->u.str, rhs->u.str) != 0;
        }
    } else if (lhs->type == LVAL_SEXPR || lhs->type == LVAL_QEXPR) {
        if (strncmp(op, "==", strlen(op)) == 0) {
            res = lval_equal(lhs, rhs) == 1;
        } else if (strncmp(op, "!=", strlen(op)) == 0) {
            res = lval_equal(lhs, rhs) == 0;
        }
    }

    lval_free(lhs);
    lval_free(rhs);
    lval_free(params);
    return lval_num(res);
}

static lval *builtin_list(lenv *e, lval *params) {
    lval *err = lval_eval_params(e, params);
    if (err && err->type == LVAL_ERR) {
        return err;
    }
    ASSERT(params != NULL, {lval_free(params);}, "");
    params->type = LVAL_QEXPR;
    return params;
}

/*
 * Get the length of the list
 * params[...]: the list items
 * reutrn: the list length
 * note: list function do not evalute item in the list
 */
static lval *builtin_len(lenv *e, lval *params) {
    ASSERT(params != NULL, {lval_free(params);}, "");
    ASSERT(params->type == LVAL_SEXPR, {lval_free(params);},
            "Function 'len' passed incorrect type parameters");
    ASSERT(params->u.expr.top == 1, {lval_free(params);},
            "Function 'len' needs 1 parameter");
    ASSERT(params->u.expr.cell[0]->type == LVAL_QEXPR, {lval_free(params);},
            "Function 'len' needs a list as parameter");
    size_t len = params->u.expr.cell[0]->u.expr.top;
    lval_free(params);
    return lval_num(len);
}

#define BUILTIN_CXX(params, name)\
    do {\
        ASSERT((params) != NULL, {lval_free(params);}, "");\
        ASSERT((params)->u.expr.top == 1, {lval_free(params);},\
                "Function '" name "' needs 1 parameters");\
    } while (0)

static lval *builtin_car(lenv *e, lval *params) {
    lval *err = lval_eval_params(e, params);
    if (err && err->type == LVAL_ERR) {
        return err;
    }
    BUILTIN_CXX(params, "car");
    if (params->type == LVAL_SEXPR) {
        lval *v = lval_take_at(params, 0);
        if (v->type == LVAL_STR) {
            lval *res = lval_str_with_len(v->u.str, 1);
            lval_free(v);
            return res;
        } else {
            return lval_take_at(v, 0);
        }
    }
}

static lval *builtin_cdar(lenv *e, lval *params) {
    lval *err = lval_eval_params(e, params);
    if (err && err->type == LVAL_ERR) {
        return err;
    }
    BUILTIN_CXX(params, "cdar");
    lval *v = lval_take_at(params, 0);
    return lval_take_at(v, 1);
}

static lval *builtin_cddar(lenv *e, lval *params) {
    lval *err = lval_eval_params(e, params);
    if (err && err->type == LVAL_ERR) {
        return err;
    }
    BUILTIN_CXX(params, "cddar");
    lval *v = lval_take_at(params, 0);
    return lval_take_at(v, 2);
}

static lval *builtin_cdddar(lenv *e, lval *params) {
    lval *err = lval_eval_params(e, params);
    if (err && err->type == LVAL_ERR) {
        return err;
    }
    BUILTIN_CXX(params, "cdddar");
    lval *v = lval_take_at(params, 0);
    return lval_take_at(v, 3);
}

static lval *builtin_cdr(lenv *e, lval *params) {
    lval *err = lval_eval_params(e, params);
    if (err && err->type == LVAL_ERR) {
        return err;
    }
    ASSERT(params != NULL, {lval_free(params);}, "");
    ASSERT(params->type == LVAL_SEXPR, {lval_free(params);},
            "Function 'cdr' passed incorrect type parameters");
    ASSERT(params->u.expr.top == 1, {lval_free(params);},
            "Function 'cdr' passed {}");
    if (params->type == LVAL_SEXPR) {
        lval *v = lval_take_at(params, 0);
        if (v->type == LVAL_STR) {
            lval *res = lval_str_with_len(v->u.str + 1,
                strlen(v->u.str) - 1);
            lval_free(v);
            return res;
        } else {
            lval_free(lval_remove(v, 0));
            return v;
        }
    }
}

static lval *builtin_append(lenv *e, lval *params) {
    lval *err = lval_eval_params(e, params);
    if (err && err->type == LVAL_ERR) {
        return err;
    }
    ASSERT(params != NULL, {lval_free(params);}, "");
    ASSERT(params->type == LVAL_SEXPR, {lval_free(params);},
            "Function 'append' passed incorrect type parameters");
    ASSERT(params->u.expr.top == 2, {lval_free(params);},
            "fUNCTION 'append' needs 2 parameters");
    lval *lhs = params->u.expr.cell[0];
    lval *rhs = params->u.expr.cell[1];
    ASSERT(lhs->type == rhs->type, {lval_free(params);},
        "Function 'append' passed incorrect type parameters");
    if (lhs->type == LVAL_QEXPR) {
        params->u.expr.top = 0;
        lval_free(params);
        return lval_merge(lhs, rhs);
    } else if (lhs->type == LVAL_STR) {
        char *s = (char *)malloc(strlen(lhs->u.str) + strlen(rhs->u.str) + 1);
        strcpy(s, lhs->u.str);
        strcat(s, rhs->u.str);
        lval_free(params);
        return lval_str_shadow(s);
    }
}

static lval *builtin_add(lenv *e, lval *params) {
    lval *err = lval_eval_params(e, params);
    if (err && err->type == LVAL_ERR) {
        return err;
    }
    size_t i;
    long res = 0;
    ASSERT(params != NULL, {lval_free(params);}, "");
    ASSERT(params->type == LVAL_SEXPR, {lval_free(params);},
            "Function '+' passed incorrect type parameters");
    ASSERT(params->u.expr.top > 0, {lval_free(params);},
            "Function '+' need more than one parameters");

    for (i = 0; i < params->u.expr.top; ++i) {
        if (params->u.expr.cell[i]->type != LVAL_NUM) {
            lval_free(params);
            return lval_err_format("Funtion 'add' %zu-th parameter wrong", i);
        }
        res += params->u.expr.cell[i]->u.num;
    }

    lval_free(params);
    return lval_num(res);
}

static lval *builtin_sub(lenv *e, lval *params) {
    lval *err = lval_eval_params(e, params);
    if (err && err->type == LVAL_ERR) {
        return err;
    }
    size_t i;
    long res = 0;
    ASSERT(params != NULL, {lval_free(params);}, "");
    ASSERT(params->type == LVAL_SEXPR, {lval_free(params); },
            "Function '-' passed incorrect type parameters");
    ASSERT(params->u.expr.top == 2, {lval_free(params); },
            "Function '-' need two parameters");
    for (i = 0; i < params->u.expr.top; ++i) {
        if (params->u.expr.cell[i]->type != LVAL_NUM) {
            lval_free(params);
            char s[64];
            return lval_err_format("Funtion 'add' %zu-th parameter wrong", i);
        }
    }
    res = params->u.expr.cell[0]->u.num - params->u.expr.cell[1]->u.num;
    lval_free(params);
    return lval_num(res);
}

static lval *builtin_mul(lenv *e, lval *params) {
    lval *err = lval_eval_params(e, params);
    if (err && err->type == LVAL_ERR) {
        return err;
    }
    size_t i;
    long res = 1;
    ASSERT(params != NULL, {lval_free(params);}, "");
    ASSERT(params->type == LVAL_SEXPR, {lval_free(params);},
            "Function '*' passed incorrect type parameters");
    ASSERT(params->u.expr.top > 0, {lval_free(params);},
            "Function '*' need more than one parameters");

    for (i = 0; i < params->u.expr.top; ++i) {
        if (params->u.expr.cell[i]->type != LVAL_NUM) {
            lval_free(params);
            return lval_err_format("Funtion 'add' %zu-th parameter wrong", i);
        }
        res *= params->u.expr.cell[i]->u.num;
    }

    lval_free(params);
    return lval_num(res);
}

static lval *builtin_div(lenv *e, lval *params) {
    lval *err = lval_eval_params(e, params);
    if (err && err->type == LVAL_ERR) {
        return err;
    }
    size_t i;
    long res = 0;
    ASSERT(params != NULL, {lval_free(params);}, "");
    ASSERT(params->type == LVAL_SEXPR, {lval_free(params);},
            "Function '/' passed incorrect type parameters");
    ASSERT(params->u.expr.top == 2, {lval_free(params);},
            "Function '/' needs two parameters");
    for (i = 0; i < params->u.expr.top; ++i) {
        if (params->u.expr.cell[i]->type != LVAL_NUM) {
            lval_free(params);
            return lval_err_format("Funtion 'add' %zu-th parameter wrong", i);
        }
    }
    /* Divide by zero error */

    if (params->u.expr.cell[1]->u.num == 0) {
        lval_free(params);
        return lval_err_format("divide by zero");
    }
    res = params->u.expr.cell[0]->u.num / params->u.expr.cell[1]->u.num;
    lval_free(params);
    return lval_num(res);
}

static lval *builtin_and(lenv *e, lval *params) {
    return utils_bin(e, params, "&&");
}

static lval *builtin_or(lenv *e, lval *params) {
    return utils_bin(e, params, "||");
}

static lval *builtin_not(lenv *e, lval *params) {
    ASSERT(params != NULL, {lval_free(params);}, "");
    ASSERT(params->u.expr.top == 1, {lval_free(params);},
            "Function '/' needs two parameters");

    lval *res = lval_eval(e, lval_at(params, 0));
    ASSERT(res->type == LVAL_NUM, {lval_free(params);},
            "Function '/' needs two parameters");
    
    params->u.expr.top = 0;
    lval_free(params);

    /* Get the NOT value of res */
    res->u.num = !res->u.num;

    return res;
}

static lval *builtin_gt(lenv *e, lval *params) {
    return utils_bin(e, params, ">");
}

static lval *builtin_lt(lenv *e, lval *params) {
    return utils_bin(e, params, "<");
}

static lval *builtin_ge(lenv *e, lval *params) {
    return utils_bin(e, params, ">=");
}

static lval *builtin_le(lenv *e, lval *params) {
    return utils_bin(e, params, "<=");
}

static lval *builtin_eq(lenv *e, lval *params) {
    return utils_bin(e, params, "==");
}

static lval *builtin_noteq(lenv *e, lval *params) {
    return utils_bin(e, params, "!=");
}

/*
 * If expression
 */
static lval *builtin_if(lenv *e, lval *params) {
    ASSERT(params != NULL, {lval_free(params);}, "");
    ASSERT(params->u.expr.top == 3, {lval_free(params);},
            "Function 'if' needs two parameters");
    lval *res = NULL;
    lval *cond = lval_eval(e, params->u.expr.cell[0]);
    lval *if_true = params->u.expr.cell[1];
    lval *if_false = params->u.expr.cell[2];
    params->u.expr.top = 0;
    ASSERT(cond->type == LVAL_NUM, {lval_free(params); lval_free(cond);
            lval_free(if_true); lval_free(if_false); },
            "Function 'if' condition is not valid");
    /* If true */
    if (cond->u.num) {
        res = lval_eval(e, if_true);
        lval_free(if_false);
    } else {
        res = lval_eval(e, if_false);
        lval_free(if_true);
    }
    lval_free(cond);
    lval_free(params);
    if (res) {
        return res;
    } else {
        return lval_err_format("Function 'if' error");
    }
}

/*
 * Condition expression
 * params: parameters list
 */
static lval *builtin_cond(lenv *e, lval *params) {
    ASSERT(params != NULL, {lval_free(params);}, "");
    ASSERT(params->u.expr.top >= 2, {lval_free(params);},
            "Function 'cond' needs as least two parameters");
    size_t i;
    int r;
    lval *res = NULL;
    lval *cond = NULL;
    for (i = 0; i < params->u.expr.top; ++i) {
        lval *expr = params->u.expr.cell[i];
        ASSERT(expr->u.expr.top == 2, {lval_free(params);},
            "Function 'cond' needs as least two parameters");
        cond = lval_remove(expr, 0);
        /* 'else' */
        if (i == params->u.expr.top - 1 &&
                cond->type == LVAL_SYM &&
                strcmp(cond->u.sym, "else") == 0) {
            res = lval_eval(e, lval_remove(expr, 0));
            lval_free(cond);
            break;
        }
        cond = lval_eval(e, cond);
        if (cond->type != LVAL_NUM) {
            lval_free(cond);
            goto lexit;
        }
        if (cond->u.num) {
            res = lval_eval(e, lval_remove(expr, 0));
            lval_free(cond);
            break;
        }
        lval_free(cond);
    }
lexit:
    lval_free(params);
    if (res == NULL) {
        res = lval_err_format("Function 'cond' unmatched case");
    }
    return res;
}

/*
 * Define a variable or functions in the specified environment
 * params[0]: the symbol(for function define, it can be function together
 *      with its formal parameters)
 * params[0]: the value of the corresponding symbol
 */
static lval *builtin_def(lenv *e, lval *params) {
    ASSERT(params != NULL, {lval_free(params);}, "");
    ASSERT(params->type == LVAL_SEXPR, {lval_free(params);},
            "Function 'define' passed incorrect type parameters");
    ASSERT(params->u.expr.top == 2, {lval_free(params);},
            "Function 'define' needs two parameters");
    if (params->u.expr.cell[0]->type == LVAL_SYM) {
        /* Evaluate the second parameters */
        params->u.expr.cell[1] = lval_eval(e, params->u.expr.cell[1]);
        if (params->u.expr.cell[1]->type == LVAL_ERR) {
            return lval_take_at(params, 1);
        }

        lval *sym = params->u.expr.cell[0];
        lval *val = params->u.expr.cell[1];
        params->u.expr.top = 0;
        lenv_put_sym(e, sym, val);
        lval_free(sym);
        lval_free(params);
        return val;
    } else if (params->u.expr.cell[0]->type == LVAL_SEXPR) {
        /*
         * Extract the function name from the first parameters
         */
        lval *sym = lval_remove(params->u.expr.cell[0], 0);
        lval *t = builtin_lambda(e, params); 
        lenv_put_sym(e, sym, t);
        lval_free(sym);
        return t;
    }
    return lval_err_format("Function 'define' first parameter is not a valid symbols");
}

/*
 * Exit the REPL
 * params: Empty parameters list
 */
static lval *builtin_exit(lenv *e, lval *params) {
    ASSERT(params != NULL, {lval_free(params);}, "");
    ASSERT(params->u.expr.top == 0, {lval_free(params);},
            "Function 'exit' needs no parameters");
    lval_free(params);
    return lval_status(STATUS_EXIT);
}

/*
 * Create a lambda expression
 * params[0]: the formal parameters list
 * params[1]: the body of lambda expression
 */
static lval *builtin_lambda(lenv *e, lval *params) {
    size_t i;
    ASSERT(params != NULL, {lval_free(params);}, "");
    ASSERT(params->u.expr.top == 2, {lval_free(params);},
            "Function 'lambda' needs two parameters");
    lval *formals = params->u.expr.cell[0];
    lval *body = params->u.expr.cell[1];
    ASSERT(formals->type == LVAL_SEXPR, {lval_free(params);},
            "Function 'lambda' first parameter is invalid");
    ASSERT(body->type == LVAL_SEXPR, {lval_free(params);},
            "Function 'lambda' first parameter is invalid");

    /* Check that first S-Expression has only symbols */
    for (i = 0; i < formals->u.expr.top; ++i) {
        if (formals->u.expr.cell[i]->type != LVAL_SYM) {
            lval_free(params);
            return lval_err_format(
                    "Function 'lambda' %zu-th formals is non-symbol");
        }
    }
    params->u.expr.top = 0;
    lval_free(params);
    return lval_lambda(formals, body);
}

/* 
 * Load and execute code in the specified file
 * params[0]: filename
 */

static lval *builtin_load(lenv *e, lval *params) {
    ASSERT(params != NULL, {goto lexit;}, "");
    ASSERT(params->u.expr.top == 1, {goto lexit;},
            "Function 'load' needs 1 parameters");
    size_t i;
    lval *res = NULL;
    lval *file = params->u.expr.cell[0];
    /* The parameter must be string */
    ASSERT(file->type == LVAL_STR, {goto lexit;},
            "Function 'load' needs 1 parameters");
    char *ext = strrchr(file->u.str, '.') + 1;
    if (strcmp(ext, "lisp") == 0) {
        /* Load lisp file */
        lval *content = parse_file(file->u.str);
        if (content == NULL) {
            res = lval_err_format("Function 'load' open file %s error",
                    file->u.str);
            goto lexit;
        }
        ASSERT(content->type == LVAL_SEXPR, {goto lexit;},
                "Content in file %s is invalid", file->u.str);
        while (content->u.expr.top) {
            lval *r = lval_eval(e, lval_remove(content, 0));
            if (r->type == LVAL_ERR) {
                lval_print(r, 0);
            }
            lval_free(r);
        }

        lval_free(content);
        /* Return success status */
        res = lval_status(STATUS_OK);
    } else if (strcmp(ext, "so") == 0) {
        /* Load shared object file */
        void *handle;
        char *error;
        size_t i;
        /* Dynamic loading the shared library specified by 'file' */
        handle = dlopen(file->u.str, RTLD_LAZY);
        if (!handle) {
            res = lval_err_format("Function 'load' open so file %s error",
                    file->u.str);
            goto lexit;
        }
        /* Get pointer to the exports list */
        func_entry *exports =
            (func_entry *)dlsym(handle, "exports");
        if ((error = dlerror()) != NULL) {
            res = lval_err_format("Function 'load' open so file error: %s",
                    error);
            goto lexit;
        }
        /* Add functions in exports list to the environment */
        for (i = 0; exports[i].name != NULL; ++i) {
            lenv_add_builtin(e, (char *)exports[i].name,
                    exports[i].func);
        }
        /* Leave the shared library close work to system */
        /* Do not call dlclose */
        /* Return success status */
        res = lval_status(STATUS_OK);
    }

lexit:
    lval_free(params);
    return res;
}

/*
 * Print the corresponding value
 * params[...] the lval which need to be print
 */

static lval *builtin_print(lenv *e, lval *params) {
    ASSERT(params != NULL, {lval_free(params);}, "");
    ASSERT(params->u.expr.top > 0, {lval_free(params);},
            "Function 'print' needs more parameters");
    size_t i;
    lval *err = lval_eval_params(e, params);
    if (err && err->type == LVAL_ERR) {
        return err;
    }
    
    for (i = 0; i < params->u.expr.top; ++i) {
        lval_print(params->u.expr.cell[0], 0);
        fprintf(stdout, " ");
    }
    fprintf(stdout, "\n");
    lval_free(params);
    return lval_status(STATUS_OK);
}

/*
 * Send an error message
 * params[0]: error message
 */
lval *builtin_error(lenv *e, lval *params) {
    ASSERT(params != NULL, {lval_free(params);}, "");
    ASSERT(params->u.expr.top == 1, {lval_free(params);},
            "Function 'error' needs one parameters");
    ASSERT(params->u.expr.cell[0]->type == LVAL_STR, {lval_free(params);},
            "Function 'error' passed incorrect type parameters");
    lval *res = lval_err_format(params->u.expr.cell[0]->u.str);
    lval_free(params);
    return res;
}

/*
 * Read a string from stdin
 * params: Empty parameters list
 */
lval *builtin_read_str(lenv *e, lval *params) {
    ASSERT(params != NULL, {lval_free(params);}, "");
    ASSERT(params->u.expr.top == 0, {lval_free(params);},
            "Function 'read-str' needs no parameters");
    fgets(buf, STREAM_BUF, stdin);
    lval *res = lval_str(buf);
    return res;
}

/*
 * Evaluate expression in specified environment
 * params: the expression to be evaluated
 */
lval *builtin_eval(lenv *e, lval *params) {
    ASSERT(params != NULL, {lval_free(params);}, ""); 
    ASSERT(params->type == LVAL_SEXPR, {lval_free(params);},
            "Function 'eval' passed incorrect type parameters");
    params->type = LVAL_SEXPR;
    return lval_eval(e, params);
}

/*
 * do operations in a list in sequence
 * params[...]: the operations that need operation
 * return: the value of the last operations
 */
lval *builtin_do(lenv *e, lval *params) {
    size_t i;
    lval *res;
    ASSERT(params != NULL, {lval_free(params);}, ""); 
    ASSERT(params->type == LVAL_SEXPR, {lval_free(params);},
            "Function 'do' passed incorrect type parameters");
    for (i = 0; i < params->u.expr.top; ++i) {
        res = lval_eval(e, params->u.expr.cell[i]);
        params->u.expr.cell[i] = res;
        if (res->type == LVAL_ERR) {
            return lval_take_at(params, i);
        }
    }
    return lval_take_at(params, i - 1);
}

/*
 * Create scope and evaluate
 * params[0]: the var-exp pair list
 * params[1]: the expression to evaluate in new scope
 */
lval *builtin_let(lenv *e, lval *params) {
    size_t i;
    ASSERT(params != NULL, {lval_free(params);}, ""); 
    ASSERT(params->u.expr.top == 2, {lval_free(params);},
            "Function 'let' needs 2 parameters");
    lval *var_exp_list = params->u.expr.cell[0];
    lval *exp = params->u.expr.cell[1];
    lenv *new_scope = lenv_new();
    /* Create new scope */
    for (i = 0; i < var_exp_list->u.expr.top; ++i) {
        lval *var_exp = var_exp_list->u.expr.cell[i];
        ASSERT(var_exp->type == LVAL_SEXPR &&var_exp->u.expr.top == 2,
                {lval_free(params);},
                "Function 'let' needs 2 parameters");
        lval *key = var_exp->u.expr.cell[0];
        lval *value = var_exp->u.expr.cell[1];
        ASSERT(key->type = LVAL_SYM, {lval_free(params);},
                "Function 'let' needs 2 parameters");
        value = lval_eval(e, value);
        if (value->type == LVAL_ERR) {
            /* In case that the err to be returned be freed */
            var_exp->u.expr.cell[1] = NULL;
            lval_free(params);
            return value;
        }
        var_exp->u.expr.cell[1] = value;
        lenv_put_sym(new_scope, key, value);
    }
    new_scope->parent = e;

    /* Evalute the body in new scope */
    /* Remove the exp from the params, so that it won't be freed again */
    params->u.expr.top = 1;
    lval *res = builtin_eval(new_scope, exp);
    lval_free(params);
    lenv_free(new_scope);
    return res;
}

/*
 * Open a file
 * params[0]: filename
 * params[1]: openmode <can be ommited>
 */
static lval *builtin_open(lenv *e, lval *params) {
    ASSERT(params != NULL, {lval_free(params);}, ""); 
    ASSERT(params->u.expr.top > 0, {lval_free(params);},
            "Function 'open' needs at least 1 parameters");
    char *mode = "r";
    FILE *f = NULL;
    lval *file = params->u.expr.cell[0];
    ASSERT(params->u.expr.top > 0, {lval_free(params);},
            "Function 'open' needs a filename as first parameter");
    if (params->u.expr.top == 2) {
        mode = params->u.expr.cell[1]->u.str;
    }
    f = fopen(file->u.str, mode);
    if (f == NULL) {
        lval_free(params);
        return lval_err_format("Function 'open' failed");
    }
    lval_free(params);
    return lval_file(f);
}

/*
 * Read data from a file
 * params[0]: file
 * params[1]: number of bytes to read
 * return: file content
 */
static lval *builtin_read(lenv *e, lval *params) {
    lval *err = lval_eval_params(e, params);
    if (err && err->type == LVAL_ERR) {
        return err;
    }
    size_t nread;
    lval *res;
    ASSERT(params != NULL, {lval_free(params);}, ""); 
    ASSERT(params->u.expr.top == 2, {lval_free(params);},
            "Function 'read' needs at 2 parameters");
    lval *lfile = params->u.expr.cell[0];
    lval *lsize = params->u.expr.cell[1];
    ASSERT(lfile->type = LVAL_FILE, {lval_free(params);},
            "Function 'read' needs a file object as first parameters");
    ASSERT(lsize->type == LVAL_NUM, {lval_free(params);},
            "Function 'read' needs a num as second parameter");
    ASSERT(lsize->u.num >= 0, {lval_free(params);},
            "Function 'read' needs second parameter to be positive");
    nread = fread(buf, sizeof(char), lsize->u.num, lfile->u.addi.fp);    
    buf[nread] = 0;
    res = lval_str(buf);

    lval_free(params);
    return res;
}

/*
 * write data to a file
 * params[0]: file
 * params[1]: the string to write
 * return: number of bytes writen to the file
 */

static lval *builtin_write(lenv *e, lval *params) {
    lval *err = lval_eval_params(e, params);
    if (err && err->type == LVAL_ERR) {
        return err;
    }
    size_t nwrite;
    lval *res;

    ASSERT(params != NULL, {lval_free(params);}, ""); 
    ASSERT(params->u.expr.top == 2, {lval_free(params);},
            "Function 'write' needs at 2 parameters");
    lval *lfile = params->u.expr.cell[0];
    lval *content = params->u.expr.cell[1];
    ASSERT(lfile->type = LVAL_FILE, {lval_free(params);},
            "Function 'write' needs a file object as first parameter");
    ASSERT(content->type == LVAL_STR, {lval_free(params);},
            "Function 'write' needs a str as second parameter");
    nwrite = fwrite(content->u.str, sizeof(char), strlen(content->u.str),
            lfile->u.addi.fp);
    res = lval_num(nwrite);
    lval_free(params);
    return res;
}

/*
 * Close the file
 * params[0]: the file to be closed
 */
static lval *builtin_close(lenv *e, lval *params) {
    lval *err = lval_eval_params(e, params);
    if (err && err->type == LVAL_ERR) {
        return err;
    }
    ASSERT(params != NULL, {lval_free(params);}, ""); 
    ASSERT(params->u.expr.top > 0, {lval_free(params);},
            "Function 'close' needs 1 parameters");
    lval *lfile = params->u.expr.cell[0];
    ASSERT(lfile->type = LVAL_FILE, {lval_free(params);},
            "Function 'close' needs a file object as first parameter");
    fclose(lfile->u.addi.fp);
    lval_free(params);
    return lval_status(STATUS_OK);
}
