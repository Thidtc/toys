#ifndef _UTILS_H__
#define _UTILS_H__

#include "global.h"

Tree *parse();

char *copyString();

Tree *newModuleTree(ModuleType, int);

Tree *newDeclTree(DeclType, int);

Tree *newStmtTree(StmtType, int);

Tree *newExpTree(ExpType, int);

void deleteTree(Tree *);


// err process
void err_ret(const char *, ...);

void err_sys(const char *, ...);

void err_dump(const char *, ...);

void err_msg(const char *, ...);

void err_quit(const char *, ...);


static void printSpaces(void);

void printTree(Tree *);
#endif
