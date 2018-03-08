%{
#define YYPARSER

#include "global.h"
#include "utils.h"

char *savedName;
int saveLineNo;

static Tree *rootTree;
%}

%union {
	char p_s[MAX_NAME_LEN];
	char p_c;
	int p_i;
	double p_r;
	Tree *p_tree;
}

%term LP
%term RP
%term LB
%term RB
%term SEMI
%term COLON
%term COMMA
%term MUL
%term DIV
%term PLUS
%term MINUS
%term GE
%term GT
%term LE
%term LT
%term EQUAL
%term UNEQUAL
%term ASSIGN
%term DOTDOT
%term DOT
%term AND
%term ARRAY
%term BBEGIN
%term CASE
%term CONST
%term DO
%term DOWNTO
%term ELSE
%term END
%term FOR
%term FUNCTION
%term GOTO
%term IF
%term MOD
%term NOT
%term OF
%term OR
%term PROC
%term PROCEDURE
%term PROGRAM
%term READ
%term RECORD
%term REPEAT
%term THEN
%term TO
%term TYPE
%term UNTIL
%term VAR
%term WHILE
%term <p_s> SYS_PROC
%term <p_s> SYS_FUNCT
%term <p_s> SYS_CON
%term <p_s> SYS_TYPE
%term <p_i> INTEGER
%term <p_r> REAL
%term <p_c> CHAR
%term <p_s> STRING
%term <p_s> ID

%type <p_tree> program
%type <p_tree> program_head
%type <p_tree> routine
%type <p_tree> sub_routine
%type <p_tree> routine_head
%type <p_tree> label_part
%type <p_tree> const_part
%type <p_tree> const_expr_list
%type <p_tree> const_value
%type <p_tree> type_part
%type <p_tree> type_decl_list
%type <p_tree> type_definition
%type <p_tree> type_decl
%type <p_tree> simple_type_decl
%type <p_tree> array_type_decl
%type <p_tree> record_type_decl
%type <p_tree> field_decl_list
%type <p_tree> field_decl
%type <p_tree> name_list
%type <p_tree> var_part
%type <p_tree> var_decl_list
%type <p_tree> var_decl
%type <p_tree> routine_part
%type <p_tree> function_decl
%type <p_tree> function_head
%type <p_tree> procedure_decl
%type <p_tree> procedure_head
%type <p_tree> parameters
%type <p_tree> para_decl_list
%type <p_tree> para_type_list
%type <p_tree> var_para_list
%type <p_tree> val_para_list
%type <p_tree> routine_body
%type <p_tree> compound_stmt
%type <p_tree> stmt_list
%type <p_tree> stmt
%type <p_tree> non_label_stmt
%type <p_tree> assign_stmt
%type <p_tree> proc_stmt
%type <p_tree> if_stmt
%type <p_tree> else_clause
%type <p_tree> repeat_stmt
%type <p_tree> while_stmt
%type <p_tree> for_stmt
%type <p_i> direction
%type <p_tree> case_stmt
%type <p_tree> case_expr_list
%type <p_tree> case_expr
%type <p_tree> goto_stmt
%type <p_tree> expression_list
%type <p_tree> expression
%type <p_tree> expr
%type <p_tree> term
%type <p_tree> factor
%type <p_tree> args_list

%%

program : program_head routine DOT
		{
			$$ = newModuleTree(MODULE_PROGRAM, lineno);
			$$->child[0] = $1;
			$$->child[1] = $2;
			rootTree = $$;
		}
		;
program_head : PROGRAM ID SEMI
		{
			$$ = newModuleTree(MODULE_PROGRAMHEAD, lineno);
			$$->attr.name = copyString($2);
		}
		;
routine : routine_head routine_body
		{
			$$ = newModuleTree(MODULE_ROUTINE, lineno);
			$$->child[0] = $1;
			$$->child[1] = $2;
		}		
		;
sub_routine : routine_head routine_body
		{
			$$ = newModuleTree(MODULE_ROUTINE, lineno);
			$$->child[0] = $1;
			$$->child[1] = $2;
		}			
		;
routine_head : label_part const_part type_part var_part routine_part
		{
			$$ = newModuleTree(MODULE_ROUTINEHEAD, lineno);
			$$->child[0] = $1;
			$$->child[1] = $2;
			$$->child[2] = $3;
			$$->child[3] = $4;
			$$->child[4] = $5;
		}
		;
label_part : 
		{
			$$ = NULL;
		}
		;
const_part : CONST const_expr_list 
		{
			$$ = $2;
		}
		| 
		{
			$$ = NULL;
		}
		;
const_expr_list : const_expr_list ID EQUAL const_value SEMI
		{
			Tree *t = $1;
			if (t != NULL) {
				for (t; t->sibling; t = t->sibling);
				t->sibling = newStmtTree(STMT_EQUAL, lineno);
				t->sibling->child[0] = $4;
				t->sibling->attr.name = copyString($2);
				$$ = $1;
			} else {
				$$ = $1;
			}
		}
		| ID EQUAL const_value SEMI
		{
			$$ = newStmtTree(STMT_EQUAL, lineno);
			$$->child[0] = $3;
			$$->attr.name = copyString($1);
		}
		;
const_value : INTEGER 
		{
			$$ = newExpTree(EXP_CONST_INT, lineno);
			$$->attr.int_val = $1;
		}
		| REAL
		{
			$$ = newExpTree(EXP_CONST_REAL, lineno);
			$$->attr.real_val = $1;
		}
		| CHAR 
		{
			$$ = newExpTree(EXP_CONST_CHAR, lineno);
			$$->attr.char_val = $1;
		}
		| STRING 
		{
			$$ = newExpTree(EXP_CONST_STR, lineno);
			$$->attr.string_val = copyString($1);
		}
		| SYS_CON
		{
			$$ = newExpTree(EXP_CONST_DEFINE, lineno);
			$$->attr.string_val = copyString($1);
		}
		;
type_part : TYPE type_decl_list 
		{
			$$ = $2;
		}
		| 
		{
			$$ = NULL;
		}
		;
type_decl_list : type_decl_list type_definition 
		{
			Tree *t = $1;
			if (t != NULL) {
				for (t; t->sibling; t = t->sibling);
				t->sibling = $2;
				$2->sibling = NULL;
				$$ = $1;
			} else {
				$$ = $2;
			}
		}
		| type_definition
		{
			$$ = $1;
		}
		;
type_definition : ID EQUAL type_decl SEMI
		{
			$$ = newStmtTree(STMT_EQUAL, lineno);
			$$->child[0] = $3;
			$$->attr.name = copyString($1);
		}		
		;
type_decl : simple_type_decl 
		{
			$$ = $1;
		}	
		| array_type_decl 
		{
			$$ = $1;
		}
		| record_type_decl
		{
			$$ = $1;
		}
		;
simple_type_decl : SYS_TYPE 
		{
			$$ = newDeclTree(DECL_VAR, lineno);
			$$->attr.name = copyString($1);
		}
		| ID 
		{
			$$ = newDeclTree(DECL_VAR, lineno);
			$$->attr.name = copyString($1);
		}
		| LP name_list RP
		{
			$$ = $2;
		}
		| const_value DOTDOT const_value
		{
			$$ = newDeclTree(DECL_SIMPLE, lineno);
			$$->child[0] = $1;
			$$->child[1] = $3;
		}
		| MINUS const_value DOTDOT const_value
		{
			$$ = newDeclTree(DECL_SIMPLE, lineno);
			$$->child[0] = $2;
			$$->child[1] = $4;
		}
		| MINUS const_value DOTDOT MINUS const_value
		{
			$$ = newDeclTree(DECL_SIMPLE, lineno);
			$$->child[0] = $2;
			$$->child[1] = $5;
		}
		| ID DOTDOT ID
		{
			$$ = newExpTree(EXP_ID, lineno);
			$$->attr.name = copyString($1);
			$$->child[0] = newExpTree(EXP_ID, lineno);
			$$->child[0]->attr.name = copyString($3);
		}
		;
array_type_decl : ARRAY LB simple_type_decl RB OF type_decl
		{
			$$ = newDeclTree(DECL_ARRAY, lineno);
			$$->child[0] = $3;
			$$->child[1] = $6;
		};
record_type_decl : RECORD field_decl_list END
		{
			$$ = $2;
		};
field_decl_list : field_decl_list field_decl 
		{
			Tree *t = $1;
			if (t != NULL) {
				for(t; t->sibling; t = t->sibling);
				t->sibling = $2;
				$2->sibling = NULL;
				$$ = $1;
			} else {
				$$ = $2;
			}
		}
		| field_decl
		{
			$$ = $1;
		}
		;
field_decl : name_list COLON type_decl SEMI
		{
			$$ = newDeclTree(DECL_FIELD, lineno);
			$$->child[0] = $1;
			$$->child[1] = $3;
		};
name_list : name_list COMMA ID 
		{
			Tree *t = $1;
			if (t != NULL) {
				for (t; t->sibling; t = t->sibling);
				t->sibling = newExpTree(EXP_ID, lineno);
				t->sibling->attr.name = copyString($3);
				$$ = $1;
			} else {
				$$ = newExpTree(EXP_ID, lineno);
				$$->attr.name = copyString($3);
			}
		}		
		| ID
		{
			$$ = newExpTree(EXP_ID, lineno);
			$$->attr.name = copyString($1);
		}
		;
var_part : VAR var_decl_list 
		{
			$$ = $2;
		}
		| 
		{
			$$ = NULL;
		}
		;
var_decl_list : var_decl_list var_decl 
		{
			Tree *t = $1;
			if (t != NULL) {
				for (t; t->sibling; t = t->sibling);
				t->sibling = $2;
				$2->sibling = NULL;
				$$ = $1;
			} else {
				$$ = $2;
			}
		}		
		| var_decl
		{
			$$ = $1;
		}
		;
var_decl : name_list COLON type_decl SEMI
		{
			$$ = newDeclTree(DECL_FIELD, lineno);
			$$->child[0] = $1;
			$$->child[1] = $3;
		}
		;
routine_part: routine_part function_decl 
		{
			Tree *t = $1;
			if (t != NULL) {
				for (t; t->sibling; t = t->sibling);
				t->sibling = $2;
				$2->sibling = NULL;
				$$ = $1;
			} else {
				$$ = $2;
			}
		}		
		| routine_part procedure_decl
		{
			Tree *t = $1;
			if (t != NULL) {
				for (t; t->sibling; t = t->sibling);
				t->sibling = $2;
				$2->sibling = NULL;
				$$ = $1;
			} else {
				$$ = $2;
			}	
		}	
		| function_decl 
		{
			$$ = $1;
		}
		| procedure_decl 
		{
			$$ = $1;
		}
		|
		{
			$$ = NULL;
		}
		;
function_decl : function_head SEMI sub_routine SEMI
		{
			$$ = newModuleTree(MODULE_FUNCTION, lineno);
			$$->child[0] = $1;
			$$->child[1] = $3;
		};
function_head : FUNCTION ID parameters COLON simple_type_decl
		{
			$$ = newModuleTree(MODULE_FUNCTIONHEAD, lineno);
			$$->child[0] = $3;
			$$->child[1] = $5;
			$$->attr.name = copyString($2);
		}
		;
procedure_decl : procedure_head SEMI sub_routine SEMI
		{
			$$ = newModuleTree(MODULE_PROCEDURE, lineno);
			$$->child[0] = $1;
			$$->child[1] = $3;
		}
		;
procedure_head : PROCEDURE ID parameters
		{
			$$ = newModuleTree(MODULE_PROCEDUREHEAD, lineno);
			$$->child[0] = $3;
			$$->attr.name = copyString($2);
		}		
		;
parameters : LP para_decl_list RP 
		{
			$$ = $2;
		}		
		| 
		{
			$$ = NULL;
		}
		;
para_decl_list : para_decl_list SEMI para_type_list 
		{
			Tree *t = $1;
			if (t != NULL) {
				for (t; t->sibling; t = t->sibling);
				t->sibling = $3;
				$3->sibling = NULL;
				$$ = $1;
			} else {
				$$ = $3;
			}
		}
		| para_type_list
		{
			$$ = $1;
		}
		;
para_type_list : var_para_list COLON simple_type_decl
		{
			$$ = newDeclTree(DECL_PARA, lineno);
			$$->child[0] = $1;
			$$->child[1] = $3;
		}
		| val_para_list COLON simple_type_decl
		{
			$$ = newDeclTree(DECL_PARA, lineno);
			$$->child[0] = $1;
			$$->child[1] = $3;
		}
		;
var_para_list : VAR name_list
		{
			$$ = $2;
		}
		;
val_para_list : name_list
		{
			$$ = $1;
		}
		;
routine_body : compound_stmt
		{
			$$ = $1;
		}
		;
compound_stmt : BBEGIN stmt_list END
		{
			$$ = $2;
		}
		;
stmt_list : stmt_list stmt SEMI 
		{
			if ($1 != NULL) {
				Tree *t = $1;
				for (t; t->sibling; t = t->sibling);
				t->sibling = $2;
				$2->sibling = NULL;
				$$ = $1;
			} else {
				$$ = $2;
			}
		}		
		| 
		{
			$$ = NULL;
		}
		;
stmt : INTEGER COLON non_label_stmt 
		{
			$$ = newStmtTree(STMT_LABEL, lineno);
			$$->child[0] = $3;
			$$->attr.goto_label = $1;
		}
		| non_label_stmt
		{
			$$ = $1;
		}
		;
non_label_stmt : assign_stmt 
		{
			$$ = $1;
		}
		| proc_stmt 
		{
			$$ = $1;
		}
		| compound_stmt 
		{
			$$ = $1;
		}
		| if_stmt 
		{
			$$ = $1;
		}
		| repeat_stmt 
		{
			$$ = $1;
		}
		| while_stmt
		{
			$$ = $1;
		}
		| for_stmt 
		{
			$$ = $1;
		}
		| case_stmt 
		{
			$$ = $1;
		}
		| goto_stmt
		{
			$$ = $1;
		}
		;
assign_stmt : ID ASSIGN expression
		{
			$$ = newStmtTree(STMT_ASSIGN, lineno);
			$$->attr.name = copyString($1);
			$$->child[0] = $3;
		}
		;
		| ID LB expression RB ASSIGN expression
		{
			$$ = newStmtTree(STMT_ASSIGN, lineno);
			$$->attr.name = copyString($1);
			$$->child[0] = $3;
			$$->child[1] = $6;
		}
		| ID DOT ID ASSIGN expression
		{
			$$ = newStmtTree(STMT_ASSIGN, lineno);
			$$->attr.name = copyString($1);
			$$->child[0] = newExpTree(EXP_ID, lineno);
			$$->child[0]->attr.name = copyString($3);
		}
		;
proc_stmt : ID
		{
			$$ = newExpTree(EXP_ID, lineno);
			$$->attr.name = copyString($1);
		}
		| ID LP args_list RP
		{
			$$ = newExpTree(EXP_ID, lineno);
			$$->child[0] = $3;
			$$->attr.name = copyString($1);
		}
		| SYS_PROC
		{
			$$ = newExpTree(EXP_ID, lineno);
			$$->attr.name = copyString($1);
		}
		| SYS_PROC LP expression_list RP
		{
			$$ = newExpTree(EXP_ID, lineno);
			$$->child[0] = $3;
			$$->attr.name = copyString($1);
		}
		| READ LP factor RP
		{
			$$ = newExpTree(EXP_ID, lineno);
			$$->child[0] = $3;
			$$->attr.name = copyString("read");
		}
		;
if_stmt : IF expression THEN stmt else_clause
		{
			$$ = newStmtTree(STMT_IF, lineno);
			$$->child[0] = $2;
			$$->child[1] = $4;
			$$->child[2] = $5;
		}
		;
else_clause : ELSE stmt 
		{
			$$ = $2;
		}		
		| 
		{
			$$ = NULL;
		}
		;
repeat_stmt : REPEAT stmt_list UNTIL expression
		{
			$$ = newStmtTree(STMT_REPEAT, lineno);
			$$->child[0] = $2;
			$$->child[1] = $4;
		}
		;
while_stmt : WHILE expression DO stmt
		{
			$$ = newStmtTree(STMT_WHILE, lineno);
			$$->child[0] = $2;
			$$->child[1] = $4;
		}		
		;
for_stmt : FOR ID ASSIGN expression direction expression DO stmt
		{
			$$ = newStmtTree(STMT_FOR, lineno);
			$$->attr.name = copyString($2);
			$$->direction = $5;
			$$->child[0] = $4;
			$$->child[1] = $6;
			$$->child[2] = $8;
		}
		;
direction : TO 
		{
			$$ = 1;
		}
		| DOWNTO
		{
			$$ = 0;
		}
		;
case_stmt : CASE expression OF case_expr_list END
		{
			$$ = newStmtTree(STMT_CASE, lineno);
			$$->child[0] = $2;
			$$->child[1] = $4;
		};
case_expr_list : case_expr_list case_expr 
		{
			Tree *t = $1;
			if (t != NULL) {
				for (t; t->sibling; t = t->sibling);
				t->sibling = $2;
				$2->sibling = NULL;
				$$ = $1;
			} else {
				$$ = $2;
			}
		}
		| case_expr
		{
			$$ = $1;
		}
		;
case_expr : const_value COLON stmt SEMI
		{
			$$ = newExpTree(EXP_CASE, lineno);
			$$->child[0] = $1;
			$$->child[1] = $3;
		}
		| ID COLON stmt SEMI
		{
			$$ = newExpTree(EXP_CASE, lineno);
			$$->attr.name = copyString($1);
			$$->child[0] = $3;
		}
		;
goto_stmt : GOTO INTEGER
		{
			$$ = newStmtTree(STMT_GOTO, lineno);
			$$->attr.goto_label = $2;
		}
		;
expression_list : expression_list COMMA expression 
		{
			Tree *t = $1;
				if (t != NULL) {
				for (t; t->sibling; t = t->sibling);
				t->sibling = $3;
				$3->sibling = NULL;
				$$ = $1;
			} else {
				$$ = $3;
			}
		}
		| expression
		{
			$$ = $1;
		}
		;
expression : expression GE expr 
		{
			$$ = newExpTree(EXP_OP, lineno);
			$$->child[0] = $1;
			$$->child[1] = $3;
			$$->attr.op = GE;
		}
		| expression GT expr 
		{
			$$ = newExpTree(EXP_OP, lineno);
			$$->child[0] = $1;
			$$->child[1] = $3;
			$$->attr.op = GT;
		}
		| expression LE expr
		{
			$$ = newExpTree(EXP_OP, lineno);
			$$->child[0] = $1;
			$$->child[1] = $3;
			$$->attr.op = LE;
		}
		| expression LT expr 
		{
			$$ = newExpTree(EXP_OP, lineno);
			$$->child[0] = $1;
			$$->child[1] = $3;
			$$->attr.op = LT;
		}
		| expression EQUAL expr
		{
			$$ = newExpTree(EXP_OP, lineno);
			$$->child[0] = $1;
			$$->child[1] = $3;
			$$->attr.op = EQUAL;
		}
		| expression UNEQUAL expr 
		{
			$$ = newExpTree(EXP_OP, lineno);
			$$->child[0] = $1;
			$$->child[1] = $3;
			$$->attr.op = UNEQUAL;
		}
		| expr
		{
			$$ = $1;
		}
		;
expr : expr PLUS term 
		{
			$$ = newExpTree(EXP_OP, lineno);
			$$->child[0] = $1;
			$$->child[1] = $3;
			$$->attr.op = PLUS;
		}
		| expr MINUS term 
		{
			$$ = newExpTree(EXP_OP, lineno);
			$$->child[0] = $1;
			$$->child[1] = $3;
			$$->attr.op = MINUS;
		}
		| expr OR term 
		{
			$$ = newExpTree(EXP_OP, lineno);
			$$->child[0] = $1;
			$$->child[1] = $3;
			$$->attr.op = OR;
		}
		| term
		{
			$$ = $1;
		}
		;
term : term MUL factor 
		{
			$$ = newExpTree(EXP_OP, lineno);
			$$->child[0] = $1;
			$$->child[1] = $3;
			$$->attr.op = MUL;
		}
		| term DIV factor 
		{
			$$ = newExpTree(EXP_OP, lineno);
			$$->child[0] = $1;
			$$->child[1] = $3;
			$$->attr.op = DIV;
		}
		| term MOD factor
		{
			$$ = newExpTree(EXP_OP, lineno);
			$$->child[0] = $1;
			$$->child[1] = $3;
			$$->attr.op = MOD;
		}
		| term AND factor 
		{
			$$ = newExpTree(EXP_OP, lineno);
			$$->child[0] = $1;
			$$->child[1] = $3;
			$$->attr.op = AND;
		}
		| factor
		{
			$$ = $1;
		}
		;
factor : ID 
		{
			$$ = newExpTree(EXP_ID, lineno);
			$$->attr.name = copyString($1);
		}		
		| ID LP args_list RP 
		{
			$$ = newExpTree(EXP_ID, lineno);
			$$->child[0] = $3;
			$$->attr.name = copyString($1);
		}
		| SYS_FUNCT
		{
			$$ = newExpTree(EXP_ID, lineno);
			$$->attr.name = copyString($1);
		}
		|SYS_FUNCT LP args_list RP 
		{
			$$ = newExpTree(EXP_ID, lineno);
			$$->child[0] = $3;
			$$->attr.name = copyString($1);
		}
		| const_value 
		{
			$$ = $1;
		}
		| LP expression RP
		{
			$$ = $2;
		}
		| NOT factor 
		{
			$$ = $2;
		}
		| MINUS factor 
		{
			$$ = $2;
		}
		| ID LB expression RB
		{
			$$ = newExpTree(EXP_ID, lineno);
			$$->child[0] = $3;
			$$->attr.name = copyString($1);
		}
		| ID DOT ID
		{		
			$$ = newExpTree(EXP_ID, lineno);
			$$->child[0] = newExpTree(EXP_ID, lineno);
			$$->child[0]->attr.name = copyString($3);
		}
		;
args_list : args_list COMMA expression 
		{
			Tree *t = $1;
			if (t != NULL) {
				for (t; t->sibling; t = t->sibling);
				t->sibling = $3;
				$3->sibling = NULL;
				$$ = $1;
			} else {
				$$ = $3;
			}
		}
		| expression
		{
			$$ = $1;
		}
		;

%%

int yyerror(char *s) {
	fprintf(listing, "Syntax error at line %d: %s\n", lineno, s);
	return 0;
}

Tree *parse()
{
	yyparse();
	return rootTree;
}
