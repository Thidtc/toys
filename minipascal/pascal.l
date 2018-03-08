%{
#include "global.h"
#include "pascal.tab.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

extern YYSTYPE yylval;

int echo_source = 1;

#define ECHO_SOURCE if(echo_source) printf("%s\n", yytext);

%}

%option noyywrap

letter	[_a-zA-Z]
alnum	[_a-zA-Z0-9]
dec		[0-9]

%%

"("				{ECHO_SOURCE return LP;}
")"				{ECHO_SOURCE return RP;}
"["				{ECHO_SOURCE return LB;}
"]"				{ECHO_SOURCE return RB;}
";"				{ECHO_SOURCE return SEMI;}
":"				{ECHO_SOURCE return COLON;}
","				{ECHO_SOURCE return COMMA;}
"*"				{ECHO_SOURCE return MUL;}
"/"				{ECHO_SOURCE return DIV;}
"+"				{ECHO_SOURCE return PLUS;}
"-"				{ECHO_SOURCE return MINUS;}
">="			{ECHO_SOURCE return GE;}
">"				{ECHO_SOURCE return GT;}
"<="			{ECHO_SOURCE return LE;}
"<"				{ECHO_SOURCE return LT;}
"="				{ECHO_SOURCE return EQUAL;}
"<>"			{ECHO_SOURCE return UNEQUAL;}
":="			{ECHO_SOURCE return ASSIGN;}
".."			{ECHO_SOURCE return DOTDOT;}
"."				{ECHO_SOURCE return DOT;}
"and"			{ECHO_SOURCE return AND;}
"array"			{ECHO_SOURCE return ARRAY;}
"begin"			{ECHO_SOURCE return BBEGIN;}
"case"			{ECHO_SOURCE return CASE;}
"const"			{ECHO_SOURCE return CONST;}
"do"			{ECHO_SOURCE return DO;}
"downto"		{ECHO_SOURCE return DOWNTO;}
"else"			{ECHO_SOURCE return ELSE;}
"end"			{ECHO_SOURCE return END;}
"for"			{ECHO_SOURCE return FOR;}
"function"		{ECHO_SOURCE return FUNCTION;}
"goto"			{ECHO_SOURCE return GOTO;}
"if"			{ECHO_SOURCE return IF;}
"mod"			{ECHO_SOURCE return MOD;}
"not"			{ECHO_SOURCE return NOT;}
"of"			{ECHO_SOURCE return OF;}
"or"			{ECHO_SOURCE return OR;}
"proc"			{ECHO_SOURCE return PROC;}
"procedure"		{ECHO_SOURCE return PROCEDURE;}
"program"		{ECHO_SOURCE return PROGRAM;}
"read"			{ECHO_SOURCE return READ;}
"record"		{ECHO_SOURCE return RECORD;}
"repeat"		{ECHO_SOURCE return REPEAT;}
"then"			{ECHO_SOURCE return THEN;}
"to"			{ECHO_SOURCE return TO;}
"type"			{ECHO_SOURCE return TYPE;}
"until"			{ECHO_SOURCE return UNTIL;}
"var"			{ECHO_SOURCE return VAR;}
"while"			{ECHO_SOURCE return WHILE;}
"write" |
"writeln"		{ECHO_SOURCE return SYS_PROC;}
"abs" |
"chr" |
"odd" |
"ord" |
"pred" |
"sqr" |
"sqrt" |
"succ"			{ECHO_SOURCE strncpy(yylval.p_s, yytext, MAX_NAME_LEN); return SYS_FUNCT;}

"false" |
"maxint" |
"true"			{ECHO_SOURCE strncpy(yylval.p_s, yytext, MAX_NAME_LEN); return SYS_CON;}

"boolean" |
"char" |
"integer" 		{ECHO_SOURCE strncpy(yylval.p_s, yytext, MAX_NAME_LEN); return SYS_TYPE;}


[1-9]+{dec}*	{ECHO_SOURCE yylval.p_i = atoi(yytext); return INTEGER;}
{dec}+(\.{dec}+)?([E|e][+\-]?{dec}+)?	{ECHO_SOURCE yylval.p_r = atof(yytext); return REAL;}
'([^']|\")'		{ECHO_SOURCE yylval.p_c = yytext[1]; return CHAR;}
\'(\\.|[^\'])(\\.|[^\'])+\'				{ECHO_SOURCE strncpy(yylval.p_s, yytext, MAX_NAME_LEN); return STRING;}

{letter}{alnum}*						{ECHO_SOURCE strncpy(yylval.p_s, yytext, MAX_NAME_LEN); return ID;}

"\n"			{lineno++;}

.				{}

%%


