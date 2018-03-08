#include "global.h"
#include "utils.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

FILE *source;
FILE *listing;
FILE *code;

int lineno;

extern FILE *yyin;

int main(int argc, char *argv[])
{
	Tree *AST;
	char pgm[MAX_FILE_NAME_LEN];	// Output file name

//	if (argc != 2) {
//		err_sys("Usage: %s <filename>\n", argv[0]);
//	}

//	strncpy(pgm, argv[1], MAX_FILE_NAME_LEN);
//	if (strchr(pgm, '.') == NULL)
//		strncat(pgm, ".a", MAX_FILE_NAME_LEN);

//	source = fopen(pgm, "r");
    // Debug
    source = fopen("/home/thidtc/Desktop/test.pas", "r");
	//source = fopen("sample/as.pas", "r");

	if (source == NULL) {
		err_sys("File %s not found\n", argv[0]);
	}
	
    listing = stdout;

	yyin = source;



	AST = parse();
	
	printTree(AST);

    deleteTree(AST);
	
	fclose(source);
	
	return 0;
}
