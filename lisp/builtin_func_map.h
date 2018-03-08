/*
 * Builtin_func_map is a map from builtin function name to
 * its corresponding function definition
 */

#ifndef BUILTIN_FUNC_H_
#define BUILTIN_FUNC_H_

#include "lispy.h"

func_entry builtin_func_map[] = {
    /* List functions */
    {"list", builtin_list},
    {"len", builtin_len},
    {"car", builtin_car},
    {"cdar", builtin_cdar},
    {"cddar", builtin_cddar},
    {"cdddar", builtin_cdddar},
    {"cdr", builtin_cdr},
    {"append", builtin_append},

    /* Mathematical functions */
    {"+", builtin_add},
    {"-", builtin_sub},
    {"*", builtin_mul},
    {"/", builtin_div},

    /* Cmparison functions */
    {">", builtin_gt},
    {"<", builtin_lt},
    {">=", builtin_ge},
    {"<=", builtin_le},
    {"==", builtin_eq},
    {"!=", builtin_noteq},

    /* Logical functions */
    {"and", builtin_and},
    {"or", builtin_or},
    {"not", builtin_not},
    
    /* Condition functions */
    {"if", builtin_if},
    {"cond", builtin_cond},

    /* Load function */
    {"load", builtin_load},

    /* define function */
    {"define", builtin_def},

    /* lambda function */
    {"lambda", builtin_lambda},

    /* error function */
    {"error", builtin_error},

    /* Exit function */
    {"exit", builtin_exit},

    /* Read string function */
    {"read-str", builtin_read_str},

    /* Evaluate function */    
    {"eval", builtin_eval},
    
    /* Evalute expression in new scope */
    {"let", builtin_let},

    /* Open file */
    {"open", builtin_open},

    /* Read from file */
    {"read", builtin_read},

    /* Write to file */
    {"write", builtin_write},

    /* Close file */
    {"close", builtin_close},

    /* Do sequential operations */
    {"do", builtin_do},
    
    {NULL, NULL},
};

#endif /* BUILTIN_FUNC_H_ */
