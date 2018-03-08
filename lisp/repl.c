#include "lispy.h"

#include <stdio.h>
#include <stdlib.h>
#include <editline.h>

static void repl_init();
static void repl_exit();

static lenv *global;

void repl() {
    /* Prompt related information */
    puts(LISP_VERSION);

    /* Initialization work */
    repl_init();

    lval *res;

    /* Main loop */
    while (1) {
        char *input = readline("lispy> ");
        add_history(input);

        res = parse_str(input);
        if (res->type == LVAL_ERR) {
            if (res->u.err.i == ERR_INVALID_SYNTAX) {
                fprintf(stdout, "Invalid syntax\n");
                lval_free(res);
                continue;
            }
        }

        res = lval_eval(global, res);

        if (res->type == LVAL_STATUS) {
            /* Exit condition*/
            if (res->u.status == STATUS_EXIT) {
                lval_free(res);
                break;
            }
        } else if (res->type == LVAL_ERR) {
            /* Error condition */
            fprintf(stderr, "%s: %s\n", strerr(res->u.err.i), res->u.err.s);
        } else {
            /* Print the result */
            lval_print(res, 0);
        }

        /* Free the resource used in the procedure */
        lval_free(res);
    }

    repl_exit();
}

static void repl_init() {
    global = lenv_new();
    /* Add builtin functions to global envirionment */
    lenv_add_builtins(global);
    /* Add lispy library contents to global environment */
    lval *exp = parse_str("(load \"lib/standard.lisp\")");
    lval *res = lval_eval(global, exp);
    lval_free(res); 
}

static void repl_exit() {
    lenv_free(global);
}
