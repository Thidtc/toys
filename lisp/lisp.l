%{
#include "lispy.h"
#include "lisp.tab.h"

#include <string.h>

int lineno = 0;

%}

%option noyywrap

%%

"("         { return LB; }
")"         { return RB; }
"{"         { return LP; }
"}"         { return RP; }
";"         { return SEMICOLON; }
"`"         { return SEP; }

-?[0-9]+                {
                            yylval.val = lval_num(atoi(yytext));
                            return NUMBER;
                        }
\"(\\.|[^"])*\"         {
                            yylval.val = lval_str_with_len(yytext + 1,
                                    strlen(yytext) - 2);
                            return STRING; 
                        }
[_a-zA-Z+-?!&*=<>][_a-zA-Z0-9+-?!&*=<>]*   {
                            yylval.val = lval_sym(yytext);
                            return SYMBOL;
                        }
;[^\n]*                 { return COMMENT; }

"\n"                    { lineno++; }

.                       {}

%%

