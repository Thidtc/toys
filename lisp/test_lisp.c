#include "lispy.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
static size_t test_fail = 0;
static size_t test_pass = 0;

#define EXPECT_BASE(condition, format, actual, expectation)\
    do {\
        if (condition) {\
            test_pass++;\
        } else {\
            test_fail++;\
            fprintf(stderr, "%s:%d :expected value " format ", actual value " format "\n",\
                    __FILE__, __LINE__,\
                    expectation, actual);\
        }\
    } while (0)

#define EXPECT_TYPE(v, t)\
    EXPECT_BASE(v->type == t, "%s",\
            lval_typename(v->type), lval_typename(t))

#define EXPECT_ERR_NO(v, err_type)\
    EXPECT_BASE(v->u.err.i == err_type, "%d",\
            v->u.err.i, err_type)

#define EXPECT_ERR_STR(v, err_str)\
    EXPECT_BASE(strcmp(v->u.err.s, err_str) == 0, "%s",\
            v->u.err.s, err_str)

#define EXPECT_NUM(v, num_value)\
    EXPECT_BASE(v->u.num == num_value, "%ld",\
            v->u.num, num_value)

#define EXPECT_STATUS(v, sta)\
    EXPECT_BASE(v->u.status == sta, "%d",\
            v->u.status, sta)

#define EXPECT_SYM(v, symbol)\
    EXPECT_BASE(strncmp(v->u.sym, symbol, strlen(symbol)) == 0, "%s",\
            v->u.sym, symbol)

#define EXPECT_STR(v, s)\
    EXPECT_BASE(strncmp(v->u.str, s, strlen(s)) == 0, "%s",\
            v->u.str, s)

#define EXPECT_FUNC(v, func)\
    EXPECT_BASE(v->u.func == func, "%p",\
            v->u.func, func)

/* Test the actual number of expressions in LVAL_EXPR */
#define EXPECT_EXPR_TOP(v, size)\
    EXPECT_BASE(v->u.expr.top == size, "%zu",\
            v->u.expr.top, size)

void test_standard_lib() {
    lenv *e = lenv_new();
    lenv_add_builtins(e);

    lval *exp = parse_str("(load \"library/standard.lisp\")");
    lval *res = lval_eval(e, exp);
    EXPECT_TYPE(res, LVAL_STATUS);
    EXPECT_STATUS(res, STATUS_OK);
    lval_free(res);

    exp = parse_str("(true)");
    res = lval_eval(e, exp);
    EXPECT_TYPE(res, LVAL_NUM);
    EXPECT_NUM(res, 1l);
    lval_free(res);

    exp = parse_str("(false)");
    res = lval_eval(e, exp);
    EXPECT_TYPE(res, LVAL_NUM);
    EXPECT_NUM(res, 0l);
    lval_free(res);

    exp = parse_str("(nil)");
    res = lval_eval(e, exp);
    EXPECT_TYPE(res, LVAL_QEXPR);
    lval_free(res);

    exp = parse_str("((flip -) 1 2)");
    res = lval_eval(e, exp);
    EXPECT_TYPE(res, LVAL_NUM);
    EXPECT_NUM(res, 1l);
    lval_free(res);

    exp = parse_str("(ghost + 2 2 2)");
    res = lval_eval(e, exp);
    EXPECT_TYPE(res, LVAL_NUM);
    EXPECT_NUM(res, 6l);
    lval_free(res);

    exp = parse_str("(map not `(1 2 0))");
    res = lval_eval(e, exp);
    EXPECT_TYPE(res, LVAL_QEXPR);
    EXPECT_NUM(res->u.expr.cell[0], 0l);
    EXPECT_NUM(res->u.expr.cell[1], 0l);
    EXPECT_NUM(res->u.expr.cell[2], 1l);
    lval_free(res);

    exp = parse_str("(map (lambda (x) (+ x 1)) `(1 2 0))");
    res = lval_eval(e, exp);
    EXPECT_TYPE(res, LVAL_QEXPR);
    EXPECT_NUM(res->u.expr.cell[0], 2l);
    EXPECT_NUM(res->u.expr.cell[1], 3l);
    EXPECT_NUM(res->u.expr.cell[2], 1l);
    lval_free(res);

    exp = parse_str("filter (lambda (x) (> x 0)) `(1 2 0)");
    res = lval_eval(e, exp);
    EXPECT_TYPE(res, LVAL_QEXPR);
    EXPECT_EXPR_TOP(res, 2l);
    EXPECT_NUM(res->u.expr.cell[0], 1l);
    EXPECT_NUM(res->u.expr.cell[1], 2l);
    lval_free(res);

    exp = parse_str("(foldl + 0 `(1 2 3))");
    res = lval_eval(e, exp);
    EXPECT_TYPE(res, LVAL_NUM);
    EXPECT_NUM(res, 6l);
    lval_free(res);

    /* Clean up */
    lenv_free(e);
    fprintf(stdout, "test_standard_lib passed\n");
}

void test_list_func() {
    lenv *e = lenv_new();
    lenv_add_builtins(e);

    lval *root = parse_str("(eval (car `(1 2 3 4)))");
    lval *res = lval_eval(e, root);
    EXPECT_TYPE(res, LVAL_NUM);
    EXPECT_NUM(res, 1l);
    lval_free(res);

    root = parse_str("(car (list 1 2))");
    res = lval_eval(e, root);
    EXPECT_TYPE(res, LVAL_NUM);
    EXPECT_NUM(res, 1l);
    lval_free(res);

    root = parse_str("((eval (car `(+ - * /))) 1 2)");
    res = lval_eval(e, root);
    EXPECT_TYPE(res, LVAL_NUM);
    EXPECT_NUM(res, 3l);
    lval_free(res);

    root = parse_str("(append \"123\" \"456\")");
    res = lval_eval(e, root);
    EXPECT_TYPE(res, LVAL_STR);
    EXPECT_STR(res, "123456");
    lval_free(res);

    root = parse_str("(car \"123\")");
    res = lval_eval(e, root);
    EXPECT_TYPE(res, LVAL_STR);
    EXPECT_STR(res, "1");
    lval_free(res);

    root = parse_str("(cdr \"123\")");
    res = lval_eval(e, root);
    EXPECT_TYPE(res, LVAL_STR);
    EXPECT_STR(res, "23");
    lval_free(res);

    root = parse_str("(len `(12 3 4))");
    res = lval_eval(e, root);
    EXPECT_TYPE(res, LVAL_NUM);
    EXPECT_NUM(res, 3l);
    lval_free(res);

    /* Clean up */
    lenv_free(e);
    fprintf(stdout, "test_list passed\n");
}

void test_math_func() {
    lenv *e = lenv_new();
    lenv_add_builtins(e);

    lval *root = parse_str("(+ 1 (* 2 3 4) 5)");
    lval *res = lval_eval(e, root);
    EXPECT_TYPE(res, LVAL_NUM);
    EXPECT_NUM(res, 30l);
    lval_free(res);

    root = parse_str("(/ 2 1)");
    res = lval_eval(e, root);
    EXPECT_TYPE(res, LVAL_NUM);
    EXPECT_NUM(res, 2l);
    lval_free(res);

    /* Clean up */
    lenv_free(e);

    fprintf(stdout, "test_math passed\n");
}

void test_error() {
    lenv *e = lenv_new();
    lenv_add_builtins(e);

    lval *root = parse_str("(foo 1)");
    lval *res = lval_eval(e, root);

    EXPECT_TYPE(res, LVAL_ERR);
    lval_free(res);

    root = parse_str("(+ 1 `(1 2) 3)");
    res = lval_eval(e, root);
    EXPECT_TYPE(res, LVAL_ERR);
    lval_free(res);

    /* Clean up */
    lenv_free(e);

    fprintf(stdout, "test_error passed\n");
}

void test_def_func() {
    lenv *e = lenv_new();
    lenv_add_builtins(e);

    lval *exp1 = parse_str("(define a 1)");
    lval *exp2 = parse_str("a");
    lval *exp3 = parse_str("(+ a 1)");

    lval *res = lval_eval(e, exp1);
    lval_free(res);

    res = lval_eval(e, exp2);
    EXPECT_TYPE(res, LVAL_NUM);
    EXPECT_NUM(res, 1l);
    lval_free(res);

    res = lval_eval(e, exp3);
    EXPECT_TYPE(res, LVAL_NUM);
    EXPECT_NUM(res, 2l);
    lval_free(res);

    lenv_free(e);

    fprintf(stdout, "test_def passed\n");
}

void test_lambda_func() {
    lenv *e = lenv_new();
    lenv_add_builtins(e);

    lval *exp1 = parse_str("(lambda (x y) (+ x y))");
    lval *exp2 = parse_str("(define func1 (lambda (x y) (+ x y)))");
    lval *exp3 = parse_str("(func1 1 2)");
    lval *exp4 = parse_str("(define (func2 x y) (* x y))");
    lval *exp5 = parse_str("(func2 1 2)");
    lval *exp6 = parse_str("(define func3 (func1 1))");
    lval *exp7 = parse_str("(func3 4)");
    lval *exp8 = parse_str("(define func4 (lambda(x . args) (+ x (car args))))");
    lval *exp9 = parse_str("(func4 1 2 3 4 5)");

    lval *res = lval_eval(e, exp1);
    EXPECT_TYPE(res, LVAL_FUNC);
    lval_free(res);

    res = lval_eval(e, exp2);
    EXPECT_TYPE(res, LVAL_FUNC);
    lval_free(res);

    res = lval_eval(e, exp3);
    EXPECT_TYPE(res, LVAL_NUM);
    EXPECT_NUM(res, 3l);
    lval_free(res);

    res = lval_eval(e, exp4);
    EXPECT_TYPE(res, LVAL_FUNC);
    lval_free(res);

    res = lval_eval(e, exp5);
    EXPECT_TYPE(res, LVAL_NUM);
    EXPECT_NUM(res, 2l);
    lval_free(res);

    res = lval_eval(e, exp6);
    EXPECT_TYPE(res, LVAL_FUNC);
    lval_free(res);

    res = lval_eval(e, exp7);
    EXPECT_TYPE(res, LVAL_NUM);
    EXPECT_NUM(res, 5l);
    lval_free(res);

    res = lval_eval(e, exp8);
    EXPECT_TYPE(res, LVAL_FUNC);
    lval_free(res);

    res = lval_eval(e, exp9);
    EXPECT_TYPE(res, LVAL_NUM);
    EXPECT_NUM(res, 3l);
    lval_free(res);

    lenv_free(e);
    
    fprintf(stdout, "test_lambda_func passed\n");
}

void test_logic_func() {
    lenv *e = lenv_new();
    lenv_add_builtins(e);
    
    lval *exp1 = parse_str("(and 1 0)");
    lval *exp2 = parse_str("(and 1 1)");
    lval *exp3 = parse_str("(or 0 0)");
    lval *exp4 = parse_str("(or 1 0)");
    lval *exp5 = parse_str("(or 1 1)");
    lval *exp6 = parse_str("(not 1)");
    lval *exp7 = parse_str("(not 0)");

    lval *res = lval_eval(e, exp1);
    EXPECT_TYPE(res, LVAL_NUM);
    EXPECT_NUM(res, 0l);
    lval_free(res);

    res = lval_eval(e, exp2);
    EXPECT_TYPE(res, LVAL_NUM);
    EXPECT_NUM(res, 1l);
    lval_free(res);

    res = lval_eval(e, exp3);
    EXPECT_TYPE(res, LVAL_NUM);
    EXPECT_NUM(res, 0l);
    lval_free(res);

    res = lval_eval(e, exp4);
    EXPECT_TYPE(res, LVAL_NUM);
    EXPECT_NUM(res, 1l);
    lval_free(res);

    res = lval_eval(e, exp5);
    EXPECT_TYPE(res, LVAL_NUM);
    EXPECT_NUM(res, 1l);
    lval_free(res);

    res = lval_eval(e, exp6);
    EXPECT_TYPE(res, LVAL_NUM);
    EXPECT_NUM(res, 0l);
    lval_free(res);

    res = lval_eval(e, exp7);
    EXPECT_TYPE(res, LVAL_NUM);
    EXPECT_NUM(res, 1l);
    lval_free(res);

    lenv_free(e);

    fprintf(stdout, "test_logic_func passed\n");
}

void test_cmp_func() {
    lenv *e = lenv_new();
    lenv_add_builtins(e);

    /* Number comparison */
    /* True cases */
    lval *exp1 = parse_str("(> 1 0)");
    lval *exp2 = parse_str("(< 1 2)");
    lval *exp3 = parse_str("(>= 2 2)");
    lval *exp4 = parse_str("(<= 3 3)");
    lval *exp5 = parse_str("(== 1 1)");
    lval *exp6 = parse_str("(!= 2 0)");
    /* False case */
    lval *exp7 = parse_str("== 1 2");
    lval *exp8 = parse_str("< 1 -1");
    
    /* String comparison */
    /* True cases */
    lval *exp9 = parse_str("(> \"b\" \"a\")");
    lval *exp10 = parse_str("(== \"abc\" \"abc\")");
    lval *exp11 = parse_str("(!= \"a\" \"ab\")");
    /* false cases */
    lval *exp12 = parse_str("(< \"ab\" \"aa\")");

    lval *res = lval_eval(e, exp1);
    EXPECT_TYPE(res, LVAL_NUM);
    EXPECT_NUM(res, 1l);
    lval_free(res);

    res = lval_eval(e, exp2);
    EXPECT_TYPE(res, LVAL_NUM);
    EXPECT_NUM(res, 1l);
    lval_free(res);

    res = lval_eval(e, exp3);
    EXPECT_TYPE(res, LVAL_NUM);
    EXPECT_NUM(res, 1l);
    lval_free(res);

    res = lval_eval(e, exp4);
    EXPECT_TYPE(res, LVAL_NUM);
    EXPECT_NUM(res, 1l);
    lval_free(res);

    res = lval_eval(e, exp5);
    EXPECT_TYPE(res, LVAL_NUM);
    EXPECT_NUM(res, 1l);
    lval_free(res);

    res = lval_eval(e, exp6);
    EXPECT_TYPE(res, LVAL_NUM);
    EXPECT_NUM(res, 1l);
    lval_free(res);

    res = lval_eval(e, exp7);
    EXPECT_TYPE(res, LVAL_NUM);
    EXPECT_NUM(res, 0l);
    lval_free(res);

    res = lval_eval(e, exp8);
    EXPECT_TYPE(res, LVAL_NUM);
    EXPECT_NUM(res, 0l);
    lval_free(res);

    res = lval_eval(e, exp9);
    EXPECT_TYPE(res, LVAL_NUM);
    EXPECT_NUM(res, 1l);
    lval_free(res);

    res = lval_eval(e, exp10);
    EXPECT_TYPE(res, LVAL_NUM);
    EXPECT_NUM(res, 1l);
    lval_free(res);

    res = lval_eval(e, exp11);
    EXPECT_TYPE(res, LVAL_NUM);
    EXPECT_NUM(res, 1l);
    lval_free(res);

    res = lval_eval(e, exp12);
    EXPECT_TYPE(res, LVAL_NUM);
    EXPECT_NUM(res, 0l);
    lval_free(res);

    lenv_free(e);

    fprintf(stdout, "test_cmp_func passed\n");
}

void test_cond_func() {
    lenv *e = lenv_new();
    lenv_add_builtins(e);

    lval *exp1 = parse_str("(if (== 1 1) 1 2)");
    lval *exp2 = parse_str("(if (== 1 2) (+ 1 2) (- 1 2))");
    lval *exp3 = parse_str("(cond ((== 1 2) 1)"
            "((== 1 1) 2))");
    lval *exp4 = parse_str("(cond ((== 1 2) 1)"
            "((== 2 1) 1)"
            "(else 0))");
    /* Error case */
    lval *exp5 = parse_str("(cond ((== 1 2) 1)"
            "((== 1 2) 2))");
    lval *exp6 = parse_str("(cond ((=== 1 2) 1)"
            "((== 1 2) 2))");

    lval *res = lval_eval(e, exp1);
    EXPECT_TYPE(res, LVAL_NUM);
    EXPECT_NUM(res, 1l);
    lval_free(res);

    res = lval_eval(e, exp2);
    EXPECT_TYPE(res, LVAL_NUM);
    EXPECT_NUM(res, -1l);
    lval_free(res);

    res = lval_eval(e, exp3);
    EXPECT_TYPE(res, LVAL_NUM);
    EXPECT_NUM(res, 2l);
    lval_free(res);

    res = lval_eval(e, exp4);
    EXPECT_TYPE(res, LVAL_NUM);
    EXPECT_NUM(res, 0l);
    lval_free(res);

    res = lval_eval(e, exp5);
    EXPECT_TYPE(res, LVAL_ERR);
    lval_free(res);

    res = lval_eval(e, exp6);
    EXPECT_TYPE(res, LVAL_ERR);
    lval_free(res);

    lenv_free(e);

    fprintf(stdout, "test_cond_func passed\n");
}

/*
 * This test case need file t.lisp, with content
 *
 * (define a 1)
 * (define b 2)
 */
void test_load_func() {
    lenv *e = lenv_new();
    lenv_add_builtins(e);

    /* Load .lisp lib */
    lval *exp1 = parse_str("(load \"t.lisp\")");
    lval *exp2 = parse_str("(a)");
    lval *exp3 = parse_str("(+ a b)");
    lval *exp4 = parse_str("(c)");

    lval *res = lval_eval(e, exp1);
    EXPECT_TYPE(res, LVAL_STATUS);
    lval_free(res);

    res = lval_eval(e, exp2);
    EXPECT_TYPE(res, LVAL_NUM);
    EXPECT_NUM(res, 1l);
    lval_free(res);

    res = lval_eval(e, exp3);
    EXPECT_TYPE(res, LVAL_NUM);
    EXPECT_NUM(res, 3l);
    lval_free(res);

    res = lval_eval(e, exp4);
    EXPECT_TYPE(res, LVAL_NUM);
    EXPECT_NUM(res, 2l);
    lval_free(res);
    
    /* Load .so lib */
    lval *exp = parse_str("(load \"./cfunc.so\")");
    res = lval_eval(e, exp);
    EXPECT_TYPE(res, LVAL_STATUS);
    EXPECT_STATUS(res, STATUS_OK);
    lval_free(res);

    exp = parse_str("(test)");
    res = lval_eval(e, exp);
    EXPECT_TYPE(res, LVAL_STR);
    EXPECT_STR(res, "hello");
    lval_free(res);

    lenv_free(e);

    fprintf(stdout, "test_load_func passed\n");
}

void test_error_func() {
    lenv *e = lenv_new();
    lenv_add_builtins(e);

    lval *exp1 = parse_str("(error \"hello\")");

    lval *res = lval_eval(e, exp1);
    EXPECT_TYPE(res, LVAL_ERR);
    EXPECT_ERR_STR(res, "hello");
    lval_free(res);

    lenv_free(e);

    fprintf(stdout, "test_error_func passed\n");
}

void test_file_func() {
    lenv *e = lenv_new();
    lenv_add_builtins(e);

    lval *exp1 = parse_str("(define f (open \"test.txt\" \"w+\"))");
    lval *exp2 = parse_str("(write f \"1234\")");
    lval* exp3 = parse_str("(close f)");
    lval *exp4 = parse_str("(define f (open \"test.txt\"))");
    lval *exp5 = parse_str("(read f 4)");

    lval *res = lval_eval(e, exp1);
    EXPECT_TYPE(res, LVAL_FILE);
    lval_free(res);

    res = lval_eval(e, exp2);
    EXPECT_TYPE(res, LVAL_NUM);
    EXPECT_NUM(res, 4l);
    lval_free(res);

    res = lval_eval(e, exp3);
    EXPECT_TYPE(res, LVAL_STATUS);
    EXPECT_STATUS(res, STATUS_OK);
    lval_free(res);

    res = lval_eval(e, exp4);
    EXPECT_TYPE(res, LVAL_FILE);
    lval_free(res);

    res = lval_eval(e, exp5);
    EXPECT_TYPE(res, LVAL_STR);
    EXPECT_STR(res, "1234");
    lval_free(res);

    lenv_free(e);

    fprintf(stdout, "test_file_func passed\n");
}

void test_other_func() {
    lenv *e = lenv_new();
    lenv_add_builtins(e);

    lval *exp = parse_str("(do 1 2 (car `(1 2 3)))");
    lval *res = lval_eval(e, exp);
    EXPECT_TYPE(res, LVAL_NUM);
    EXPECT_NUM(res, 1l);
    lval_free(res);

    exp = parse_str("(let ((x 1) (y 2)) (+ x y))");
    res = lval_eval(e, exp);
    EXPECT_TYPE(res, LVAL_NUM);
    EXPECT_NUM(res, 3l);
    lval_free(res);

    lenv_free(e);
    fprintf(stdout, "test_other_func passed\n");
}

void test_recursive() {
    lenv *e = lenv_new();
    lenv_add_builtins(e);

    /* Fibnacci */
    lval *exp1 = parse_str("(define (fib x)"
        "(cond ((== x 1) 1)"
            "((== x 2) 1)"
            "(else ( + (fib (- x 1)) (fib (- x 2))))"
        "))");
    lval *exp2 = parse_str("(fib 1)");
    lval *exp3 = parse_str("(fib 2)");
    lval *exp4 = parse_str("(fib 3)");
    lval *exp5 = parse_str("(fib 5)");


    lval *res = lval_eval(e, exp1);
    EXPECT_TYPE(res, LVAL_FUNC);
    lval_free(res);

    res = lval_eval(e, exp2);
    EXPECT_TYPE(res, LVAL_NUM);
    EXPECT_NUM(res, 1l);
    lval_free(res);

    res = lval_eval(e, exp3);
    EXPECT_TYPE(res, LVAL_NUM);
    EXPECT_NUM(res, 1l);
    lval_free(res);

    res = lval_eval(e, exp4);
    EXPECT_TYPE(res, LVAL_NUM);
    EXPECT_NUM(res, 2l);
    lval_free(res);

    res = lval_eval(e, exp5);
    EXPECT_TYPE(res, LVAL_NUM);
    EXPECT_NUM(res, 5l);
    lval_free(res);

    lenv_free(e);
    
    fprintf(stdout, "test_recursive passed\n");
}

/* TODO: fix the memory leak when encounting parsing error */
void test_invalid_syntax() {
    lenv *e = lenv_new();
    lenv_add_builtins(e);

    lval *exp1 = parse_str("(= 1");

    lval *res = lval_eval(e, exp1);
    EXPECT_TYPE(res, LVAL_ERR);
    EXPECT_ERR_NO(res, ERR_INVALID_SYNTAX);
    lval_free(res);

    lenv_free(e);
}

int main() {
    test_standard_lib();
    test_list_func();
    test_math_func();
    test_def_func();
    test_error();
    test_lambda_func();
    test_logic_func();
    test_cmp_func();
    test_cond_func();
    test_load_func();
    test_error_func();
    test_file_func();
    test_other_func();
    test_recursive();

    //test_invalid_syntax();
    
    /* Test results */
    fprintf(stdout, "TOTAL: %zu\n", test_fail + test_pass);
    fprintf(stdout, "FAIL: %zu\n", test_fail);
    fprintf(stdout, "PASS: %zu\n", test_pass);
    return 0;
}
