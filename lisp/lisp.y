%{
#define YYPARSER

#include "lispy.h"

static lval *root;

int yylex();

/* Parse from string */
typedef struct yy_buffer_state * YY_BUFFER_STATE;
extern YY_BUFFER_STATE yy_scan_string(const char *s);
extern void yy_delete_buffer(YY_BUFFER_STATE buffer);

/* Parse from file */
extern FILE *yyin;
%}

%union {
    lval *val;
}


%term LP
%term RP
%term LB
%term RB
%term SEP
%term DQ
%term SEMICOLON
%term <val> COMMENT
%term <val> STRING
%term <val> SYMBOL
%term <val> NUMBER

%type <val> lisp
%type <val> exprs
%type <val> expr
%type <val> qexpr
%type <val> sexpr

%%

lisp : exprs
     {
        $$ = $1;
        root = $$;
     }

exprs : exprs expr
      {
        if ($2 != NULL) {
            $$ = lval_push($1, $2);
        }
      }
      |
      {
        /* S-Expression as default expression */
        $$ = lval_expr(LVAL_SEXPR);
      }

expr : qexpr
     {
     }
     | sexpr
     {
     }
     | COMMENT
     {
        $$ = NULL;
     }
     | STRING
     {
     }
     | SYMBOL
     {
     }
     | NUMBER
     {
     }

qexpr : SEP LB exprs RB
      {
        /* Cast S-Expression to Q-Expression */
        $$ = $3;
        $$->type = LVAL_QEXPR;
      }

sexpr : LB exprs RB
      {
        $$ = $2;
      }

%%

int yyerror(const char *s) {
    //lval_free(root);
    root = lval_err(ERR_INVALID_SYNTAX);
    return 0;
}

lval *parse_str(const char *s) {
    YY_BUFFER_STATE buffer = yy_scan_string(s); 
    yyparse();
    yy_delete_buffer(buffer);
    return root;
}

lval *parse_file(const char *fname) {
    lval *res = NULL;
    size_t nread, nneed;
    FILE *f = fopen(fname, "r");
    if (f == NULL) {
        return NULL;
    }
    fseek(f, 0, SEEK_END);
    int length = ftell(f);
    rewind(f);
    char *buf = (char *)malloc(sizeof(char) * (length + 1));
    char *pos = buf;
    nneed = length;
    while (nneed) {
        nread = fread(buf, sizeof(char), nneed, f);
        if (nread == 0) {
            break;
        } else if (nread < 0) {
            goto lexit;
        }
        nneed -=  nread;
    }
    buf[length] = 0;
    res = parse_str(buf);
lexit:
    free(buf);
    return res;
}
