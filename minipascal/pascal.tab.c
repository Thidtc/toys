/* A Bison parser, made by GNU Bison 3.0.2.  */

/* Bison implementation for Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.0.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
#line 1 "pascal.y" /* yacc.c:339  */

#define YYPARSER

#include "global.h"
#include "utils.h"

char *savedName;
int saveLineNo;

static Tree *rootTree;

#line 78 "pascal.tab.c" /* yacc.c:339  */

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* In a future release of Bison, this section will be replaced
   by #include "pascal.tab.h".  */
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
#line 13 "pascal.y" /* yacc.c:355  */

	char p_s[MAX_NAME_LEN];
	char p_c;
	int p_i;
	double p_r;
	Tree *p_tree;

#line 187 "pascal.tab.c" /* yacc.c:355  */
};
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_PASCAL_TAB_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 202 "pascal.tab.c" /* yacc.c:358  */

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif


#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  5
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   338

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  61
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  56
/* YYNRULES -- Number of rules.  */
#define YYNRULES  127
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  261

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   315

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   138,   138,   146,   152,   159,   166,   177,   181,   186,
     190,   203,   210,   215,   220,   225,   230,   236,   241,   245,
     257,   262,   269,   273,   277,   282,   287,   292,   296,   302,
     308,   314,   322,   328,   332,   344,   349,   355,   368,   374,
     379,   383,   395,   400,   407,   419,   431,   435,   440,   444,
     450,   458,   465,   472,   477,   481,   493,   498,   504,   511,
     516,   521,   526,   531,   544,   548,   554,   559,   563,   567,
     571,   575,   579,   583,   587,   591,   596,   603,   610,   618,
     623,   629,   634,   640,   647,   655,   660,   664,   671,   678,
     688,   692,   697,   703,   715,   720,   726,   733,   739,   751,
     756,   763,   770,   777,   784,   791,   798,   803,   810,   817,
     824,   829,   836,   843,   850,   857,   862,   867,   873,   878,
     884,   888,   892,   896,   900,   906,   913,   925
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "LP", "RP", "LB", "RB", "SEMI", "COLON",
  "COMMA", "MUL", "DIV", "PLUS", "MINUS", "GE", "GT", "LE", "LT", "EQUAL",
  "UNEQUAL", "ASSIGN", "DOTDOT", "DOT", "AND", "ARRAY", "BBEGIN", "CASE",
  "CONST", "DO", "DOWNTO", "ELSE", "END", "FOR", "FUNCTION", "GOTO", "IF",
  "MOD", "NOT", "OF", "OR", "PROC", "PROCEDURE", "PROGRAM", "READ",
  "RECORD", "REPEAT", "THEN", "TO", "TYPE", "UNTIL", "VAR", "WHILE",
  "SYS_PROC", "SYS_FUNCT", "SYS_CON", "SYS_TYPE", "INTEGER", "REAL",
  "CHAR", "STRING", "ID", "$accept", "program", "program_head", "routine",
  "sub_routine", "routine_head", "label_part", "const_part",
  "const_expr_list", "const_value", "type_part", "type_decl_list",
  "type_definition", "type_decl", "simple_type_decl", "array_type_decl",
  "record_type_decl", "field_decl_list", "field_decl", "name_list",
  "var_part", "var_decl_list", "var_decl", "routine_part", "function_decl",
  "function_head", "procedure_decl", "procedure_head", "parameters",
  "para_decl_list", "para_type_list", "var_para_list", "val_para_list",
  "routine_body", "compound_stmt", "stmt_list", "stmt", "non_label_stmt",
  "assign_stmt", "proc_stmt", "if_stmt", "else_clause", "repeat_stmt",
  "while_stmt", "for_stmt", "direction", "case_stmt", "case_expr_list",
  "case_expr", "goto_stmt", "expression_list", "expression", "expr",
  "term", "factor", "args_list", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315
};
# endif

#define YYPACT_NINF -175

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-175)))

#define YYTABLE_NINF -1

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     -28,   -31,    25,  -175,    28,  -175,    36,    35,    55,  -175,
    -175,  -175,  -175,  -175,    34,    37,   -13,    78,    49,    68,
      71,   126,  -175,    74,    82,   126,   121,  -175,   126,   137,
     135,    61,  -175,   138,  -175,  -175,  -175,  -175,  -175,  -175,
    -175,  -175,  -175,   276,   133,   141,    68,  -175,   110,    -7,
     126,   126,   126,   168,  -175,  -175,  -175,  -175,  -175,    43,
    -175,   243,    11,    17,  -175,   152,  -175,   150,   126,   101,
     261,   126,   188,   126,   126,   126,   113,  -175,   169,   276,
      56,  -175,  -175,    33,   110,  -175,   115,   117,    -7,  -175,
     180,  -175,   182,   250,  -175,  -175,   126,   126,   126,   131,
     126,   126,   126,   126,   126,   126,   257,   126,   126,   126,
     126,   126,   126,   126,   126,   174,   177,   126,   174,    45,
     304,  -175,   304,    63,   284,   304,   173,  -175,   187,   110,
     276,   196,   110,  -175,   181,   186,   191,  -175,  -175,  -175,
      56,   143,  -175,   207,   207,  -175,  -175,  -175,  -175,  -175,
     114,   116,   291,  -175,    11,    11,    11,    11,    11,    11,
     204,   208,   193,  -175,    17,    17,    17,  -175,  -175,  -175,
    -175,   227,   185,  -175,   304,  -175,  -175,   126,  -175,   126,
     198,   126,  -175,   128,   200,    48,   -16,  -175,   122,   167,
     276,  -175,   221,  -175,   -40,   228,  -175,   222,    35,   225,
    -175,  -175,  -175,   174,   174,  -175,  -175,  -175,  -175,   126,
     174,  -175,   304,   304,   126,   304,  -175,    32,   229,  -175,
    -175,    56,  -175,  -175,  -175,   110,   246,    64,  -175,   230,
     255,    48,  -175,  -175,  -175,   263,   264,   268,  -175,   304,
     276,  -175,   199,   265,   246,  -175,   -40,    48,    48,  -175,
    -175,  -175,   174,  -175,    56,  -175,  -175,  -175,  -175,  -175,
    -175
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,     7,     0,     1,     0,     0,     9,     3,
       2,    64,     4,    61,     0,    18,     0,     0,     8,     0,
      40,     0,    62,     0,     0,     0,     0,    64,     0,    81,
       0,    79,    69,     0,    66,    67,    68,    70,    71,    72,
      73,    74,    75,     0,     0,     0,    17,    20,     0,    48,
       0,     0,     0,   118,    16,    12,    13,    14,    15,   116,
     120,     0,   106,   110,   115,     0,    97,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    63,     0,     0,
       0,    19,    38,     0,    39,    42,     0,     0,     6,    46,
       0,    47,     0,     0,   123,   122,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      99,    65,   127,     0,     0,    76,     0,    11,     0,     0,
       0,     0,     0,    25,    26,     0,     0,    22,    23,    24,
       0,     0,    41,    54,    54,    44,    45,     7,     7,   121,
       0,     0,     0,   125,   100,   101,   102,   103,   104,   105,
       0,     0,     0,    94,   107,   108,   109,   111,   112,   114,
     113,     0,    86,    83,    87,    88,    82,     0,    80,     0,
       0,     0,    10,     0,     0,     0,     0,    35,     0,     0,
       0,    21,     0,    37,     0,     0,    52,     0,     0,     0,
     119,   117,   124,     0,     0,    92,    93,    91,    90,     0,
       0,    84,    98,   126,     0,    78,    27,     0,     0,    33,
      34,     0,    31,    28,    43,     0,    60,     0,    56,     0,
       0,     0,    49,     5,    51,     0,     0,     0,    85,    77,
       0,    29,     0,     0,    59,    53,     0,     0,     0,    50,
      96,    95,     0,    30,     0,    36,    55,    57,    58,    89,
      32
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -175,  -175,  -175,  -175,   125,   285,  -175,  -175,  -175,   -43,
    -175,  -175,   245,  -137,  -174,  -175,  -175,  -175,   106,  -124,
    -175,  -175,   209,  -175,   206,  -175,   216,  -175,   151,  -175,
      66,  -175,  -175,   139,    -6,   309,  -111,   259,  -175,  -175,
    -175,  -175,  -175,  -175,  -175,  -175,  -175,  -175,   176,  -175,
    -175,   -19,   224,    47,   -35,    52
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     2,     3,     6,   197,   198,     8,    15,    18,    60,
      20,    46,    47,   136,   137,   138,   139,   186,   187,    83,
      49,    84,    85,    88,    89,    90,    91,    92,   195,   227,
     228,   229,   230,    12,    32,    16,    33,    34,    35,    36,
      37,   211,    38,    39,    40,   209,    41,   162,   163,    42,
     119,   122,    62,    63,    64,   123
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint16 yytable[] =
{
      78,    13,    61,   192,   172,   183,    67,   175,   188,    70,
     225,   218,    11,    21,     1,   219,    94,    95,    22,    23,
      82,    24,    25,   107,   108,     5,    86,   110,   111,     4,
      26,    93,    27,   116,    87,     9,   128,   135,    28,    29,
     112,   140,   141,    30,    82,   240,    97,    31,    98,   176,
     109,   129,   120,   113,   177,   124,   125,   249,    10,   129,
      11,   130,   188,   161,    73,    99,    74,   178,   245,   130,
     226,   246,   179,   257,   258,   167,   168,   169,   170,   152,
     131,    75,    14,    76,   243,    19,    54,   184,    55,    56,
      57,    58,   235,   236,    17,   171,    43,   135,   174,   238,
     132,   244,    54,   133,    55,    56,    57,    58,   134,    44,
      54,   133,    55,    56,    57,    58,   134,   260,   200,   161,
     201,    48,   226,   179,    68,   179,    11,    21,    45,    50,
     221,   141,   216,    23,    65,    24,    25,   141,    66,    51,
      71,   259,   135,    72,    26,    77,    27,   223,   150,   151,
     117,    79,    28,    29,   164,   165,   166,    30,   212,    80,
     213,    31,   215,    52,   100,   101,   102,   103,   104,   105,
      82,    96,   114,   126,   241,   143,   127,   144,   135,    53,
      54,   173,    55,    56,    57,    58,    59,   147,   135,   148,
     237,   153,    13,   181,   182,   239,   115,   253,   191,    11,
      21,   185,   189,   193,   135,   135,    23,   190,    24,    25,
     194,   135,   203,    11,    21,   210,   204,    26,   214,    27,
      23,   217,    24,    25,   205,    28,    29,   222,   224,   232,
      30,    26,   234,    27,    31,   242,   231,   254,   247,    28,
      29,   100,   101,   102,   103,   104,   105,    54,    31,    55,
      56,    57,    58,   160,   149,   141,   207,   100,   101,   102,
     103,   104,   105,   248,   100,   101,   102,   103,   104,   105,
     250,   251,   255,   199,   208,   100,   101,   102,   103,   104,
     105,   106,   100,   101,   102,   103,   104,   105,     7,   118,
     180,    81,   220,   142,   145,   196,   252,   202,   100,   101,
     102,   103,   104,   105,   146,   100,   101,   102,   103,   104,
     105,    54,   256,    55,    56,    57,    58,   160,   100,   101,
     102,   103,   104,   105,   154,   155,   156,   157,   158,   159,
      54,   121,    55,    56,    57,    58,    69,   233,   206
};

static const yytype_uint8 yycheck[] =
{
      43,     7,    21,   140,   115,   129,    25,   118,   132,    28,
      50,   185,    25,    26,    42,    31,    51,    52,    31,    32,
      60,    34,    35,    12,    13,     0,    33,    10,    11,    60,
      43,    50,    45,    68,    41,     7,    79,    80,    51,    52,
      23,     8,     9,    56,    60,    13,     3,    60,     5,     4,
      39,     3,    71,    36,     9,    74,    75,   231,    22,     3,
      25,    13,   186,   106,     3,    22,     5,     4,     4,    13,
     194,     7,     9,   247,   248,   110,   111,   112,   113,    98,
      24,    20,    27,    22,   221,    48,    54,   130,    56,    57,
      58,    59,   203,   204,    60,   114,    18,   140,   117,   210,
      44,   225,    54,    55,    56,    57,    58,    59,    60,    60,
      54,    55,    56,    57,    58,    59,    60,   254,     4,   162,
       4,    50,   246,     9,     3,     9,    25,    26,    60,     3,
       8,     9,     4,    32,    60,    34,    35,     9,    56,    13,
       3,   252,   185,     8,    43,     7,    45,   190,    96,    97,
      49,    18,    51,    52,   107,   108,   109,    56,   177,    18,
     179,    60,   181,    37,    14,    15,    16,    17,    18,    19,
      60,     3,    20,    60,   217,    60,     7,    60,   221,    53,
      54,     4,    56,    57,    58,    59,    60,     7,   231,     7,
     209,    60,   198,    20,     7,   214,    46,   240,     7,    25,
      26,     5,    21,    60,   247,   248,    32,    21,    34,    35,
       3,   254,     8,    25,    26,    30,     8,    43,    20,    45,
      32,    21,    34,    35,    31,    51,    52,    60,     7,     7,
      56,    43,     7,    45,    60,     6,     8,    38,     8,    51,
      52,    14,    15,    16,    17,    18,    19,    54,    60,    56,
      57,    58,    59,    60,     4,     9,    29,    14,    15,    16,
      17,    18,    19,     8,    14,    15,    16,    17,    18,    19,
       7,     7,     7,   148,    47,    14,    15,    16,    17,    18,
      19,    38,    14,    15,    16,    17,    18,    19,     3,    28,
       6,    46,   186,    84,    88,   144,    28,     6,    14,    15,
      16,    17,    18,    19,    88,    14,    15,    16,    17,    18,
      19,    54,   246,    56,    57,    58,    59,    60,    14,    15,
      16,    17,    18,    19,   100,   101,   102,   103,   104,   105,
      54,    72,    56,    57,    58,    59,    27,   198,   162
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    42,    62,    63,    60,     0,    64,    66,    67,     7,
      22,    25,    94,    95,    27,    68,    96,    60,    69,    48,
      71,    26,    31,    32,    34,    35,    43,    45,    51,    52,
      56,    60,    95,    97,    98,    99,   100,   101,   103,   104,
     105,   107,   110,    18,    60,    60,    72,    73,    50,    81,
       3,    13,    37,    53,    54,    56,    57,    58,    59,    60,
      70,   112,   113,   114,   115,    60,    56,   112,     3,    96,
     112,     3,     8,     3,     5,    20,    22,     7,    70,    18,
      18,    73,    60,    80,    82,    83,    33,    41,    84,    85,
      86,    87,    88,   112,   115,   115,     3,     3,     5,    22,
      14,    15,    16,    17,    18,    19,    38,    12,    13,    39,
      10,    11,    23,    36,    20,    46,   115,    49,    28,   111,
     112,    98,   112,   116,   112,   112,    60,     7,    70,     3,
      13,    24,    44,    55,    60,    70,    74,    75,    76,    77,
       8,     9,    83,    60,    60,    85,    87,     7,     7,     4,
     116,   116,   112,    60,   113,   113,   113,   113,   113,   113,
      60,    70,   108,   109,   114,   114,   114,   115,   115,   115,
     115,   112,    97,     4,   112,    97,     4,     9,     4,     9,
       6,    20,     7,    80,    70,     5,    78,    79,    80,    21,
      21,     7,    74,    60,     3,    89,    89,    65,    66,    65,
       4,     4,     6,     8,     8,    31,   109,    29,    47,   106,
      30,   102,   112,   112,    20,   112,     4,    21,    75,    31,
      79,     8,    60,    70,     7,    50,    80,    90,    91,    92,
      93,     8,     7,    94,     7,    97,    97,   112,    97,   112,
      13,    70,     6,    74,    80,     4,     7,     8,     8,    75,
       7,     7,    28,    70,    38,     7,    91,    75,    75,    97,
      74
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    61,    62,    63,    64,    65,    66,    67,    68,    68,
      69,    69,    70,    70,    70,    70,    70,    71,    71,    72,
      72,    73,    74,    74,    74,    75,    75,    75,    75,    75,
      75,    75,    76,    77,    78,    78,    79,    80,    80,    81,
      81,    82,    82,    83,    84,    84,    84,    84,    84,    85,
      86,    87,    88,    89,    89,    90,    90,    91,    91,    92,
      93,    94,    95,    96,    96,    97,    97,    98,    98,    98,
      98,    98,    98,    98,    98,    98,    99,    99,    99,   100,
     100,   100,   100,   100,   101,   102,   102,   103,   104,   105,
     106,   106,   107,   108,   108,   109,   109,   110,   111,   111,
     112,   112,   112,   112,   112,   112,   112,   113,   113,   113,
     113,   114,   114,   114,   114,   114,   115,   115,   115,   115,
     115,   115,   115,   115,   115,   115,   116,   116
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     3,     3,     2,     2,     5,     0,     2,     0,
       5,     4,     1,     1,     1,     1,     1,     2,     0,     2,
       1,     4,     1,     1,     1,     1,     1,     3,     3,     4,
       5,     3,     6,     3,     2,     1,     4,     3,     1,     2,
       0,     2,     1,     4,     2,     2,     1,     1,     0,     4,
       5,     4,     3,     3,     0,     3,     1,     3,     3,     2,
       1,     1,     3,     3,     0,     3,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     3,     6,     5,     1,
       4,     1,     4,     4,     5,     2,     0,     4,     4,     8,
       1,     1,     5,     2,     1,     4,     4,     2,     3,     1,
       3,     3,     3,     3,     3,     3,     1,     3,     3,     3,
       1,     3,     3,     3,     3,     1,     1,     4,     1,     4,
       1,     3,     2,     2,     4,     3,     3,     1
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
do                                                              \
  if (yychar == YYEMPTY)                                        \
    {                                                           \
      yychar = (Token);                                         \
      yylval = (Value);                                         \
      YYPOPSTACK (yylen);                                       \
      yystate = *yyssp;                                         \
      goto yybackup;                                            \
    }                                                           \
  else                                                          \
    {                                                           \
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;                                                  \
    }                                                           \
while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  unsigned long int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &(yyvsp[(yyi + 1) - (yynrhs)])
                                              );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
yystrlen (const char *yystr)
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            /* Fall through.  */
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        YYSTYPE *yyvs1 = yyvs;
        yytype_int16 *yyss1 = yyss;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * sizeof (*yyssp),
                    &yyvs1, yysize * sizeof (*yyvsp),
                    &yystacksize);

        yyss = yyss1;
        yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yytype_int16 *yyss1 = yyss;
        union yyalloc *yyptr =
          (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
                  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 139 "pascal.y" /* yacc.c:1646  */
    {
			(yyval.p_tree) = newModuleTree(MODULE_PROGRAM, lineno);
			(yyval.p_tree)->child[0] = (yyvsp[-2].p_tree);
			(yyval.p_tree)->child[1] = (yyvsp[-1].p_tree);
			rootTree = (yyval.p_tree);
		}
#line 1492 "pascal.tab.c" /* yacc.c:1646  */
    break;

  case 3:
#line 147 "pascal.y" /* yacc.c:1646  */
    {
			(yyval.p_tree) = newModuleTree(MODULE_PROGRAMHEAD, lineno);
			(yyval.p_tree)->attr.name = copyString((yyvsp[-1].p_s));
		}
#line 1501 "pascal.tab.c" /* yacc.c:1646  */
    break;

  case 4:
#line 153 "pascal.y" /* yacc.c:1646  */
    {
			(yyval.p_tree) = newModuleTree(MODULE_ROUTINE, lineno);
			(yyval.p_tree)->child[0] = (yyvsp[-1].p_tree);
			(yyval.p_tree)->child[1] = (yyvsp[0].p_tree);
		}
#line 1511 "pascal.tab.c" /* yacc.c:1646  */
    break;

  case 5:
#line 160 "pascal.y" /* yacc.c:1646  */
    {
			(yyval.p_tree) = newModuleTree(MODULE_ROUTINE, lineno);
			(yyval.p_tree)->child[0] = (yyvsp[-1].p_tree);
			(yyval.p_tree)->child[1] = (yyvsp[0].p_tree);
		}
#line 1521 "pascal.tab.c" /* yacc.c:1646  */
    break;

  case 6:
#line 167 "pascal.y" /* yacc.c:1646  */
    {
			(yyval.p_tree) = newModuleTree(MODULE_ROUTINEHEAD, lineno);
			(yyval.p_tree)->child[0] = (yyvsp[-4].p_tree);
			(yyval.p_tree)->child[1] = (yyvsp[-3].p_tree);
			(yyval.p_tree)->child[2] = (yyvsp[-2].p_tree);
			(yyval.p_tree)->child[3] = (yyvsp[-1].p_tree);
			(yyval.p_tree)->child[4] = (yyvsp[0].p_tree);
		}
#line 1534 "pascal.tab.c" /* yacc.c:1646  */
    break;

  case 7:
#line 177 "pascal.y" /* yacc.c:1646  */
    {
			(yyval.p_tree) = NULL;
		}
#line 1542 "pascal.tab.c" /* yacc.c:1646  */
    break;

  case 8:
#line 182 "pascal.y" /* yacc.c:1646  */
    {
			(yyval.p_tree) = (yyvsp[0].p_tree);
		}
#line 1550 "pascal.tab.c" /* yacc.c:1646  */
    break;

  case 9:
#line 186 "pascal.y" /* yacc.c:1646  */
    {
			(yyval.p_tree) = NULL;
		}
#line 1558 "pascal.tab.c" /* yacc.c:1646  */
    break;

  case 10:
#line 191 "pascal.y" /* yacc.c:1646  */
    {
			Tree *t = (yyvsp[-4].p_tree);
			if (t != NULL) {
				for (t; t->sibling; t = t->sibling);
				t->sibling = newStmtTree(STMT_EQUAL, lineno);
				t->sibling->child[0] = (yyvsp[-1].p_tree);
				t->sibling->attr.name = copyString((yyvsp[-3].p_s));
				(yyval.p_tree) = (yyvsp[-4].p_tree);
			} else {
				(yyval.p_tree) = (yyvsp[-4].p_tree);
			}
		}
#line 1575 "pascal.tab.c" /* yacc.c:1646  */
    break;

  case 11:
#line 204 "pascal.y" /* yacc.c:1646  */
    {
			(yyval.p_tree) = newStmtTree(STMT_EQUAL, lineno);
			(yyval.p_tree)->child[0] = (yyvsp[-1].p_tree);
			(yyval.p_tree)->attr.name = copyString((yyvsp[-3].p_s));
		}
#line 1585 "pascal.tab.c" /* yacc.c:1646  */
    break;

  case 12:
#line 211 "pascal.y" /* yacc.c:1646  */
    {
			(yyval.p_tree) = newExpTree(EXP_CONST_INT, lineno);
			(yyval.p_tree)->attr.int_val = (yyvsp[0].p_i);
		}
#line 1594 "pascal.tab.c" /* yacc.c:1646  */
    break;

  case 13:
#line 216 "pascal.y" /* yacc.c:1646  */
    {
			(yyval.p_tree) = newExpTree(EXP_CONST_REAL, lineno);
			(yyval.p_tree)->attr.real_val = (yyvsp[0].p_r);
		}
#line 1603 "pascal.tab.c" /* yacc.c:1646  */
    break;

  case 14:
#line 221 "pascal.y" /* yacc.c:1646  */
    {
			(yyval.p_tree) = newExpTree(EXP_CONST_CHAR, lineno);
			(yyval.p_tree)->attr.char_val = (yyvsp[0].p_c);
		}
#line 1612 "pascal.tab.c" /* yacc.c:1646  */
    break;

  case 15:
#line 226 "pascal.y" /* yacc.c:1646  */
    {
			(yyval.p_tree) = newExpTree(EXP_CONST_STR, lineno);
			(yyval.p_tree)->attr.string_val = copyString((yyvsp[0].p_s));
		}
#line 1621 "pascal.tab.c" /* yacc.c:1646  */
    break;

  case 16:
#line 231 "pascal.y" /* yacc.c:1646  */
    {
			(yyval.p_tree) = newExpTree(EXP_CONST_DEFINE, lineno);
			(yyval.p_tree)->attr.string_val = copyString((yyvsp[0].p_s));
		}
#line 1630 "pascal.tab.c" /* yacc.c:1646  */
    break;

  case 17:
#line 237 "pascal.y" /* yacc.c:1646  */
    {
			(yyval.p_tree) = (yyvsp[0].p_tree);
		}
#line 1638 "pascal.tab.c" /* yacc.c:1646  */
    break;

  case 18:
#line 241 "pascal.y" /* yacc.c:1646  */
    {
			(yyval.p_tree) = NULL;
		}
#line 1646 "pascal.tab.c" /* yacc.c:1646  */
    break;

  case 19:
#line 246 "pascal.y" /* yacc.c:1646  */
    {
			Tree *t = (yyvsp[-1].p_tree);
			if (t != NULL) {
				for (t; t->sibling; t = t->sibling);
				t->sibling = (yyvsp[0].p_tree);
				(yyvsp[0].p_tree)->sibling = NULL;
				(yyval.p_tree) = (yyvsp[-1].p_tree);
			} else {
				(yyval.p_tree) = (yyvsp[0].p_tree);
			}
		}
#line 1662 "pascal.tab.c" /* yacc.c:1646  */
    break;

  case 20:
#line 258 "pascal.y" /* yacc.c:1646  */
    {
			(yyval.p_tree) = (yyvsp[0].p_tree);
		}
#line 1670 "pascal.tab.c" /* yacc.c:1646  */
    break;

  case 21:
#line 263 "pascal.y" /* yacc.c:1646  */
    {
			(yyval.p_tree) = newStmtTree(STMT_EQUAL, lineno);
			(yyval.p_tree)->child[0] = (yyvsp[-1].p_tree);
			(yyval.p_tree)->attr.name = copyString((yyvsp[-3].p_s));
		}
#line 1680 "pascal.tab.c" /* yacc.c:1646  */
    break;

  case 22:
#line 270 "pascal.y" /* yacc.c:1646  */
    {
			(yyval.p_tree) = (yyvsp[0].p_tree);
		}
#line 1688 "pascal.tab.c" /* yacc.c:1646  */
    break;

  case 23:
#line 274 "pascal.y" /* yacc.c:1646  */
    {
			(yyval.p_tree) = (yyvsp[0].p_tree);
		}
#line 1696 "pascal.tab.c" /* yacc.c:1646  */
    break;

  case 24:
#line 278 "pascal.y" /* yacc.c:1646  */
    {
			(yyval.p_tree) = (yyvsp[0].p_tree);
		}
#line 1704 "pascal.tab.c" /* yacc.c:1646  */
    break;

  case 25:
#line 283 "pascal.y" /* yacc.c:1646  */
    {
			(yyval.p_tree) = newDeclTree(DECL_VAR, lineno);
			(yyval.p_tree)->attr.name = copyString((yyvsp[0].p_s));
		}
#line 1713 "pascal.tab.c" /* yacc.c:1646  */
    break;

  case 26:
#line 288 "pascal.y" /* yacc.c:1646  */
    {
			(yyval.p_tree) = newDeclTree(DECL_VAR, lineno);
			(yyval.p_tree)->attr.name = copyString((yyvsp[0].p_s));
		}
#line 1722 "pascal.tab.c" /* yacc.c:1646  */
    break;

  case 27:
#line 293 "pascal.y" /* yacc.c:1646  */
    {
			(yyval.p_tree) = (yyvsp[-1].p_tree);
		}
#line 1730 "pascal.tab.c" /* yacc.c:1646  */
    break;

  case 28:
#line 297 "pascal.y" /* yacc.c:1646  */
    {
			(yyval.p_tree) = newDeclTree(DECL_SIMPLE, lineno);
			(yyval.p_tree)->child[0] = (yyvsp[-2].p_tree);
			(yyval.p_tree)->child[1] = (yyvsp[0].p_tree);
		}
#line 1740 "pascal.tab.c" /* yacc.c:1646  */
    break;

  case 29:
#line 303 "pascal.y" /* yacc.c:1646  */
    {
			(yyval.p_tree) = newDeclTree(DECL_SIMPLE, lineno);
			(yyval.p_tree)->child[0] = (yyvsp[-2].p_tree);
			(yyval.p_tree)->child[1] = (yyvsp[0].p_tree);
		}
#line 1750 "pascal.tab.c" /* yacc.c:1646  */
    break;

  case 30:
#line 309 "pascal.y" /* yacc.c:1646  */
    {
			(yyval.p_tree) = newDeclTree(DECL_SIMPLE, lineno);
			(yyval.p_tree)->child[0] = (yyvsp[-3].p_tree);
			(yyval.p_tree)->child[1] = (yyvsp[0].p_tree);
		}
#line 1760 "pascal.tab.c" /* yacc.c:1646  */
    break;

  case 31:
#line 315 "pascal.y" /* yacc.c:1646  */
    {
			(yyval.p_tree) = newExpTree(EXP_ID, lineno);
			(yyval.p_tree)->attr.name = copyString((yyvsp[-2].p_s));
			(yyval.p_tree)->child[0] = newExpTree(EXP_ID, lineno);
			(yyval.p_tree)->child[0]->attr.name = copyString((yyvsp[0].p_s));
		}
#line 1771 "pascal.tab.c" /* yacc.c:1646  */
    break;

  case 32:
#line 323 "pascal.y" /* yacc.c:1646  */
    {
			(yyval.p_tree) = newDeclTree(DECL_ARRAY, lineno);
			(yyval.p_tree)->child[0] = (yyvsp[-3].p_tree);
			(yyval.p_tree)->child[1] = (yyvsp[0].p_tree);
		}
#line 1781 "pascal.tab.c" /* yacc.c:1646  */
    break;

  case 33:
#line 329 "pascal.y" /* yacc.c:1646  */
    {
			(yyval.p_tree) = (yyvsp[-1].p_tree);
		}
#line 1789 "pascal.tab.c" /* yacc.c:1646  */
    break;

  case 34:
#line 333 "pascal.y" /* yacc.c:1646  */
    {
			Tree *t = (yyvsp[-1].p_tree);
			if (t != NULL) {
				for(t; t->sibling; t = t->sibling);
				t->sibling = (yyvsp[0].p_tree);
				(yyvsp[0].p_tree)->sibling = NULL;
				(yyval.p_tree) = (yyvsp[-1].p_tree);
			} else {
				(yyval.p_tree) = (yyvsp[0].p_tree);
			}
		}
#line 1805 "pascal.tab.c" /* yacc.c:1646  */
    break;

  case 35:
#line 345 "pascal.y" /* yacc.c:1646  */
    {
			(yyval.p_tree) = (yyvsp[0].p_tree);
		}
#line 1813 "pascal.tab.c" /* yacc.c:1646  */
    break;

  case 36:
#line 350 "pascal.y" /* yacc.c:1646  */
    {
			(yyval.p_tree) = newDeclTree(DECL_FIELD, lineno);
			(yyval.p_tree)->child[0] = (yyvsp[-3].p_tree);
			(yyval.p_tree)->child[1] = (yyvsp[-1].p_tree);
		}
#line 1823 "pascal.tab.c" /* yacc.c:1646  */
    break;

  case 37:
#line 356 "pascal.y" /* yacc.c:1646  */
    {
			Tree *t = (yyvsp[-2].p_tree);
			if (t != NULL) {
				for (t; t->sibling; t = t->sibling);
				t->sibling = newExpTree(EXP_ID, lineno);
				t->sibling->attr.name = copyString((yyvsp[0].p_s));
				(yyval.p_tree) = (yyvsp[-2].p_tree);
			} else {
				(yyval.p_tree) = newExpTree(EXP_ID, lineno);
				(yyval.p_tree)->attr.name = copyString((yyvsp[0].p_s));
			}
		}
#line 1840 "pascal.tab.c" /* yacc.c:1646  */
    break;

  case 38:
#line 369 "pascal.y" /* yacc.c:1646  */
    {
			(yyval.p_tree) = newExpTree(EXP_ID, lineno);
			(yyval.p_tree)->attr.name = copyString((yyvsp[0].p_s));
		}
#line 1849 "pascal.tab.c" /* yacc.c:1646  */
    break;

  case 39:
#line 375 "pascal.y" /* yacc.c:1646  */
    {
			(yyval.p_tree) = (yyvsp[0].p_tree);
		}
#line 1857 "pascal.tab.c" /* yacc.c:1646  */
    break;

  case 40:
#line 379 "pascal.y" /* yacc.c:1646  */
    {
			(yyval.p_tree) = NULL;
		}
#line 1865 "pascal.tab.c" /* yacc.c:1646  */
    break;

  case 41:
#line 384 "pascal.y" /* yacc.c:1646  */
    {
			Tree *t = (yyvsp[-1].p_tree);
			if (t != NULL) {
				for (t; t->sibling; t = t->sibling);
				t->sibling = (yyvsp[0].p_tree);
				(yyvsp[0].p_tree)->sibling = NULL;
				(yyval.p_tree) = (yyvsp[-1].p_tree);
			} else {
				(yyval.p_tree) = (yyvsp[0].p_tree);
			}
		}
#line 1881 "pascal.tab.c" /* yacc.c:1646  */
    break;

  case 42:
#line 396 "pascal.y" /* yacc.c:1646  */
    {
			(yyval.p_tree) = (yyvsp[0].p_tree);
		}
#line 1889 "pascal.tab.c" /* yacc.c:1646  */
    break;

  case 43:
#line 401 "pascal.y" /* yacc.c:1646  */
    {
			(yyval.p_tree) = newDeclTree(DECL_FIELD, lineno);
			(yyval.p_tree)->child[0] = (yyvsp[-3].p_tree);
			(yyval.p_tree)->child[1] = (yyvsp[-1].p_tree);
		}
#line 1899 "pascal.tab.c" /* yacc.c:1646  */
    break;

  case 44:
#line 408 "pascal.y" /* yacc.c:1646  */
    {
			Tree *t = (yyvsp[-1].p_tree);
			if (t != NULL) {
				for (t; t->sibling; t = t->sibling);
				t->sibling = (yyvsp[0].p_tree);
				(yyvsp[0].p_tree)->sibling = NULL;
				(yyval.p_tree) = (yyvsp[-1].p_tree);
			} else {
				(yyval.p_tree) = (yyvsp[0].p_tree);
			}
		}
#line 1915 "pascal.tab.c" /* yacc.c:1646  */
    break;

  case 45:
#line 420 "pascal.y" /* yacc.c:1646  */
    {
			Tree *t = (yyvsp[-1].p_tree);
			if (t != NULL) {
				for (t; t->sibling; t = t->sibling);
				t->sibling = (yyvsp[0].p_tree);
				(yyvsp[0].p_tree)->sibling = NULL;
				(yyval.p_tree) = (yyvsp[-1].p_tree);
			} else {
				(yyval.p_tree) = (yyvsp[0].p_tree);
			}	
		}
#line 1931 "pascal.tab.c" /* yacc.c:1646  */
    break;

  case 46:
#line 432 "pascal.y" /* yacc.c:1646  */
    {
			(yyval.p_tree) = (yyvsp[0].p_tree);
		}
#line 1939 "pascal.tab.c" /* yacc.c:1646  */
    break;

  case 47:
#line 436 "pascal.y" /* yacc.c:1646  */
    {
			(yyval.p_tree) = (yyvsp[0].p_tree);
		}
#line 1947 "pascal.tab.c" /* yacc.c:1646  */
    break;

  case 48:
#line 440 "pascal.y" /* yacc.c:1646  */
    {
			(yyval.p_tree) = NULL;
		}
#line 1955 "pascal.tab.c" /* yacc.c:1646  */
    break;

  case 49:
#line 445 "pascal.y" /* yacc.c:1646  */
    {
			(yyval.p_tree) = newModuleTree(MODULE_FUNCTION, lineno);
			(yyval.p_tree)->child[0] = (yyvsp[-3].p_tree);
			(yyval.p_tree)->child[1] = (yyvsp[-1].p_tree);
		}
#line 1965 "pascal.tab.c" /* yacc.c:1646  */
    break;

  case 50:
#line 451 "pascal.y" /* yacc.c:1646  */
    {
			(yyval.p_tree) = newModuleTree(MODULE_FUNCTIONHEAD, lineno);
			(yyval.p_tree)->child[0] = (yyvsp[-2].p_tree);
			(yyval.p_tree)->child[1] = (yyvsp[0].p_tree);
			(yyval.p_tree)->attr.name = copyString((yyvsp[-3].p_s));
		}
#line 1976 "pascal.tab.c" /* yacc.c:1646  */
    break;

  case 51:
#line 459 "pascal.y" /* yacc.c:1646  */
    {
			(yyval.p_tree) = newModuleTree(MODULE_PROCEDURE, lineno);
			(yyval.p_tree)->child[0] = (yyvsp[-3].p_tree);
			(yyval.p_tree)->child[1] = (yyvsp[-1].p_tree);
		}
#line 1986 "pascal.tab.c" /* yacc.c:1646  */
    break;

  case 52:
#line 466 "pascal.y" /* yacc.c:1646  */
    {
			(yyval.p_tree) = newModuleTree(MODULE_PROCEDUREHEAD, lineno);
			(yyval.p_tree)->child[0] = (yyvsp[0].p_tree);
			(yyval.p_tree)->attr.name = copyString((yyvsp[-1].p_s));
		}
#line 1996 "pascal.tab.c" /* yacc.c:1646  */
    break;

  case 53:
#line 473 "pascal.y" /* yacc.c:1646  */
    {
			(yyval.p_tree) = (yyvsp[-1].p_tree);
		}
#line 2004 "pascal.tab.c" /* yacc.c:1646  */
    break;

  case 54:
#line 477 "pascal.y" /* yacc.c:1646  */
    {
			(yyval.p_tree) = NULL;
		}
#line 2012 "pascal.tab.c" /* yacc.c:1646  */
    break;

  case 55:
#line 482 "pascal.y" /* yacc.c:1646  */
    {
			Tree *t = (yyvsp[-2].p_tree);
			if (t != NULL) {
				for (t; t->sibling; t = t->sibling);
				t->sibling = (yyvsp[0].p_tree);
				(yyvsp[0].p_tree)->sibling = NULL;
				(yyval.p_tree) = (yyvsp[-2].p_tree);
			} else {
				(yyval.p_tree) = (yyvsp[0].p_tree);
			}
		}
#line 2028 "pascal.tab.c" /* yacc.c:1646  */
    break;

  case 56:
#line 494 "pascal.y" /* yacc.c:1646  */
    {
			(yyval.p_tree) = (yyvsp[0].p_tree);
		}
#line 2036 "pascal.tab.c" /* yacc.c:1646  */
    break;

  case 57:
#line 499 "pascal.y" /* yacc.c:1646  */
    {
			(yyval.p_tree) = newDeclTree(DECL_PARA, lineno);
			(yyval.p_tree)->child[0] = (yyvsp[-2].p_tree);
			(yyval.p_tree)->child[1] = (yyvsp[0].p_tree);
		}
#line 2046 "pascal.tab.c" /* yacc.c:1646  */
    break;

  case 58:
#line 505 "pascal.y" /* yacc.c:1646  */
    {
			(yyval.p_tree) = newDeclTree(DECL_PARA, lineno);
			(yyval.p_tree)->child[0] = (yyvsp[-2].p_tree);
			(yyval.p_tree)->child[1] = (yyvsp[0].p_tree);
		}
#line 2056 "pascal.tab.c" /* yacc.c:1646  */
    break;

  case 59:
#line 512 "pascal.y" /* yacc.c:1646  */
    {
			(yyval.p_tree) = (yyvsp[0].p_tree);
		}
#line 2064 "pascal.tab.c" /* yacc.c:1646  */
    break;

  case 60:
#line 517 "pascal.y" /* yacc.c:1646  */
    {
			(yyval.p_tree) = (yyvsp[0].p_tree);
		}
#line 2072 "pascal.tab.c" /* yacc.c:1646  */
    break;

  case 61:
#line 522 "pascal.y" /* yacc.c:1646  */
    {
			(yyval.p_tree) = (yyvsp[0].p_tree);
		}
#line 2080 "pascal.tab.c" /* yacc.c:1646  */
    break;

  case 62:
#line 527 "pascal.y" /* yacc.c:1646  */
    {
			(yyval.p_tree) = (yyvsp[-1].p_tree);
		}
#line 2088 "pascal.tab.c" /* yacc.c:1646  */
    break;

  case 63:
#line 532 "pascal.y" /* yacc.c:1646  */
    {
			if ((yyvsp[-2].p_tree) != NULL) {
				Tree *t = (yyvsp[-2].p_tree);
				for (t; t->sibling; t = t->sibling);
				t->sibling = (yyvsp[-1].p_tree);
				(yyvsp[-1].p_tree)->sibling = NULL;
				(yyval.p_tree) = (yyvsp[-2].p_tree);
			} else {
				(yyval.p_tree) = (yyvsp[-1].p_tree);
			}
		}
#line 2104 "pascal.tab.c" /* yacc.c:1646  */
    break;

  case 64:
#line 544 "pascal.y" /* yacc.c:1646  */
    {
			(yyval.p_tree) = NULL;
		}
#line 2112 "pascal.tab.c" /* yacc.c:1646  */
    break;

  case 65:
#line 549 "pascal.y" /* yacc.c:1646  */
    {
			(yyval.p_tree) = newStmtTree(STMT_LABEL, lineno);
			(yyval.p_tree)->child[0] = (yyvsp[0].p_tree);
			(yyval.p_tree)->attr.goto_label = (yyvsp[-2].p_i);
		}
#line 2122 "pascal.tab.c" /* yacc.c:1646  */
    break;

  case 66:
#line 555 "pascal.y" /* yacc.c:1646  */
    {
			(yyval.p_tree) = (yyvsp[0].p_tree);
		}
#line 2130 "pascal.tab.c" /* yacc.c:1646  */
    break;

  case 67:
#line 560 "pascal.y" /* yacc.c:1646  */
    {
			(yyval.p_tree) = (yyvsp[0].p_tree);
		}
#line 2138 "pascal.tab.c" /* yacc.c:1646  */
    break;

  case 68:
#line 564 "pascal.y" /* yacc.c:1646  */
    {
			(yyval.p_tree) = (yyvsp[0].p_tree);
		}
#line 2146 "pascal.tab.c" /* yacc.c:1646  */
    break;

  case 69:
#line 568 "pascal.y" /* yacc.c:1646  */
    {
			(yyval.p_tree) = (yyvsp[0].p_tree);
		}
#line 2154 "pascal.tab.c" /* yacc.c:1646  */
    break;

  case 70:
#line 572 "pascal.y" /* yacc.c:1646  */
    {
			(yyval.p_tree) = (yyvsp[0].p_tree);
		}
#line 2162 "pascal.tab.c" /* yacc.c:1646  */
    break;

  case 71:
#line 576 "pascal.y" /* yacc.c:1646  */
    {
			(yyval.p_tree) = (yyvsp[0].p_tree);
		}
#line 2170 "pascal.tab.c" /* yacc.c:1646  */
    break;

  case 72:
#line 580 "pascal.y" /* yacc.c:1646  */
    {
			(yyval.p_tree) = (yyvsp[0].p_tree);
		}
#line 2178 "pascal.tab.c" /* yacc.c:1646  */
    break;

  case 73:
#line 584 "pascal.y" /* yacc.c:1646  */
    {
			(yyval.p_tree) = (yyvsp[0].p_tree);
		}
#line 2186 "pascal.tab.c" /* yacc.c:1646  */
    break;

  case 74:
#line 588 "pascal.y" /* yacc.c:1646  */
    {
			(yyval.p_tree) = (yyvsp[0].p_tree);
		}
#line 2194 "pascal.tab.c" /* yacc.c:1646  */
    break;

  case 75:
#line 592 "pascal.y" /* yacc.c:1646  */
    {
			(yyval.p_tree) = (yyvsp[0].p_tree);
		}
#line 2202 "pascal.tab.c" /* yacc.c:1646  */
    break;

  case 76:
#line 597 "pascal.y" /* yacc.c:1646  */
    {
			(yyval.p_tree) = newStmtTree(STMT_ASSIGN, lineno);
			(yyval.p_tree)->attr.name = copyString((yyvsp[-2].p_s));
			(yyval.p_tree)->child[0] = (yyvsp[0].p_tree);
		}
#line 2212 "pascal.tab.c" /* yacc.c:1646  */
    break;

  case 77:
#line 604 "pascal.y" /* yacc.c:1646  */
    {
			(yyval.p_tree) = newStmtTree(STMT_ASSIGN, lineno);
			(yyval.p_tree)->attr.name = copyString((yyvsp[-5].p_s));
			(yyval.p_tree)->child[0] = (yyvsp[-3].p_tree);
			(yyval.p_tree)->child[1] = (yyvsp[0].p_tree);
		}
#line 2223 "pascal.tab.c" /* yacc.c:1646  */
    break;

  case 78:
#line 611 "pascal.y" /* yacc.c:1646  */
    {
			(yyval.p_tree) = newStmtTree(STMT_ASSIGN, lineno);
			(yyval.p_tree)->attr.name = copyString((yyvsp[-4].p_s));
			(yyval.p_tree)->child[0] = newExpTree(EXP_ID, lineno);
			(yyval.p_tree)->child[0]->attr.name = copyString((yyvsp[-2].p_s));
		}
#line 2234 "pascal.tab.c" /* yacc.c:1646  */
    break;

  case 79:
#line 619 "pascal.y" /* yacc.c:1646  */
    {
			(yyval.p_tree) = newExpTree(EXP_ID, lineno);
			(yyval.p_tree)->attr.name = copyString((yyvsp[0].p_s));
		}
#line 2243 "pascal.tab.c" /* yacc.c:1646  */
    break;

  case 80:
#line 624 "pascal.y" /* yacc.c:1646  */
    {
			(yyval.p_tree) = newExpTree(EXP_ID, lineno);
			(yyval.p_tree)->child[0] = (yyvsp[-1].p_tree);
			(yyval.p_tree)->attr.name = copyString((yyvsp[-3].p_s));
		}
#line 2253 "pascal.tab.c" /* yacc.c:1646  */
    break;

  case 81:
#line 630 "pascal.y" /* yacc.c:1646  */
    {
			(yyval.p_tree) = newExpTree(EXP_ID, lineno);
			(yyval.p_tree)->attr.name = copyString((yyvsp[0].p_s));
		}
#line 2262 "pascal.tab.c" /* yacc.c:1646  */
    break;

  case 82:
#line 635 "pascal.y" /* yacc.c:1646  */
    {
			(yyval.p_tree) = newExpTree(EXP_ID, lineno);
			(yyval.p_tree)->child[0] = (yyvsp[-1].p_tree);
			(yyval.p_tree)->attr.name = copyString((yyvsp[-3].p_s));
		}
#line 2272 "pascal.tab.c" /* yacc.c:1646  */
    break;

  case 83:
#line 641 "pascal.y" /* yacc.c:1646  */
    {
			(yyval.p_tree) = newExpTree(EXP_ID, lineno);
			(yyval.p_tree)->child[0] = (yyvsp[-1].p_tree);
			(yyval.p_tree)->attr.name = copyString("read");
		}
#line 2282 "pascal.tab.c" /* yacc.c:1646  */
    break;

  case 84:
#line 648 "pascal.y" /* yacc.c:1646  */
    {
			(yyval.p_tree) = newStmtTree(STMT_IF, lineno);
			(yyval.p_tree)->child[0] = (yyvsp[-3].p_tree);
			(yyval.p_tree)->child[1] = (yyvsp[-1].p_tree);
			(yyval.p_tree)->child[2] = (yyvsp[0].p_tree);
		}
#line 2293 "pascal.tab.c" /* yacc.c:1646  */
    break;

  case 85:
#line 656 "pascal.y" /* yacc.c:1646  */
    {
			(yyval.p_tree) = (yyvsp[0].p_tree);
		}
#line 2301 "pascal.tab.c" /* yacc.c:1646  */
    break;

  case 86:
#line 660 "pascal.y" /* yacc.c:1646  */
    {
			(yyval.p_tree) = NULL;
		}
#line 2309 "pascal.tab.c" /* yacc.c:1646  */
    break;

  case 87:
#line 665 "pascal.y" /* yacc.c:1646  */
    {
			(yyval.p_tree) = newStmtTree(STMT_REPEAT, lineno);
			(yyval.p_tree)->child[0] = (yyvsp[-2].p_tree);
			(yyval.p_tree)->child[1] = (yyvsp[0].p_tree);
		}
#line 2319 "pascal.tab.c" /* yacc.c:1646  */
    break;

  case 88:
#line 672 "pascal.y" /* yacc.c:1646  */
    {
			(yyval.p_tree) = newStmtTree(STMT_WHILE, lineno);
			(yyval.p_tree)->child[0] = (yyvsp[-2].p_tree);
			(yyval.p_tree)->child[1] = (yyvsp[0].p_tree);
		}
#line 2329 "pascal.tab.c" /* yacc.c:1646  */
    break;

  case 89:
#line 679 "pascal.y" /* yacc.c:1646  */
    {
			(yyval.p_tree) = newStmtTree(STMT_FOR, lineno);
			(yyval.p_tree)->attr.name = copyString((yyvsp[-6].p_s));
			(yyval.p_tree)->direction = (yyvsp[-3].p_i);
			(yyval.p_tree)->child[0] = (yyvsp[-4].p_tree);
			(yyval.p_tree)->child[1] = (yyvsp[-2].p_tree);
			(yyval.p_tree)->child[2] = (yyvsp[0].p_tree);
		}
#line 2342 "pascal.tab.c" /* yacc.c:1646  */
    break;

  case 90:
#line 689 "pascal.y" /* yacc.c:1646  */
    {
			(yyval.p_i) = 1;
		}
#line 2350 "pascal.tab.c" /* yacc.c:1646  */
    break;

  case 91:
#line 693 "pascal.y" /* yacc.c:1646  */
    {
			(yyval.p_i) = 0;
		}
#line 2358 "pascal.tab.c" /* yacc.c:1646  */
    break;

  case 92:
#line 698 "pascal.y" /* yacc.c:1646  */
    {
			(yyval.p_tree) = newStmtTree(STMT_CASE, lineno);
			(yyval.p_tree)->child[0] = (yyvsp[-3].p_tree);
			(yyval.p_tree)->child[1] = (yyvsp[-1].p_tree);
		}
#line 2368 "pascal.tab.c" /* yacc.c:1646  */
    break;

  case 93:
#line 704 "pascal.y" /* yacc.c:1646  */
    {
			Tree *t = (yyvsp[-1].p_tree);
			if (t != NULL) {
				for (t; t->sibling; t = t->sibling);
				t->sibling = (yyvsp[0].p_tree);
				(yyvsp[0].p_tree)->sibling = NULL;
				(yyval.p_tree) = (yyvsp[-1].p_tree);
			} else {
				(yyval.p_tree) = (yyvsp[0].p_tree);
			}
		}
#line 2384 "pascal.tab.c" /* yacc.c:1646  */
    break;

  case 94:
#line 716 "pascal.y" /* yacc.c:1646  */
    {
			(yyval.p_tree) = (yyvsp[0].p_tree);
		}
#line 2392 "pascal.tab.c" /* yacc.c:1646  */
    break;

  case 95:
#line 721 "pascal.y" /* yacc.c:1646  */
    {
			(yyval.p_tree) = newExpTree(EXP_CASE, lineno);
			(yyval.p_tree)->child[0] = (yyvsp[-3].p_tree);
			(yyval.p_tree)->child[1] = (yyvsp[-1].p_tree);
		}
#line 2402 "pascal.tab.c" /* yacc.c:1646  */
    break;

  case 96:
#line 727 "pascal.y" /* yacc.c:1646  */
    {
			(yyval.p_tree) = newExpTree(EXP_CASE, lineno);
			(yyval.p_tree)->attr.name = copyString((yyvsp[-3].p_s));
			(yyval.p_tree)->child[0] = (yyvsp[-1].p_tree);
		}
#line 2412 "pascal.tab.c" /* yacc.c:1646  */
    break;

  case 97:
#line 734 "pascal.y" /* yacc.c:1646  */
    {
			(yyval.p_tree) = newStmtTree(STMT_GOTO, lineno);
			(yyval.p_tree)->attr.goto_label = (yyvsp[0].p_i);
		}
#line 2421 "pascal.tab.c" /* yacc.c:1646  */
    break;

  case 98:
#line 740 "pascal.y" /* yacc.c:1646  */
    {
			Tree *t = (yyvsp[-2].p_tree);
				if (t != NULL) {
				for (t; t->sibling; t = t->sibling);
				t->sibling = (yyvsp[0].p_tree);
				(yyvsp[0].p_tree)->sibling = NULL;
				(yyval.p_tree) = (yyvsp[-2].p_tree);
			} else {
				(yyval.p_tree) = (yyvsp[0].p_tree);
			}
		}
#line 2437 "pascal.tab.c" /* yacc.c:1646  */
    break;

  case 99:
#line 752 "pascal.y" /* yacc.c:1646  */
    {
			(yyval.p_tree) = (yyvsp[0].p_tree);
		}
#line 2445 "pascal.tab.c" /* yacc.c:1646  */
    break;

  case 100:
#line 757 "pascal.y" /* yacc.c:1646  */
    {
			(yyval.p_tree) = newExpTree(EXP_OP, lineno);
			(yyval.p_tree)->child[0] = (yyvsp[-2].p_tree);
			(yyval.p_tree)->child[1] = (yyvsp[0].p_tree);
			(yyval.p_tree)->attr.op = GE;
		}
#line 2456 "pascal.tab.c" /* yacc.c:1646  */
    break;

  case 101:
#line 764 "pascal.y" /* yacc.c:1646  */
    {
			(yyval.p_tree) = newExpTree(EXP_OP, lineno);
			(yyval.p_tree)->child[0] = (yyvsp[-2].p_tree);
			(yyval.p_tree)->child[1] = (yyvsp[0].p_tree);
			(yyval.p_tree)->attr.op = GT;
		}
#line 2467 "pascal.tab.c" /* yacc.c:1646  */
    break;

  case 102:
#line 771 "pascal.y" /* yacc.c:1646  */
    {
			(yyval.p_tree) = newExpTree(EXP_OP, lineno);
			(yyval.p_tree)->child[0] = (yyvsp[-2].p_tree);
			(yyval.p_tree)->child[1] = (yyvsp[0].p_tree);
			(yyval.p_tree)->attr.op = LE;
		}
#line 2478 "pascal.tab.c" /* yacc.c:1646  */
    break;

  case 103:
#line 778 "pascal.y" /* yacc.c:1646  */
    {
			(yyval.p_tree) = newExpTree(EXP_OP, lineno);
			(yyval.p_tree)->child[0] = (yyvsp[-2].p_tree);
			(yyval.p_tree)->child[1] = (yyvsp[0].p_tree);
			(yyval.p_tree)->attr.op = LT;
		}
#line 2489 "pascal.tab.c" /* yacc.c:1646  */
    break;

  case 104:
#line 785 "pascal.y" /* yacc.c:1646  */
    {
			(yyval.p_tree) = newExpTree(EXP_OP, lineno);
			(yyval.p_tree)->child[0] = (yyvsp[-2].p_tree);
			(yyval.p_tree)->child[1] = (yyvsp[0].p_tree);
			(yyval.p_tree)->attr.op = EQUAL;
		}
#line 2500 "pascal.tab.c" /* yacc.c:1646  */
    break;

  case 105:
#line 792 "pascal.y" /* yacc.c:1646  */
    {
			(yyval.p_tree) = newExpTree(EXP_OP, lineno);
			(yyval.p_tree)->child[0] = (yyvsp[-2].p_tree);
			(yyval.p_tree)->child[1] = (yyvsp[0].p_tree);
			(yyval.p_tree)->attr.op = UNEQUAL;
		}
#line 2511 "pascal.tab.c" /* yacc.c:1646  */
    break;

  case 106:
#line 799 "pascal.y" /* yacc.c:1646  */
    {
			(yyval.p_tree) = (yyvsp[0].p_tree);
		}
#line 2519 "pascal.tab.c" /* yacc.c:1646  */
    break;

  case 107:
#line 804 "pascal.y" /* yacc.c:1646  */
    {
			(yyval.p_tree) = newExpTree(EXP_OP, lineno);
			(yyval.p_tree)->child[0] = (yyvsp[-2].p_tree);
			(yyval.p_tree)->child[1] = (yyvsp[0].p_tree);
			(yyval.p_tree)->attr.op = PLUS;
		}
#line 2530 "pascal.tab.c" /* yacc.c:1646  */
    break;

  case 108:
#line 811 "pascal.y" /* yacc.c:1646  */
    {
			(yyval.p_tree) = newExpTree(EXP_OP, lineno);
			(yyval.p_tree)->child[0] = (yyvsp[-2].p_tree);
			(yyval.p_tree)->child[1] = (yyvsp[0].p_tree);
			(yyval.p_tree)->attr.op = MINUS;
		}
#line 2541 "pascal.tab.c" /* yacc.c:1646  */
    break;

  case 109:
#line 818 "pascal.y" /* yacc.c:1646  */
    {
			(yyval.p_tree) = newExpTree(EXP_OP, lineno);
			(yyval.p_tree)->child[0] = (yyvsp[-2].p_tree);
			(yyval.p_tree)->child[1] = (yyvsp[0].p_tree);
			(yyval.p_tree)->attr.op = OR;
		}
#line 2552 "pascal.tab.c" /* yacc.c:1646  */
    break;

  case 110:
#line 825 "pascal.y" /* yacc.c:1646  */
    {
			(yyval.p_tree) = (yyvsp[0].p_tree);
		}
#line 2560 "pascal.tab.c" /* yacc.c:1646  */
    break;

  case 111:
#line 830 "pascal.y" /* yacc.c:1646  */
    {
			(yyval.p_tree) = newExpTree(EXP_OP, lineno);
			(yyval.p_tree)->child[0] = (yyvsp[-2].p_tree);
			(yyval.p_tree)->child[1] = (yyvsp[0].p_tree);
			(yyval.p_tree)->attr.op = MUL;
		}
#line 2571 "pascal.tab.c" /* yacc.c:1646  */
    break;

  case 112:
#line 837 "pascal.y" /* yacc.c:1646  */
    {
			(yyval.p_tree) = newExpTree(EXP_OP, lineno);
			(yyval.p_tree)->child[0] = (yyvsp[-2].p_tree);
			(yyval.p_tree)->child[1] = (yyvsp[0].p_tree);
			(yyval.p_tree)->attr.op = DIV;
		}
#line 2582 "pascal.tab.c" /* yacc.c:1646  */
    break;

  case 113:
#line 844 "pascal.y" /* yacc.c:1646  */
    {
			(yyval.p_tree) = newExpTree(EXP_OP, lineno);
			(yyval.p_tree)->child[0] = (yyvsp[-2].p_tree);
			(yyval.p_tree)->child[1] = (yyvsp[0].p_tree);
			(yyval.p_tree)->attr.op = MOD;
		}
#line 2593 "pascal.tab.c" /* yacc.c:1646  */
    break;

  case 114:
#line 851 "pascal.y" /* yacc.c:1646  */
    {
			(yyval.p_tree) = newExpTree(EXP_OP, lineno);
			(yyval.p_tree)->child[0] = (yyvsp[-2].p_tree);
			(yyval.p_tree)->child[1] = (yyvsp[0].p_tree);
			(yyval.p_tree)->attr.op = AND;
		}
#line 2604 "pascal.tab.c" /* yacc.c:1646  */
    break;

  case 115:
#line 858 "pascal.y" /* yacc.c:1646  */
    {
			(yyval.p_tree) = (yyvsp[0].p_tree);
		}
#line 2612 "pascal.tab.c" /* yacc.c:1646  */
    break;

  case 116:
#line 863 "pascal.y" /* yacc.c:1646  */
    {
			(yyval.p_tree) = newExpTree(EXP_ID, lineno);
			(yyval.p_tree)->attr.name = copyString((yyvsp[0].p_s));
		}
#line 2621 "pascal.tab.c" /* yacc.c:1646  */
    break;

  case 117:
#line 868 "pascal.y" /* yacc.c:1646  */
    {
			(yyval.p_tree) = newExpTree(EXP_ID, lineno);
			(yyval.p_tree)->child[0] = (yyvsp[-1].p_tree);
			(yyval.p_tree)->attr.name = copyString((yyvsp[-3].p_s));
		}
#line 2631 "pascal.tab.c" /* yacc.c:1646  */
    break;

  case 118:
#line 874 "pascal.y" /* yacc.c:1646  */
    {
			(yyval.p_tree) = newExpTree(EXP_ID, lineno);
			(yyval.p_tree)->attr.name = copyString((yyvsp[0].p_s));
		}
#line 2640 "pascal.tab.c" /* yacc.c:1646  */
    break;

  case 119:
#line 879 "pascal.y" /* yacc.c:1646  */
    {
			(yyval.p_tree) = newExpTree(EXP_ID, lineno);
			(yyval.p_tree)->child[0] = (yyvsp[-1].p_tree);
			(yyval.p_tree)->attr.name = copyString((yyvsp[-3].p_s));
		}
#line 2650 "pascal.tab.c" /* yacc.c:1646  */
    break;

  case 120:
#line 885 "pascal.y" /* yacc.c:1646  */
    {
			(yyval.p_tree) = (yyvsp[0].p_tree);
		}
#line 2658 "pascal.tab.c" /* yacc.c:1646  */
    break;

  case 121:
#line 889 "pascal.y" /* yacc.c:1646  */
    {
			(yyval.p_tree) = (yyvsp[-1].p_tree);
		}
#line 2666 "pascal.tab.c" /* yacc.c:1646  */
    break;

  case 122:
#line 893 "pascal.y" /* yacc.c:1646  */
    {
			(yyval.p_tree) = (yyvsp[0].p_tree);
		}
#line 2674 "pascal.tab.c" /* yacc.c:1646  */
    break;

  case 123:
#line 897 "pascal.y" /* yacc.c:1646  */
    {
			(yyval.p_tree) = (yyvsp[0].p_tree);
		}
#line 2682 "pascal.tab.c" /* yacc.c:1646  */
    break;

  case 124:
#line 901 "pascal.y" /* yacc.c:1646  */
    {
			(yyval.p_tree) = newExpTree(EXP_ID, lineno);
			(yyval.p_tree)->child[0] = (yyvsp[-1].p_tree);
			(yyval.p_tree)->attr.name = copyString((yyvsp[-3].p_s));
		}
#line 2692 "pascal.tab.c" /* yacc.c:1646  */
    break;

  case 125:
#line 907 "pascal.y" /* yacc.c:1646  */
    {		
			(yyval.p_tree) = newExpTree(EXP_ID, lineno);
			(yyval.p_tree)->child[0] = newExpTree(EXP_ID, lineno);
			(yyval.p_tree)->child[0]->attr.name = copyString((yyvsp[0].p_s));
		}
#line 2702 "pascal.tab.c" /* yacc.c:1646  */
    break;

  case 126:
#line 914 "pascal.y" /* yacc.c:1646  */
    {
			Tree *t = (yyvsp[-2].p_tree);
			if (t != NULL) {
				for (t; t->sibling; t = t->sibling);
				t->sibling = (yyvsp[0].p_tree);
				(yyvsp[0].p_tree)->sibling = NULL;
				(yyval.p_tree) = (yyvsp[-2].p_tree);
			} else {
				(yyval.p_tree) = (yyvsp[0].p_tree);
			}
		}
#line 2718 "pascal.tab.c" /* yacc.c:1646  */
    break;

  case 127:
#line 926 "pascal.y" /* yacc.c:1646  */
    {
			(yyval.p_tree) = (yyvsp[0].p_tree);
		}
#line 2726 "pascal.tab.c" /* yacc.c:1646  */
    break;


#line 2730 "pascal.tab.c" /* yacc.c:1646  */
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 931 "pascal.y" /* yacc.c:1906  */


int yyerror(char *s) {
	fprintf(listing, "Syntax error at line %d: %s\n", lineno, s);
	return 0;
}

Tree *parse()
{
	yyparse();
	return rootTree;
}
