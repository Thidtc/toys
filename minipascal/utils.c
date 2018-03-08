#include "utils.h"
//#include <malloc.h>
#include <string.h>

extern FILE *listing;

char *copyString(char *s)
{
	int n;
	char *t;
	if (s == NULL)
		return NULL;
	n = strlen(s) + 1;
	t = (char *)malloc(n);
	if (t == NULL) {
		err_sys("malloc error");
	} else {
		strcpy(t, s);
	}
	return t;
}

Tree *newModuleTree(ModuleType type, int lineno)
{
	int i;
	Tree *t = (Tree *)malloc(sizeof(Tree));
	if (t == NULL) {
		err_sys("malloc error");
	}
	for (i = 0; i < MAX_CHILDS; i++) {
		t->child[i] = NULL;
	}
	t->sibling = NULL;
	t->nodetype = NODE_MODULE;
	t->type.module = type;
	t->lineno = lineno;
	
	return t;
}

Tree *newDeclTree(DeclType type, int lineno)
{
	int i;
	Tree *t = (Tree *)malloc(sizeof(Tree));
	if (t == NULL) {
		err_sys("malloc error");
	}
	for (i = 0; i < MAX_CHILDS; i++) {
		t->child[i] = NULL;
	}
	t->sibling = NULL;
	t->nodetype = NODE_DECL;
	t->type.decl = type;
	t->lineno = lineno;
	
	return t;
}

Tree *newStmtTree(StmtType type, int lineno)
{
	int i;
	Tree *t = (Tree *)malloc(sizeof(Tree));
	if (t == NULL) {
		err_sys("malloc error");
	}
	for (i = 0; i < MAX_CHILDS; i++) {
		t->child[i] = NULL;
	}
	t->sibling = NULL;
	t->nodetype = NODE_STMT;
	t->type.stmt = type;
	t->lineno = lineno;
	
	return t;
}

Tree *newExpTree(ExpType type, int lineno)
{
	int i;
	Tree *t = (Tree *)malloc(sizeof(Tree));
	if (t == NULL) {
		err_sys("malloc error");
	}
	for (i = 0; i < MAX_CHILDS; i++) {
		t->child[i] = NULL;
	}
	t->sibling = NULL;
	t->nodetype = NODE_EXP;
	t->type.exp = type;
	t->lineno = lineno;
	
	return t;
}



static indentno = 0;
#define INDENT indentno+=2
#define UNINDENT indentno-=2

static void printSpaces(void)
{ int i;
  for (i=0;i<indentno;i++)
    fprintf(listing," ");
}

// Print the tree
void printTree(Tree *tree)
{
	int i;
	INDENT;
	while (tree) {
		printSpaces();
		switch (tree->nodetype) {
			case NODE_MODULE:
				switch(tree->type.module) {
					case MODULE_PROGRAM:
						fprintf(listing, "program\n");
						break;
					case MODULE_PROGRAMHEAD:
						fprintf(listing, "program name: %s\n", tree->attr.name);
						break;
					case MODULE_ROUTINE:
						fprintf(listing, "routine\n");
						break;
					case MODULE_ROUTINEHEAD:
						fprintf(listing, "routine head\n");
						break;
					case MODULE_PROCEDURE:
						fprintf(listing, "procedure\n");
						break;
					case MODULE_PROCEDUREHEAD:
						fprintf(listing, "procedure name: %s\n", tree->attr.name);
						break;
					case MODULE_FUNCTION:
						fprintf(listing, "function\n");
						break;
					case MODULE_FUNCTIONHEAD:
						fprintf(listing, "function name: %s\n", tree->attr.name);
						break;
					default:
						fprintf(listing, "unknown module\n");
						break;
				}
				break;
			case NODE_DECL:
					switch(tree->type.module) {
					case DECL_ARRAY:
						fprintf(listing, "decl array\n");
						break;
					case DECL_FIELD:
						fprintf(listing, "decl field\n");
						break;
					case DECL_SIMPLE:
						fprintf(listing, "decl simple\n");
						break;
					case DECL_VAR:
						fprintf(listing, "decl var: %s\n", tree->attr.name);
						break;
					case DECL_PARA:
						fprintf(listing, "decl para\n");
						break;
					default:
						fprintf(listing, "unknown decl\n");
						break;
				}
				break;
			case NODE_STMT:
				switch(tree->type.module) {
					case STMT_EQUAL:
						fprintf(listing, "stmt equal: %s\n", tree->attr.name);
						break;
					case STMT_ASSIGN:
						fprintf(listing, "stmt assign: %s\n", tree->attr.name);
						break;
					case STMT_PROC:
						fprintf(listing, "stmt proc\n"); 	// not use
						break;
					case STMT_COMP:
						fprintf(listing, "stmt comp\n");		// not use
						break;
					case STMT_IF:
						fprintf(listing, "stmt if\n");
						break;
					case STMT_REPEAT:
						fprintf(listing, "stmt repeat\n");
						break;
					case STMT_WHILE:
						fprintf(listing, "stmt while\n");
						break;
					case STMT_LABEL:
						fprintf(listing, "stmt label: %d\n", tree->attr.goto_label);
						break;
					case STMT_FOR:
						fprintf(listing, "stmt for: %s\n", tree->attr.name);
						break;
					case STMT_CASE:
						fprintf(listing, "stmt case\n");
						break;
					case STMT_GOTO:
						fprintf(listing, "stmt goto: %d\n", tree->attr.goto_label);
						break;
					default:
						fprintf(listing, "unknown stmt\n");
						break;
				}
				break;
			case NODE_EXP:
				switch(tree->type.module) {
					case EXP_OP:
						fprintf(listing, "exp op: %d\n", tree->attr.op);
						break;
					case EXP_CONST_DEFINE:
						fprintf(listing, "exp const define: %s\n", tree->attr.string_val);
						break;
					case EXP_CONST_INT:
						fprintf(listing, "exp const int: %d\n", tree->attr.int_val);
						break;
					case EXP_CONST_CHAR:
						fprintf(listing, "exp const char: %c\n", tree->attr.char_val);
						break;
					case EXP_CONST_STR:
						fprintf(listing, "exp const string: %s\n", tree->attr.string_val);
						break;
					case EXP_CONST_REAL:
						fprintf(listing, "exp const real: %lf\n", tree->attr.real_val);
						break;
					case EXP_ID:
						fprintf(listing, "exp id: %s\n", tree->attr.name);
						break;
					case EXP_CASE:
						fprintf(listing, "exp case\n");
						break;
					case EXP_FACTOR:
						fprintf(listing, "exp factor\n"); 	// not use
						break;
					default:
						fprintf(listing, "unknown exp\n");
						break;
				}
				break;
			default:
				break;
		}
		for (i = 0; i < MAX_CHILDS; i++) {
			printTree(tree->child[i]);
		}
		tree = tree->sibling;
	}
	UNINDENT;
}

void deleteTree(Tree *tree)
{
	int i;
	Tree *sib;
	while (tree) {
		printSpaces();
		switch (tree->nodetype) {
			case NODE_MODULE:
				switch(tree->type.module) {
					case MODULE_PROGRAM:
						break;
					case MODULE_PROGRAMHEAD:
                        if (tree->attr.name)
                            free(tree->attr.name);
						break;
					case MODULE_ROUTINE:
						break;
					case MODULE_ROUTINEHEAD:
						break;
					case MODULE_PROCEDURE:
						break;
					case MODULE_PROCEDUREHEAD:
                        if (tree->attr.name)
                            free(tree->attr.name);
						break;
					case MODULE_FUNCTION:
						break;
					case MODULE_FUNCTIONHEAD:
                        if (tree->attr.name)
                            free(tree->attr.name);
						break;
					default:
						break;
				}
				break;
			case NODE_DECL:
					switch(tree->type.module) {
					case DECL_ARRAY:
						break;
					case DECL_FIELD:
						break;
					case DECL_SIMPLE:
						break;
					case DECL_VAR:
						break;
					case DECL_PARA:
						break;
					default:
						break;
				}
				break;
			case NODE_STMT:
				switch(tree->type.module) {
					case STMT_EQUAL:
                        if (tree->attr.name)
                            free(tree->attr.name);
						break;
					case STMT_ASSIGN:
                        if (tree->attr.name)
                            free(tree->attr.name);
						break;
					case STMT_PROC: 	// not use
						break;
					case STMT_COMP:		// not use
						break;
					case STMT_IF:
						break;
					case STMT_REPEAT:
						break;
					case STMT_WHILE:
						break;
					case STMT_LABEL:
						break;
					case STMT_FOR:
                        if (tree->attr.name)
                            free(tree->attr.name);
						break;
					case STMT_CASE:
						break;
					case STMT_GOTO:
						break;
					default:
						break;
				}
				break;
			case NODE_EXP:
				switch(tree->type.module) {
					case EXP_OP:
						break;
					case EXP_CONST_DEFINE:
                        if (tree->attr.name)
                            free(tree->attr.string_val);
						break;
					case EXP_CONST_INT:
						break;
					case EXP_CONST_CHAR:
						break;
					case EXP_CONST_STR:
                        if (tree->attr.name)
                            free(tree->attr.string_val);
						break;
					case EXP_CONST_REAL:
						break;
					case EXP_ID:
                        if (tree->attr.name)
                            free(tree->attr.name);
						break;
					case EXP_CASE:
						break;
					case EXP_FACTOR:
						break;
					default:
						break;
				}
				break;
			default:
				break;
		}
		for (i = 0; i < MAX_CHILDS; i++) {
			deleteTree(tree->child[i]);
		}
		sib = tree->sibling;
        free(tree);
		tree = sib;
	}
}
