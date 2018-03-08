/* A Bison parser, made by GNU Bison 3.0.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2013 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_YY_PASCAL_TAB_H_INCLUDED
# define YY_YY_PASCAL_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    LP = 258,
    RP = 259,
    LB = 260,
    RB = 261,
    SEMI = 262,
    COLON = 263,
    COMMA = 264,
    MUL = 265,
    DIV = 266,
    PLUS = 267,
    MINUS = 268,
    GE = 269,
    GT = 270,
    LE = 271,
    LT = 272,
    EQUAL = 273,
    UNEQUAL = 274,
    ASSIGN = 275,
    DOTDOT = 276,
    DOT = 277,
    AND = 278,
    ARRAY = 279,
    BBEGIN = 280,
    CASE = 281,
    CONST = 282,
    DO = 283,
    DOWNTO = 284,
    ELSE = 285,
    END = 286,
    FOR = 287,
    FUNCTION = 288,
    GOTO = 289,
    IF = 290,
    MOD = 291,
    NOT = 292,
    OF = 293,
    OR = 294,
    PROC = 295,
    PROCEDURE = 296,
    PROGRAM = 297,
    READ = 298,
    RECORD = 299,
    REPEAT = 300,
    THEN = 301,
    TO = 302,
    TYPE = 303,
    UNTIL = 304,
    VAR = 305,
    WHILE = 306,
    SYS_PROC = 307,
    SYS_FUNCT = 308,
    SYS_CON = 309,
    SYS_TYPE = 310,
    INTEGER = 311,
    REAL = 312,
    CHAR = 313,
    STRING = 314,
    ID = 315
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE YYSTYPE;
union YYSTYPE
{
#line 13 "pascal.y" /* yacc.c:1909  */

	char p_s[MAX_NAME_LEN];
	char p_c;
	int p_i;
	double p_r;
	Tree *p_tree;

#line 123 "pascal.tab.h" /* yacc.c:1909  */
};
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_PASCAL_TAB_H_INCLUDED  */
