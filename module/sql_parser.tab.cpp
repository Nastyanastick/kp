/* A Bison parser, made by GNU Bison 2.7.  */

/* Bison implementation for Yacc-like parsers in C
   
      Copyright (C) 1984, 1989-1990, 2000-2012 Free Software Foundation, Inc.
   
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
#define YYBISON_VERSION "2.7"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
/* Line 371 of yacc.c  */
#line 1 "module/sql_parser.y"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <algorithm>
#include <cctype>
#include "sql_ast.h"

int yylex();
void yyerror(const char *s);

std::vector<sql::Command> parsedCommands;

static sql::SelectCmd currentSelect;
static sql::UpdateCmd currentUpdate;
static sql::DeleteCmd currentDelete;
static sql::InsertCmd currentInsert;
static sql::CreateTableCmd currentCreateTable;

static bool iequals(const char* a, const char* b) {
    if (!a || !b) return false;
    std::string x(a), y(b);
    std::transform(x.begin(), x.end(), x.begin(), [](unsigned char c){ return std::tolower(c); });
    std::transform(y.begin(), y.end(), y.begin(), [](unsigned char c){ return std::tolower(c); });
    return x == y;
}

static char* quote_string(const char* s) {
    std::string tmp = std::string("\"") + (s ? s : "") + "\"";
    char* r = (char*)malloc(tmp.size() + 1);
    memcpy(r, tmp.c_str(), tmp.size() + 1);
    return r;
}

void sql_push_create_database(const char* name);
void sql_push_drop_database(const char* name);
void sql_push_use_database(const char* name);
void sql_push_drop_table(const char* name);

void sql_push_create_table_start(const char* name);
void sql_push_create_table_column(const char* name, const char* type, int notnull, int indexed);
void sql_push_create_table_end();

void sql_push_insert(const char* tableName);
void sql_insert_add_column(const char* name);
void sql_insert_add_row_start();
void sql_insert_add_row_value(const char* val, int isString, int isNull);
void sql_insert_add_row_end();
void sql_push_insert_finalize();

void sql_push_select_start();
void sql_select_add_all();
void sql_select_add_column(const char* name, const char* alias);
void sql_select_add_aggregate(const char* func, const char* arg, const char* alias);
void sql_select_set_table(const char* name);
void sql_select_set_where(ConditionNode* cond);
void sql_push_select_finalize();

void sql_push_update_start(const char* tableName);
void sql_update_add_assignment(const char* col, const char* val);
void sql_update_set_where(ConditionNode* cond);
void sql_push_update_finalize();

void sql_push_delete_start(const char* tableName);
void sql_delete_set_where(ConditionNode* cond);
void sql_push_delete_finalize();

/* Line 371 of yacc.c  */
#line 138 "module/sql_parser.tab.cpp"

# ifndef YY_NULL
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULL nullptr
#  else
#   define YY_NULL 0
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
   by #include "sql_parser.tab.hpp".  */
#ifndef YY_YY_MODULE_SQL_PARSER_TAB_HPP_INCLUDED
# define YY_YY_MODULE_SQL_PARSER_TAB_HPP_INCLUDED
/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif
/* "%code requires" blocks.  */
/* Line 387 of yacc.c  */
#line 71 "module/sql_parser.y"

#include "table.h"
#include "sql_ast.h"


/* Line 387 of yacc.c  */
#line 176 "module/sql_parser.tab.cpp"

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     IDENTIFIER = 258,
     STRING_LITERAL = 259,
     NUMBER = 260,
     NULL_TOKEN = 261,
     CREATE = 262,
     DROP = 263,
     DATABASE = 264,
     USE = 265,
     TABLE = 266,
     INSERT = 267,
     INTO = 268,
     VALUES = 269,
     SELECT = 270,
     FROM = 271,
     WHERE = 272,
     UPDATE = 273,
     SET = 274,
     DELETE = 275,
     AS = 276,
     BETWEEN = 277,
     LIKE = 278,
     SUM = 279,
     COUNT = 280,
     AVG = 281,
     AND = 282,
     OR = 283,
     EQ = 284,
     NEQ = 285,
     LE = 286,
     GE = 287,
     LT = 288,
     GT = 289
   };
#endif


#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{
/* Line 387 of yacc.c  */
#line 76 "module/sql_parser.y"

    char* str;
    ConditionNode* cond;


/* Line 387 of yacc.c  */
#line 231 "module/sql_parser.tab.cpp"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;

#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */

#endif /* !YY_YY_MODULE_SQL_PARSER_TAB_HPP_INCLUDED  */

/* Copy the second part of user declarations.  */

/* Line 390 of yacc.c  */
#line 259 "module/sql_parser.tab.cpp"

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
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
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
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
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

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(N) (N)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
}
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
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
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
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
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
#   if ! defined malloc && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
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
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

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
      while (YYID (0))
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  2
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   146

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  41
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  33
/* YYNRULES -- Number of rules.  */
#define YYNRULES  79
/* YYNRULES -- Number of states.  */
#define YYNSTATES  160

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   289

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      36,    37,    39,     2,    38,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    35,
       2,    40,     2,     2,     2,     2,     2,     2,     2,     2,
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
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     4,     7,    12,    17,    21,    22,    31,
      36,    37,    48,    49,    57,    58,    59,    68,    69,    77,
      78,    85,    87,    91,    94,    98,   100,   104,   106,   110,
     111,   116,   118,   122,   124,   126,   128,   130,   132,   134,
     136,   138,   140,   142,   144,   146,   150,   152,   156,   161,
     168,   173,   180,   185,   192,   197,   204,   205,   208,   209,
     212,   213,   216,   218,   222,   226,   228,   230,   234,   236,
     240,   242,   246,   252,   256,   260,   264,   268,   272,   276
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      42,     0,    -1,    -1,    42,    43,    -1,     7,     9,     3,
      35,    -1,     8,     9,     3,    35,    -1,    10,     3,    35,
      -1,    -1,     7,    11,     3,    44,    36,    51,    37,    35,
      -1,     8,    11,     3,    35,    -1,    -1,    12,    13,     3,
      45,    36,    53,    37,    14,    54,    35,    -1,    -1,    12,
      13,     3,    46,    14,    54,    35,    -1,    -1,    -1,    15,
      47,    61,    16,     3,    48,    64,    35,    -1,    -1,    18,
       3,    49,    19,    67,    65,    35,    -1,    -1,    20,    16,
       3,    50,    66,    35,    -1,    52,    -1,    51,    38,    52,
      -1,     3,     3,    -1,     3,     3,     3,    -1,     3,    -1,
      53,    38,     3,    -1,    55,    -1,    54,    38,    55,    -1,
      -1,    36,    56,    57,    37,    -1,    58,    -1,    57,    38,
      58,    -1,     5,    -1,     4,    -1,     6,    -1,     5,    -1,
       4,    -1,     6,    -1,     5,    -1,     4,    -1,     6,    -1,
      39,    -1,    62,    -1,    63,    -1,    62,    38,    63,    -1,
       3,    -1,     3,    21,     3,    -1,    24,    36,     3,    37,
      -1,    24,    36,     3,    37,    21,     3,    -1,    25,    36,
      39,    37,    -1,    25,    36,    39,    37,    21,     3,    -1,
      25,    36,     3,    37,    -1,    25,    36,     3,    37,    21,
       3,    -1,    26,    36,     3,    37,    -1,    26,    36,     3,
      37,    21,     3,    -1,    -1,    17,    69,    -1,    -1,    17,
      69,    -1,    -1,    17,    69,    -1,    68,    -1,    67,    38,
      68,    -1,     3,    40,    60,    -1,    70,    -1,    71,    -1,
      70,    28,    71,    -1,    72,    -1,    71,    27,    72,    -1,
      73,    -1,    36,    70,    37,    -1,     3,    22,    59,    27,
      59,    -1,     3,    23,    59,    -1,     3,    29,    59,    -1,
       3,    30,    59,    -1,     3,    31,    59,    -1,     3,    32,
      59,    -1,     3,    33,    59,    -1,     3,    34,    59,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   100,   100,   102,   106,   109,   112,   116,   115,   120,
     124,   123,   129,   128,   134,   136,   133,   141,   140,   146,
     145,   152,   153,   157,   163,   177,   180,   185,   186,   191,
     190,   197,   198,   202,   208,   214,   221,   224,   230,   235,
     238,   241,   246,   249,   253,   254,   258,   261,   264,   267,
     270,   273,   276,   279,   282,   285,   291,   293,   299,   301,
     307,   309,   314,   315,   319,   328,   333,   336,   345,   348,
     357,   360,   365,   373,   380,   387,   394,   401,   408,   415
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "IDENTIFIER", "STRING_LITERAL", "NUMBER",
  "NULL_TOKEN", "CREATE", "DROP", "DATABASE", "USE", "TABLE", "INSERT",
  "INTO", "VALUES", "SELECT", "FROM", "WHERE", "UPDATE", "SET", "DELETE",
  "AS", "BETWEEN", "LIKE", "SUM", "COUNT", "AVG", "AND", "OR", "EQ", "NEQ",
  "LE", "GE", "LT", "GT", "';'", "'('", "')'", "','", "'*'", "'='",
  "$accept", "commands", "stmt", "$@1", "$@2", "$@3", "$@4", "$@5", "$@6",
  "$@7", "column_list", "column_def", "id_list", "insert_row_list",
  "insert_row", "$@8", "insert_value_list", "insert_value",
  "compare_value", "assign_value", "select_list", "select_items",
  "select_item", "select_where", "update_where", "delete_where",
  "assignment_list", "assignment", "cond", "or_expr", "and_expr",
  "primary_expr", "comparison_expr", YY_NULL
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,    59,    40,    41,    44,    42,
      61
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    41,    42,    42,    43,    43,    43,    44,    43,    43,
      45,    43,    46,    43,    47,    48,    43,    49,    43,    50,
      43,    51,    51,    52,    52,    53,    53,    54,    54,    56,
      55,    57,    57,    58,    58,    58,    59,    59,    59,    60,
      60,    60,    61,    61,    62,    62,    63,    63,    63,    63,
      63,    63,    63,    63,    63,    63,    64,    64,    65,    65,
      66,    66,    67,    67,    68,    69,    70,    70,    71,    71,
      72,    72,    73,    73,    73,    73,    73,    73,    73,    73
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     2,     4,     4,     3,     0,     8,     4,
       0,    10,     0,     7,     0,     0,     8,     0,     7,     0,
       6,     1,     3,     2,     3,     1,     3,     1,     3,     0,
       4,     1,     3,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     3,     1,     3,     4,     6,
       4,     6,     4,     6,     4,     6,     0,     2,     0,     2,
       0,     2,     1,     3,     3,     1,     1,     3,     1,     3,
       1,     3,     5,     3,     3,     3,     3,     3,     3,     3
};

/* YYDEFACT[STATE-NAME] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       2,     0,     1,     0,     0,     0,     0,    14,     0,     0,
       3,     0,     0,     0,     0,     0,     0,     0,    17,     0,
       0,     7,     0,     0,     6,    10,    46,     0,     0,     0,
      42,     0,    43,    44,     0,    19,     4,     0,     5,     9,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    60,
       0,     0,     0,    47,     0,     0,     0,     0,    15,    45,
       0,    58,    62,     0,     0,     0,     0,    21,    25,     0,
      29,     0,    27,    48,    52,    50,    54,    56,     0,     0,
       0,     0,     0,     0,    61,    65,    66,    68,    70,    20,
      23,     0,     0,     0,     0,     0,    13,     0,     0,     0,
       0,     0,     0,     0,    40,    39,    41,    64,    59,    63,
      18,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    24,     8,    22,     0,    26,    34,    33,    35,
       0,    31,    28,    49,    53,    51,    55,    57,    16,    37,
      36,    38,     0,    73,    74,    75,    76,    77,    78,    79,
      71,    67,    69,     0,    30,     0,     0,    11,    32,    72
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,    10,    37,    40,    41,    17,    77,    34,    49,
      66,    67,    69,    71,    72,    95,   130,   131,   142,   107,
      31,    32,    33,   103,    81,    64,    61,    62,    84,    85,
      86,    87,    88
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -113
static const yytype_int8 yypact[] =
{
    -113,    39,  -113,    27,    41,    14,    29,  -113,    45,    37,
    -113,    55,    68,    69,    70,    40,    71,     4,  -113,    73,
      42,  -113,    43,    44,  -113,    66,    60,    46,    47,    48,
    -113,    72,    49,  -113,    67,  -113,  -113,    53,  -113,  -113,
      54,    77,    82,    89,     6,    90,    91,     7,    92,    79,
      94,    95,    63,  -113,    64,    65,    74,    75,  -113,  -113,
      76,    -3,  -113,     5,    78,    97,   -11,  -113,  -113,    18,
    -113,   -20,  -113,    83,    84,    85,    86,    93,    56,     5,
      92,    80,   -10,     5,  -113,    81,    87,  -113,  -113,  -113,
     100,    88,    94,   103,   105,    59,  -113,    63,   115,   116,
     117,   118,     5,    96,  -113,  -113,  -113,  -113,  -113,  -113,
    -113,    62,    62,    62,    62,    62,    62,    62,    62,   -12,
       5,     5,  -113,  -113,  -113,    63,  -113,  -113,  -113,  -113,
      32,  -113,  -113,  -113,  -113,  -113,  -113,  -113,  -113,  -113,
    -113,  -113,    98,  -113,  -113,  -113,  -113,  -113,  -113,  -113,
    -113,    87,  -113,     2,  -113,    59,    62,  -113,  -113,  -113
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
    -113,  -113,  -113,  -113,  -113,  -113,  -113,  -113,  -113,  -113,
    -113,    30,  -113,    -1,    31,  -113,  -113,   -29,  -112,  -113,
    -113,  -113,    99,  -113,  -113,  -113,  -113,    50,   -68,    51,
       9,    11,  -113
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -13
static const yytype_int16 yytable[] =
{
     143,   144,   145,   146,   147,   148,   149,    26,    82,    55,
      26,   108,   111,   112,    79,    96,   120,    15,    97,   113,
     114,   115,   116,   117,   118,   150,    91,    92,    27,    28,
      29,    27,    28,    29,   137,    80,    11,   157,    12,     2,
      97,    83,    16,    30,   159,    56,     3,     4,    18,     5,
      13,     6,    14,    19,     7,    93,    94,     8,    20,     9,
     104,   105,   106,   127,   128,   129,   139,   140,   141,   154,
     155,    21,    22,    23,    25,    24,    35,    36,    38,    39,
     -12,    42,    43,    44,    45,    53,    48,    47,    46,    50,
      51,    52,    54,    57,    58,    60,    63,    65,    68,    70,
      90,    73,    74,   122,    98,    99,   100,   101,   126,   120,
     102,    75,    76,    89,   121,   110,    78,   125,   133,   134,
     135,   136,   124,   123,   153,   156,   158,     0,   132,   151,
     109,   138,   152,     0,   119,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    59
};

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-113)))

#define yytable_value_is_error(Yytable_value) \
  YYID (0)

static const yytype_int16 yycheck[] =
{
     112,   113,   114,   115,   116,   117,   118,     3,     3,     3,
       3,    79,    22,    23,    17,    35,    28,     3,    38,    29,
      30,    31,    32,    33,    34,    37,    37,    38,    24,    25,
      26,    24,    25,    26,   102,    38,     9,    35,    11,     0,
      38,    36,    13,    39,   156,    39,     7,     8,     3,    10,
       9,    12,    11,    16,    15,    37,    38,    18,     3,    20,
       4,     5,     6,     4,     5,     6,     4,     5,     6,    37,
      38,     3,     3,     3,     3,    35,     3,    35,    35,    35,
      14,    21,    36,    36,    36,     3,    19,    38,    16,    36,
      36,    14,     3,     3,     3,     3,    17,     3,     3,    36,
       3,    37,    37,     3,    21,    21,    21,    21,     3,    28,
      17,    37,    37,    35,    27,    35,    40,    14,     3,     3,
       3,     3,    92,    35,   125,    27,   155,    -1,    97,   120,
      80,    35,   121,    -1,    83,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    47
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    42,     0,     7,     8,    10,    12,    15,    18,    20,
      43,     9,    11,     9,    11,     3,    13,    47,     3,    16,
       3,     3,     3,     3,    35,     3,     3,    24,    25,    26,
      39,    61,    62,    63,    49,     3,    35,    44,    35,    35,
      45,    46,    21,    36,    36,    36,    16,    38,    19,    50,
      36,    36,    14,     3,     3,     3,    39,     3,     3,    63,
       3,    67,    68,    17,    66,     3,    51,    52,     3,    53,
      36,    54,    55,    37,    37,    37,    37,    48,    40,    17,
      38,    65,     3,    36,    69,    70,    71,    72,    73,    35,
       3,    37,    38,    37,    38,    56,    35,    38,    21,    21,
      21,    21,    17,    64,     4,     5,     6,    60,    69,    68,
      35,    22,    23,    29,    30,    31,    32,    33,    34,    70,
      28,    27,     3,    35,    52,    14,     3,     4,     5,     6,
      57,    58,    55,     3,     3,     3,     3,    69,    35,     4,
       5,     6,    59,    59,    59,    59,    59,    59,    59,    59,
      37,    71,    72,    54,    37,    38,    27,    35,    58,    59
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  However,
   YYFAIL appears to be in use.  Nevertheless, it is formally deprecated
   in Bison 2.4.2's NEWS entry, where a plan to phase it out is
   discussed.  */

#define YYFAIL		goto yyerrlab
#if defined YYFAIL
  /* This is here to suppress warnings from the GCC cpp's
     -Wunused-macros.  Normally we don't worry about that warning, but
     some users do, and we want to make it easy for users to remove
     YYFAIL uses, which will produce warnings from Bison 2.5.  */
#endif

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
      YYERROR;							\
    }								\
while (YYID (0))

/* Error token number */
#define YYTERROR	1
#define YYERRCODE	256


/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */
#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
        break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

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
#ifndef	YYINITDEPTH
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
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
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
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
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
  YYSIZE_T yysize0 = yytnamerr (YY_NULL, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULL;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - Assume YYFAIL is not used.  It's too flawed to consider.  See
       <http://lists.gnu.org/archive/html/bison-patches/2009-12/msg00024.html>
       for details.  YYERROR is fine as it does not invoke this
       function.
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
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULL, yytname[yyx]);
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

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
        break;
    }
}




/* The lookahead symbol.  */
int yychar;


#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval YY_INITIAL_VALUE(yyval_default);

/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.

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
      yychar = YYLEX;
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
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 4:
/* Line 1792 of yacc.c  */
#line 107 "module/sql_parser.y"
    { sql_push_create_database((yyvsp[(3) - (4)].str)); free((yyvsp[(3) - (4)].str)); }
    break;

  case 5:
/* Line 1792 of yacc.c  */
#line 110 "module/sql_parser.y"
    { sql_push_drop_database((yyvsp[(3) - (4)].str)); free((yyvsp[(3) - (4)].str)); }
    break;

  case 6:
/* Line 1792 of yacc.c  */
#line 113 "module/sql_parser.y"
    { sql_push_use_database((yyvsp[(2) - (3)].str)); free((yyvsp[(2) - (3)].str)); }
    break;

  case 7:
/* Line 1792 of yacc.c  */
#line 116 "module/sql_parser.y"
    { sql_push_create_table_start((yyvsp[(3) - (3)].str)); free((yyvsp[(3) - (3)].str)); }
    break;

  case 8:
/* Line 1792 of yacc.c  */
#line 118 "module/sql_parser.y"
    { sql_push_create_table_end(); }
    break;

  case 9:
/* Line 1792 of yacc.c  */
#line 121 "module/sql_parser.y"
    { sql_push_drop_table((yyvsp[(3) - (4)].str)); free((yyvsp[(3) - (4)].str)); }
    break;

  case 10:
/* Line 1792 of yacc.c  */
#line 124 "module/sql_parser.y"
    { sql_push_insert((yyvsp[(3) - (3)].str)); free((yyvsp[(3) - (3)].str)); }
    break;

  case 11:
/* Line 1792 of yacc.c  */
#line 126 "module/sql_parser.y"
    { sql_push_insert_finalize(); }
    break;

  case 12:
/* Line 1792 of yacc.c  */
#line 129 "module/sql_parser.y"
    { sql_push_insert((yyvsp[(3) - (3)].str)); free((yyvsp[(3) - (3)].str)); }
    break;

  case 13:
/* Line 1792 of yacc.c  */
#line 131 "module/sql_parser.y"
    { sql_push_insert_finalize(); }
    break;

  case 14:
/* Line 1792 of yacc.c  */
#line 134 "module/sql_parser.y"
    { sql_push_select_start(); }
    break;

  case 15:
/* Line 1792 of yacc.c  */
#line 136 "module/sql_parser.y"
    { sql_select_set_table((yyvsp[(5) - (5)].str)); free((yyvsp[(5) - (5)].str)); }
    break;

  case 16:
/* Line 1792 of yacc.c  */
#line 138 "module/sql_parser.y"
    { sql_push_select_finalize(); }
    break;

  case 17:
/* Line 1792 of yacc.c  */
#line 141 "module/sql_parser.y"
    { sql_push_update_start((yyvsp[(2) - (2)].str)); free((yyvsp[(2) - (2)].str)); }
    break;

  case 18:
/* Line 1792 of yacc.c  */
#line 143 "module/sql_parser.y"
    { sql_push_update_finalize(); }
    break;

  case 19:
/* Line 1792 of yacc.c  */
#line 146 "module/sql_parser.y"
    { sql_push_delete_start((yyvsp[(3) - (3)].str)); free((yyvsp[(3) - (3)].str)); }
    break;

  case 20:
/* Line 1792 of yacc.c  */
#line 148 "module/sql_parser.y"
    { sql_push_delete_finalize(); }
    break;

  case 23:
/* Line 1792 of yacc.c  */
#line 158 "module/sql_parser.y"
    {
            sql_push_create_table_column((yyvsp[(1) - (2)].str), (yyvsp[(2) - (2)].str), 0, 0);
            free((yyvsp[(1) - (2)].str)); free((yyvsp[(2) - (2)].str));
        }
    break;

  case 24:
/* Line 1792 of yacc.c  */
#line 164 "module/sql_parser.y"
    {
            int notnull = 0;
            int indexed = 0;

            if (iequals((yyvsp[(3) - (3)].str), "NOT_NULL")) notnull = 1;
            else if (iequals((yyvsp[(3) - (3)].str), "INDEXED")) indexed = 1;

            sql_push_create_table_column((yyvsp[(1) - (3)].str), (yyvsp[(2) - (3)].str), notnull, indexed);
            free((yyvsp[(1) - (3)].str)); free((yyvsp[(2) - (3)].str)); free((yyvsp[(3) - (3)].str));
        }
    break;

  case 25:
/* Line 1792 of yacc.c  */
#line 178 "module/sql_parser.y"
    { sql_insert_add_column((yyvsp[(1) - (1)].str)); free((yyvsp[(1) - (1)].str)); }
    break;

  case 26:
/* Line 1792 of yacc.c  */
#line 181 "module/sql_parser.y"
    { sql_insert_add_column((yyvsp[(3) - (3)].str)); free((yyvsp[(3) - (3)].str)); }
    break;

  case 29:
/* Line 1792 of yacc.c  */
#line 191 "module/sql_parser.y"
    { sql_insert_add_row_start(); }
    break;

  case 30:
/* Line 1792 of yacc.c  */
#line 193 "module/sql_parser.y"
    { sql_insert_add_row_end(); }
    break;

  case 33:
/* Line 1792 of yacc.c  */
#line 203 "module/sql_parser.y"
    {
            sql_insert_add_row_value((yyvsp[(1) - (1)].str), 0, 0);
            free((yyvsp[(1) - (1)].str));
        }
    break;

  case 34:
/* Line 1792 of yacc.c  */
#line 209 "module/sql_parser.y"
    {
            sql_insert_add_row_value((yyvsp[(1) - (1)].str), 1, 0);
            free((yyvsp[(1) - (1)].str));
        }
    break;

  case 35:
/* Line 1792 of yacc.c  */
#line 215 "module/sql_parser.y"
    {
            sql_insert_add_row_value(nullptr, 0, 1);
        }
    break;

  case 36:
/* Line 1792 of yacc.c  */
#line 222 "module/sql_parser.y"
    { (yyval.str) = (yyvsp[(1) - (1)].str); }
    break;

  case 37:
/* Line 1792 of yacc.c  */
#line 225 "module/sql_parser.y"
    {
            (yyval.str) = quote_string((yyvsp[(1) - (1)].str));
            free((yyvsp[(1) - (1)].str));
        }
    break;

  case 38:
/* Line 1792 of yacc.c  */
#line 231 "module/sql_parser.y"
    { (yyval.str) = strdup("NULL"); }
    break;

  case 39:
/* Line 1792 of yacc.c  */
#line 236 "module/sql_parser.y"
    { (yyval.str) = (yyvsp[(1) - (1)].str); }
    break;

  case 40:
/* Line 1792 of yacc.c  */
#line 239 "module/sql_parser.y"
    { (yyval.str) = (yyvsp[(1) - (1)].str); }
    break;

  case 41:
/* Line 1792 of yacc.c  */
#line 242 "module/sql_parser.y"
    { (yyval.str) = strdup("NULL"); }
    break;

  case 42:
/* Line 1792 of yacc.c  */
#line 247 "module/sql_parser.y"
    { sql_select_add_all(); }
    break;

  case 46:
/* Line 1792 of yacc.c  */
#line 259 "module/sql_parser.y"
    { sql_select_add_column((yyvsp[(1) - (1)].str), nullptr); free((yyvsp[(1) - (1)].str)); }
    break;

  case 47:
/* Line 1792 of yacc.c  */
#line 262 "module/sql_parser.y"
    { sql_select_add_column((yyvsp[(1) - (3)].str), (yyvsp[(3) - (3)].str)); free((yyvsp[(1) - (3)].str)); free((yyvsp[(3) - (3)].str)); }
    break;

  case 48:
/* Line 1792 of yacc.c  */
#line 265 "module/sql_parser.y"
    { sql_select_add_aggregate("SUM", (yyvsp[(3) - (4)].str), nullptr); free((yyvsp[(3) - (4)].str)); }
    break;

  case 49:
/* Line 1792 of yacc.c  */
#line 268 "module/sql_parser.y"
    { sql_select_add_aggregate("SUM", (yyvsp[(3) - (6)].str), (yyvsp[(6) - (6)].str)); free((yyvsp[(3) - (6)].str)); free((yyvsp[(6) - (6)].str)); }
    break;

  case 50:
/* Line 1792 of yacc.c  */
#line 271 "module/sql_parser.y"
    { sql_select_add_aggregate("COUNT", "*", nullptr); }
    break;

  case 51:
/* Line 1792 of yacc.c  */
#line 274 "module/sql_parser.y"
    { sql_select_add_aggregate("COUNT", "*", (yyvsp[(6) - (6)].str)); free((yyvsp[(6) - (6)].str)); }
    break;

  case 52:
/* Line 1792 of yacc.c  */
#line 277 "module/sql_parser.y"
    { sql_select_add_aggregate("COUNT", (yyvsp[(3) - (4)].str), nullptr); free((yyvsp[(3) - (4)].str)); }
    break;

  case 53:
/* Line 1792 of yacc.c  */
#line 280 "module/sql_parser.y"
    { sql_select_add_aggregate("COUNT", (yyvsp[(3) - (6)].str), (yyvsp[(6) - (6)].str)); free((yyvsp[(3) - (6)].str)); free((yyvsp[(6) - (6)].str)); }
    break;

  case 54:
/* Line 1792 of yacc.c  */
#line 283 "module/sql_parser.y"
    { sql_select_add_aggregate("AVG", (yyvsp[(3) - (4)].str), nullptr); free((yyvsp[(3) - (4)].str)); }
    break;

  case 55:
/* Line 1792 of yacc.c  */
#line 286 "module/sql_parser.y"
    { sql_select_add_aggregate("AVG", (yyvsp[(3) - (6)].str), (yyvsp[(6) - (6)].str)); free((yyvsp[(3) - (6)].str)); free((yyvsp[(6) - (6)].str)); }
    break;

  case 56:
/* Line 1792 of yacc.c  */
#line 291 "module/sql_parser.y"
    { sql_select_set_where(nullptr); }
    break;

  case 57:
/* Line 1792 of yacc.c  */
#line 294 "module/sql_parser.y"
    { sql_select_set_where((yyvsp[(2) - (2)].cond)); }
    break;

  case 58:
/* Line 1792 of yacc.c  */
#line 299 "module/sql_parser.y"
    { sql_update_set_where(nullptr); }
    break;

  case 59:
/* Line 1792 of yacc.c  */
#line 302 "module/sql_parser.y"
    { sql_update_set_where((yyvsp[(2) - (2)].cond)); }
    break;

  case 60:
/* Line 1792 of yacc.c  */
#line 307 "module/sql_parser.y"
    { sql_delete_set_where(nullptr); }
    break;

  case 61:
/* Line 1792 of yacc.c  */
#line 310 "module/sql_parser.y"
    { sql_delete_set_where((yyvsp[(2) - (2)].cond)); }
    break;

  case 64:
/* Line 1792 of yacc.c  */
#line 320 "module/sql_parser.y"
    {
            sql_update_add_assignment((yyvsp[(1) - (3)].str), (yyvsp[(3) - (3)].str));
            free((yyvsp[(1) - (3)].str));
            if ((yyvsp[(3) - (3)].str)) free((yyvsp[(3) - (3)].str));
        }
    break;

  case 65:
/* Line 1792 of yacc.c  */
#line 329 "module/sql_parser.y"
    { (yyval.cond) = (yyvsp[(1) - (1)].cond); }
    break;

  case 66:
/* Line 1792 of yacc.c  */
#line 334 "module/sql_parser.y"
    { (yyval.cond) = (yyvsp[(1) - (1)].cond); }
    break;

  case 67:
/* Line 1792 of yacc.c  */
#line 337 "module/sql_parser.y"
    {
            (yyval.cond) = new ConditionNode(ConditionNode::OR);
            (yyval.cond)->leftChild = (yyvsp[(1) - (3)].cond);
            (yyval.cond)->rightChild = (yyvsp[(3) - (3)].cond);
        }
    break;

  case 68:
/* Line 1792 of yacc.c  */
#line 346 "module/sql_parser.y"
    { (yyval.cond) = (yyvsp[(1) - (1)].cond); }
    break;

  case 69:
/* Line 1792 of yacc.c  */
#line 349 "module/sql_parser.y"
    {
            (yyval.cond) = new ConditionNode(ConditionNode::AND);
            (yyval.cond)->leftChild = (yyvsp[(1) - (3)].cond);
            (yyval.cond)->rightChild = (yyvsp[(3) - (3)].cond);
        }
    break;

  case 70:
/* Line 1792 of yacc.c  */
#line 358 "module/sql_parser.y"
    { (yyval.cond) = (yyvsp[(1) - (1)].cond); }
    break;

  case 71:
/* Line 1792 of yacc.c  */
#line 361 "module/sql_parser.y"
    { (yyval.cond) = (yyvsp[(2) - (3)].cond); }
    break;

  case 72:
/* Line 1792 of yacc.c  */
#line 366 "module/sql_parser.y"
    {
            (yyval.cond) = new ConditionNode((yyvsp[(1) - (5)].str), "BETWEEN", (yyvsp[(3) - (5)].str) ? (yyvsp[(3) - (5)].str) : "", (yyvsp[(5) - (5)].str) ? (yyvsp[(5) - (5)].str) : "");
            free((yyvsp[(1) - (5)].str));
            if ((yyvsp[(3) - (5)].str)) free((yyvsp[(3) - (5)].str));
            if ((yyvsp[(5) - (5)].str)) free((yyvsp[(5) - (5)].str));
        }
    break;

  case 73:
/* Line 1792 of yacc.c  */
#line 374 "module/sql_parser.y"
    {
            (yyval.cond) = new ConditionNode((yyvsp[(1) - (3)].str), "LIKE", (yyvsp[(3) - (3)].str) ? (yyvsp[(3) - (3)].str) : "");
            free((yyvsp[(1) - (3)].str));
            if ((yyvsp[(3) - (3)].str)) free((yyvsp[(3) - (3)].str));
        }
    break;

  case 74:
/* Line 1792 of yacc.c  */
#line 381 "module/sql_parser.y"
    {
            (yyval.cond) = new ConditionNode((yyvsp[(1) - (3)].str), "==", (yyvsp[(3) - (3)].str) ? (yyvsp[(3) - (3)].str) : "");
            free((yyvsp[(1) - (3)].str));
            if ((yyvsp[(3) - (3)].str)) free((yyvsp[(3) - (3)].str));
        }
    break;

  case 75:
/* Line 1792 of yacc.c  */
#line 388 "module/sql_parser.y"
    {
            (yyval.cond) = new ConditionNode((yyvsp[(1) - (3)].str), "!=", (yyvsp[(3) - (3)].str) ? (yyvsp[(3) - (3)].str) : "");
            free((yyvsp[(1) - (3)].str));
            if ((yyvsp[(3) - (3)].str)) free((yyvsp[(3) - (3)].str));
        }
    break;

  case 76:
/* Line 1792 of yacc.c  */
#line 395 "module/sql_parser.y"
    {
            (yyval.cond) = new ConditionNode((yyvsp[(1) - (3)].str), "<=", (yyvsp[(3) - (3)].str) ? (yyvsp[(3) - (3)].str) : "");
            free((yyvsp[(1) - (3)].str));
            if ((yyvsp[(3) - (3)].str)) free((yyvsp[(3) - (3)].str));
        }
    break;

  case 77:
/* Line 1792 of yacc.c  */
#line 402 "module/sql_parser.y"
    {
            (yyval.cond) = new ConditionNode((yyvsp[(1) - (3)].str), ">=", (yyvsp[(3) - (3)].str) ? (yyvsp[(3) - (3)].str) : "");
            free((yyvsp[(1) - (3)].str));
            if ((yyvsp[(3) - (3)].str)) free((yyvsp[(3) - (3)].str));
        }
    break;

  case 78:
/* Line 1792 of yacc.c  */
#line 409 "module/sql_parser.y"
    {
            (yyval.cond) = new ConditionNode((yyvsp[(1) - (3)].str), "<", (yyvsp[(3) - (3)].str) ? (yyvsp[(3) - (3)].str) : "");
            free((yyvsp[(1) - (3)].str));
            if ((yyvsp[(3) - (3)].str)) free((yyvsp[(3) - (3)].str));
        }
    break;

  case 79:
/* Line 1792 of yacc.c  */
#line 416 "module/sql_parser.y"
    {
            (yyval.cond) = new ConditionNode((yyvsp[(1) - (3)].str), ">", (yyvsp[(3) - (3)].str) ? (yyvsp[(3) - (3)].str) : "");
            free((yyvsp[(1) - (3)].str));
            if ((yyvsp[(3) - (3)].str)) free((yyvsp[(3) - (3)].str));
        }
    break;


/* Line 1792 of yacc.c  */
#line 2043 "module/sql_parser.tab.cpp"
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

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
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

  /* Do not reclaim the symbols of the rule which action triggered
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
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

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
  /* Do not reclaim the symbols of the rule which action triggered
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
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}


/* Line 2055 of yacc.c  */
#line 423 "module/sql_parser.y"


void yyerror(const char *s) {
    fprintf(stderr, "Parse error: %s\n", s);
}

void sql_push_create_database(const char* name) {
    sql::Command cmd;
    cmd.type = sql::CmdType::CREATE_DATABASE;
    cmd.dbName = name;
    parsedCommands.push_back(cmd);
}

void sql_push_drop_database(const char* name) {
    sql::Command cmd;
    cmd.type = sql::CmdType::DROP_DATABASE;
    cmd.dbName = name;
    parsedCommands.push_back(cmd);
}

void sql_push_use_database(const char* name) {
    sql::Command cmd;
    cmd.type = sql::CmdType::USE_DATABASE;
    cmd.dbName = name;
    parsedCommands.push_back(cmd);
}

void sql_push_drop_table(const char* name) {
    sql::Command cmd;
    cmd.type = sql::CmdType::DROP_TABLE;
    cmd.dropTableName = name;
    parsedCommands.push_back(cmd);
}

void sql_push_create_table_start(const char* name) {
    currentCreateTable.tableName = name;
    currentCreateTable.columns.clear();
}

void sql_push_create_table_column(const char* name, const char* type, int notnull, int indexed) {
    Column col;
    col.name = name;
    col.type = type;
    col.notNull = (notnull == 1) || (indexed == 1);
    col.indexed = (indexed == 1);
    currentCreateTable.columns.push_back(col);
}

void sql_push_create_table_end() {
    sql::Command cmd;
    cmd.type = sql::CmdType::CREATE_TABLE;
    cmd.createTable = currentCreateTable;
    parsedCommands.push_back(cmd);
}

void sql_push_insert(const char* tableName) {
    currentInsert.tableName = tableName;
    currentInsert.columns.clear();
    currentInsert.rows.clear();
}

void sql_insert_add_column(const char* name) {
    currentInsert.columns.push_back(name);
}

void sql_insert_add_row_start() {
    currentInsert.rows.push_back(std::vector<std::string>());
}

void sql_insert_add_row_value(const char* val, int isString, int isNull) {
    if (currentInsert.rows.empty()) {
        currentInsert.rows.push_back(std::vector<std::string>());
    }

    if (isNull) {
        currentInsert.rows.back().push_back("NULL");
        return;
    }

    if (!val) {
        currentInsert.rows.back().push_back("NULL");
        return;
    }

    currentInsert.rows.back().push_back(val);
}

void sql_insert_add_row_end() {
}

void sql_push_insert_finalize() {
    sql::Command cmd;
    cmd.type = sql::CmdType::INSERT;
    cmd.insert = currentInsert;
    parsedCommands.push_back(cmd);
}

void sql_push_select_start() {
    currentSelect.columns.clear();
    currentSelect.tableName.clear();
    currentSelect.where = nullptr;
}

void sql_select_add_all() {
    sql::SelectColumn col;
    col.all = true;
    currentSelect.columns.push_back(col);
}

void sql_select_add_column(const char* name, const char* alias) {
    sql::SelectColumn col;
    col.all = false;
    col.name = name;
    col.alias = alias ? alias : "";
    currentSelect.columns.push_back(col);
}

void sql_select_add_aggregate(const char* func, const char* arg, const char* alias) {
    sql::SelectColumn col;
    col.all = false;
    col.aggregate = true;
    col.aggFunc = func ? func : "";
    col.aggArg = arg ? arg : "";
    col.alias = alias ? alias : "";
    currentSelect.columns.push_back(col);
}

void sql_select_set_table(const char* name) {
    currentSelect.tableName = name;
}

void sql_select_set_where(ConditionNode* cond) {
    currentSelect.where = cond;
}

void sql_push_select_finalize() {
    sql::Command cmd;
    cmd.type = sql::CmdType::SELECT;
    cmd.select = currentSelect;
    parsedCommands.push_back(cmd);
}

void sql_push_update_start(const char* tableName) {
    currentUpdate.tableName = tableName;
    currentUpdate.assignments.clear();
    currentUpdate.where = nullptr;
}

void sql_update_add_assignment(const char* col, const char* val) {
    currentUpdate.assignments.push_back({col, val ? val : "NULL"});
}

void sql_update_set_where(ConditionNode* cond) {
    currentUpdate.where = cond;
}

void sql_push_update_finalize() {
    sql::Command cmd;
    cmd.type = sql::CmdType::UPDATE;
    cmd.update = currentUpdate;
    parsedCommands.push_back(cmd);
}

void sql_push_delete_start(const char* tableName) {
    currentDelete.tableName = tableName;
    currentDelete.where = nullptr;
}

void sql_delete_set_where(ConditionNode* cond) {
    currentDelete.where = cond;
}

void sql_push_delete_finalize() {
    sql::Command cmd;
    cmd.type = sql::CmdType::DELETE;
    cmd.del = currentDelete;
    parsedCommands.push_back(cmd);
}
