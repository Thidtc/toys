#ifndef _GLOBAL_H__
#define _GLOBAL_H__

//#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>



#define MAX_NAME_LEN 256

#define MAX_FILE_NAME_LEN 256

//#define EOF 0

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 0
#endif

extern FILE *source;
extern FILE *listing;
extern FILE *code;

extern int lineno;


#define MAX_CHILDS 5

extern FILE *source;
extern FILE *obeject;

typedef int TokenType;

typedef enum {
	NODE_MODULE,
	NODE_DECL,
	NODE_STMT,
	NODE_EXP,
} NodeType;

typedef enum {
	MODULE_PROGRAM,
	MODULE_PROGRAMHEAD,
	MODULE_ROUTINE,
	MODULE_ROUTINEHEAD,
	MODULE_PROCEDURE,
	MODULE_PROCEDUREHEAD,
	MODULE_FUNCTION,
	MODULE_FUNCTIONHEAD,
} ModuleType;

typedef enum {
	DECL_ARRAY,
	DECL_FIELD,
	DECL_SIMPLE,
	DECL_VAR,
	DECL_PARA,
} DeclType;

typedef enum {
	STMT_EQUAL,
	STMT_ASSIGN,
	STMT_PROC,
	STMT_COMP,
	STMT_IF,
	STMT_REPEAT,
	STMT_WHILE,
	STMT_LABEL,
	STMT_FOR,
	STMT_CASE,
	STMT_GOTO,
} StmtType;

typedef enum {
	EXP_OP,
	EXP_CONST_DEFINE,
	EXP_CONST_INT,
	EXP_CONST_CHAR,
	EXP_CONST_STR,
	EXP_CONST_REAL,
	EXP_ID,
	EXP_CASE,
	EXP_FACTOR,
} ExpType;

typedef enum {
	TYPE_VOID,
	TYPE_INT,
	TYPE_BOOL,
	TYPE_CHAR,
	TYPE_REAL,
	TYPE_STRING,
} ExpTType;

typedef struct tag_tree
{ 
	struct tag_tree *child[MAX_CHILDS];
	struct tag_tree *sibling;
	int lineno;
	NodeType nodetype;
	union {
	    ModuleType module; 
	    StmtType stmt; 
	    ExpType exp;
	    DeclType decl;
	} type;
 	union { 
 		TokenType op;
		int goto_label;
		int int_val;
		char char_val;
		char * string_val;
		double real_val;
		char * name; 
	} attr;
	
	int direction;//1-to,0-downto
	//ExpTType type; /* for type checking of exps */
} Tree;

#ifndef YYPARSER
#include "pascal.tab.h"
#endif


#endif
