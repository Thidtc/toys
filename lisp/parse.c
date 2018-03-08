#include "lispy.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv) {
    //yyin = fopen("./t.lisp", "r");
    lval *root = parse_str("(eval (car `(1 2 3 4)))");
    printf("\n***********\n");
    lval_print(root, 0);
    printf("\n***********\n");
    lenv *e = lenv_new();
    lenv_add_builtins(e);
    lval *res = lval_eval(e, root);
    lval_print(res, 0);
    printf("\n***********\n");
    lval_free(res);
    lenv_free(e);
    //lval_free(root);
    return 0;
}
