/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "module/sql_parser.y"

#include <stdio.h>
#include <string.h>
#include "sql_ast.h"

int yylex();

extern "C" {
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
    void sql_select_set_table(const char* name);
    void sql_select_set_where(const char* left, const char* op, const char* right, const char* right2);
    void sql_push_select_finalize();
    void sql_push_update_start(const char* tableName);
    void sql_update_add_assignment(const char* col, const char* val);
    void sql_update_set_where(const char* left, const char* op, const char* right, const char* right2);
    void sql_push_update_finalize();
    void sql_push_delete_start(const char* tableName);
    void sql_delete_set_where(const char* left, const char* op, const char* right, const char* right2);
    void sql_push_delete_finalize();
}

void yyerror(const char *s);

#line 110 "module/sql_parser.tab.cpp"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

#include "sql_parser.tab.hpp"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_IDENTIFIER = 3,                 /* IDENTIFIER  */
  YYSYMBOL_STRING_LITERAL = 4,             /* STRING_LITERAL  */
  YYSYMBOL_NUMBER = 5,                     /* NUMBER  */
  YYSYMBOL_NULL_TOKEN = 6,                 /* NULL_TOKEN  */
  YYSYMBOL_CREATE = 7,                     /* CREATE  */
  YYSYMBOL_DROP = 8,                       /* DROP  */
  YYSYMBOL_DATABASE = 9,                   /* DATABASE  */
  YYSYMBOL_USE = 10,                       /* USE  */
  YYSYMBOL_TABLE = 11,                     /* TABLE  */
  YYSYMBOL_INSERT = 12,                    /* INSERT  */
  YYSYMBOL_INTO = 13,                      /* INTO  */
  YYSYMBOL_VALUES = 14,                    /* VALUES  */
  YYSYMBOL_VALUE = 15,                     /* VALUE  */
  YYSYMBOL_SELECT = 16,                    /* SELECT  */
  YYSYMBOL_FROM = 17,                      /* FROM  */
  YYSYMBOL_WHERE = 18,                     /* WHERE  */
  YYSYMBOL_UPDATE = 19,                    /* UPDATE  */
  YYSYMBOL_SET = 20,                       /* SET  */
  YYSYMBOL_DELETE = 21,                    /* DELETE  */
  YYSYMBOL_AS = 22,                        /* AS  */
  YYSYMBOL_BETWEEN = 23,                   /* BETWEEN  */
  YYSYMBOL_LIKE = 24,                      /* LIKE  */
  YYSYMBOL_AND = 25,                       /* AND  */
  YYSYMBOL_EQ = 26,                        /* EQ  */
  YYSYMBOL_NEQ = 27,                       /* NEQ  */
  YYSYMBOL_LE = 28,                        /* LE  */
  YYSYMBOL_GE = 29,                        /* GE  */
  YYSYMBOL_LT = 30,                        /* LT  */
  YYSYMBOL_GT = 31,                        /* GT  */
  YYSYMBOL_32_ = 32,                       /* ','  */
  YYSYMBOL_33_ = 33,                       /* ';'  */
  YYSYMBOL_34_ = 34,                       /* '('  */
  YYSYMBOL_35_ = 35,                       /* ')'  */
  YYSYMBOL_36_ = 36,                       /* '*'  */
  YYSYMBOL_37_ = 37,                       /* '='  */
  YYSYMBOL_YYACCEPT = 38,                  /* $accept  */
  YYSYMBOL_commands = 39,                  /* commands  */
  YYSYMBOL_stmt = 40,                      /* stmt  */
  YYSYMBOL_41_1 = 41,                      /* $@1  */
  YYSYMBOL_42_2 = 42,                      /* $@2  */
  YYSYMBOL_43_3 = 43,                      /* $@3  */
  YYSYMBOL_44_4 = 44,                      /* $@4  */
  YYSYMBOL_45_5 = 45,                      /* $@5  */
  YYSYMBOL_46_6 = 46,                      /* $@6  */
  YYSYMBOL_column_list = 47,               /* column_list  */
  YYSYMBOL_column_def = 48,                /* column_def  */
  YYSYMBOL_id_list = 49,                   /* id_list  */
  YYSYMBOL_insert_row_list = 50,           /* insert_row_list  */
  YYSYMBOL_insert_row = 51,                /* insert_row  */
  YYSYMBOL_52_7 = 52,                      /* $@7  */
  YYSYMBOL_value_list = 53,                /* value_list  */
  YYSYMBOL_insert_value = 54,              /* insert_value  */
  YYSYMBOL_value = 55,                     /* value  */
  YYSYMBOL_select_list = 56,               /* select_list  */
  YYSYMBOL_select_items = 57,              /* select_items  */
  YYSYMBOL_select_item = 58,               /* select_item  */
  YYSYMBOL_select_where = 59,              /* select_where  */
  YYSYMBOL_update_where = 60,              /* update_where  */
  YYSYMBOL_delete_where = 61,              /* delete_where  */
  YYSYMBOL_assignment_list = 62,           /* assignment_list  */
  YYSYMBOL_assignment = 63,                /* assignment  */
  YYSYMBOL_cond = 64                       /* cond  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_int8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

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


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
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

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

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
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
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
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  2
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   103

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  38
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  27
/* YYNRULES -- Number of rules.  */
#define YYNRULES  59
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  122

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   286


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      34,    35,    36,     2,    32,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    33,
       2,    37,     2,     2,     2,     2,     2,     2,     2,     2,
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
      25,    26,    27,    28,    29,    30,    31
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint8 yyrline[] =
{
       0,    66,    66,    68,    72,    73,    74,    75,    75,    76,
      77,    77,    78,    78,    78,    79,    79,    80,    80,    84,
      85,    89,    90,    94,    95,    99,   100,   104,   104,   108,
     109,   113,   114,   115,   119,   120,   121,   125,   126,   130,
     131,   135,   136,   139,   141,   144,   146,   149,   151,   155,
     156,   160,   164,   165,   166,   167,   168,   169,   170,   171
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "IDENTIFIER",
  "STRING_LITERAL", "NUMBER", "NULL_TOKEN", "CREATE", "DROP", "DATABASE",
  "USE", "TABLE", "INSERT", "INTO", "VALUES", "VALUE", "SELECT", "FROM",
  "WHERE", "UPDATE", "SET", "DELETE", "AS", "BETWEEN", "LIKE", "AND", "EQ",
  "NEQ", "LE", "GE", "LT", "GT", "','", "';'", "'('", "')'", "'*'", "'='",
  "$accept", "commands", "stmt", "$@1", "$@2", "$@3", "$@4", "$@5", "$@6",
  "column_list", "column_def", "id_list", "insert_row_list", "insert_row",
  "$@7", "value_list", "insert_value", "value", "select_list",
  "select_items", "select_item", "select_where", "update_where",
  "delete_where", "assignment_list", "assignment", "cond", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-80)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int8 yypact[] =
{
     -80,     2,   -80,    19,    31,    12,    11,   -80,    40,    34,
     -80,    49,    50,    51,    52,    23,    54,    -3,   -80,    55,
      26,   -80,    27,    28,   -80,   -80,    42,   -80,    45,    33,
     -80,    43,   -80,   -80,    32,   -80,   -80,    35,    64,    65,
      67,    68,    56,    69,    70,   -80,   -80,   -80,    38,    -5,
     -80,    73,    46,    74,   -16,   -80,   -80,   -15,    60,    41,
      73,    68,    47,     8,   -80,   -80,    78,    69,    53,    79,
      71,    73,    57,   -80,   -80,   -80,   -80,   -80,   -80,   -80,
      41,    80,    41,    41,    41,    41,    41,    41,   -80,   -80,
     -80,   -80,    58,   -80,   -80,    62,   -80,   -80,   -80,   -80,
     -80,   -80,   -80,    59,    41,   -80,    -6,   -80,   -80,    44,
      59,    61,   -80,   -80,   -80,     9,   -80,   -80,   -80,    44,
     -80,   -80
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       2,     0,     1,     0,     0,     0,     0,    12,     0,     0,
       3,     0,     0,     0,     0,     0,     0,     0,    15,     0,
       0,     7,     0,     0,     6,    10,    41,    37,     0,    38,
      39,     0,    17,     4,     0,     5,     9,     0,     0,     0,
       0,     0,    47,     0,     0,    42,    13,    40,     0,    45,
      49,     0,     0,     0,     0,    19,    23,     0,    43,     0,
       0,     0,     0,     0,    48,    18,    21,     0,     0,     0,
       0,     0,     0,    35,    34,    36,    51,    46,    50,    16,
       0,     0,     0,     0,     0,     0,     0,     0,    22,    20,
       8,    24,     0,    44,    14,     0,    53,    54,    55,    56,
      57,    58,    59,     0,     0,    27,     0,    25,    52,     0,
       0,     0,    32,    31,    33,     0,    29,    26,    11,     0,
      28,    30
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -80,   -80,   -80,   -80,   -80,   -80,   -80,   -80,   -80,   -80,
      16,   -80,   -80,   -22,   -80,   -80,   -30,   -79,   -80,   -80,
      63,   -80,   -80,   -80,   -80,    30,   -49
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
       0,     1,    10,    34,    37,    17,    58,    31,    42,    54,
      55,    57,   106,   107,   109,   115,   116,    76,    28,    29,
      30,    72,    62,    52,    49,    50,    64
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int8 yytable[] =
{
      26,    95,     2,    97,    98,    99,   100,   101,   102,     3,
       4,    77,     5,    60,     6,    15,    67,    69,     7,    68,
      70,     8,    93,     9,    16,   108,   110,    61,    11,   111,
      12,    80,    81,    27,    82,    83,    84,    85,    86,    87,
      13,   119,    14,    18,   120,    73,    74,    75,   112,   113,
     114,    19,    20,    21,    22,    23,    24,    25,    32,    33,
      35,    36,    39,    41,    38,    40,    43,    45,    46,    44,
      26,    48,    53,    56,    51,    59,    63,    66,    71,    65,
      79,    88,    91,    89,    96,    92,    90,   104,   117,   121,
      94,    78,   103,   105,   118,     0,     0,     0,     0,     0,
       0,     0,     0,    47
};

static const yytype_int8 yycheck[] =
{
       3,    80,     0,    82,    83,    84,    85,    86,    87,     7,
       8,    60,    10,    18,    12,     3,    32,    32,    16,    35,
      35,    19,    71,    21,    13,   104,    32,    32,     9,    35,
      11,    23,    24,    36,    26,    27,    28,    29,    30,    31,
       9,    32,    11,     3,    35,     4,     5,     6,     4,     5,
       6,    17,     3,     3,     3,     3,    33,     3,     3,    33,
      33,    33,    17,    20,    22,    32,    34,     3,     3,    34,
       3,     3,     3,     3,    18,    37,     3,     3,    18,    33,
      33,     3,     3,    67,     4,    14,    33,    25,   110,   119,
      33,    61,    34,    34,    33,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    40
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    39,     0,     7,     8,    10,    12,    16,    19,    21,
      40,     9,    11,     9,    11,     3,    13,    43,     3,    17,
       3,     3,     3,     3,    33,     3,     3,    36,    56,    57,
      58,    45,     3,    33,    41,    33,    33,    42,    22,    17,
      32,    20,    46,    34,    34,     3,     3,    58,     3,    62,
      63,    18,    61,     3,    47,    48,     3,    49,    44,    37,
      18,    32,    60,     3,    64,    33,     3,    32,    35,    32,
      35,    18,    59,     4,     5,     6,    55,    64,    63,    33,
      23,    24,    26,    27,    28,    29,    30,    31,     3,    48,
      33,     3,    14,    64,    33,    55,     4,    55,    55,    55,
      55,    55,    55,    34,    25,    34,    50,    51,    55,    52,
      32,    35,     4,     5,     6,    53,    54,    51,    33,    32,
      35,    54
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    38,    39,    39,    40,    40,    40,    41,    40,    40,
      42,    40,    43,    44,    40,    45,    40,    46,    40,    47,
      47,    48,    48,    49,    49,    50,    50,    52,    51,    53,
      53,    54,    54,    54,    55,    55,    55,    56,    56,    57,
      57,    58,    58,    59,    59,    60,    60,    61,    61,    62,
      62,    63,    64,    64,    64,    64,    64,    64,    64,    64
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     0,     2,     4,     4,     3,     0,     8,     4,
       0,    12,     0,     0,     8,     0,     7,     0,     6,     1,
       3,     2,     3,     1,     3,     1,     3,     0,     4,     1,
       3,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       3,     1,     3,     0,     2,     0,     2,     0,     2,     1,
       3,     3,     5,     3,     3,     3,     3,     3,     3,     3
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
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

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


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




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
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
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
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
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
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






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
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
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


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

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
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
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
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
| yyreduce -- do a reduction.  |
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
  case 4: /* stmt: CREATE DATABASE IDENTIFIER ';'  */
#line 72 "module/sql_parser.y"
                                   { sql_push_create_database((yyvsp[-1].str)); free((yyvsp[-1].str)); }
#line 1233 "module/sql_parser.tab.cpp"
    break;

  case 5: /* stmt: DROP DATABASE IDENTIFIER ';'  */
#line 73 "module/sql_parser.y"
                                 { sql_push_drop_database((yyvsp[-1].str)); free((yyvsp[-1].str)); }
#line 1239 "module/sql_parser.tab.cpp"
    break;

  case 6: /* stmt: USE IDENTIFIER ';'  */
#line 74 "module/sql_parser.y"
                       { sql_push_use_database((yyvsp[-1].str)); free((yyvsp[-1].str)); }
#line 1245 "module/sql_parser.tab.cpp"
    break;

  case 7: /* $@1: %empty  */
#line 75 "module/sql_parser.y"
                            { sql_push_create_table_start((yyvsp[0].str)); free((yyvsp[0].str)); }
#line 1251 "module/sql_parser.tab.cpp"
    break;

  case 8: /* stmt: CREATE TABLE IDENTIFIER $@1 '(' column_list ')' ';'  */
#line 75 "module/sql_parser.y"
                                                                                                   { sql_push_create_table_end(); }
#line 1257 "module/sql_parser.tab.cpp"
    break;

  case 9: /* stmt: DROP TABLE IDENTIFIER ';'  */
#line 76 "module/sql_parser.y"
                              { sql_push_drop_table((yyvsp[-1].str)); free((yyvsp[-1].str)); }
#line 1263 "module/sql_parser.tab.cpp"
    break;

  case 10: /* $@2: %empty  */
#line 77 "module/sql_parser.y"
                           { sql_push_insert((yyvsp[0].str)); free((yyvsp[0].str)); }
#line 1269 "module/sql_parser.tab.cpp"
    break;

  case 11: /* stmt: INSERT INTO IDENTIFIER $@2 '(' id_list ')' VALUES '(' insert_row_list ')' ';'  */
#line 77 "module/sql_parser.y"
                                                                                                                 { sql_push_insert_finalize(); }
#line 1275 "module/sql_parser.tab.cpp"
    break;

  case 12: /* $@3: %empty  */
#line 78 "module/sql_parser.y"
           { sql_push_select_start(); }
#line 1281 "module/sql_parser.tab.cpp"
    break;

  case 13: /* $@4: %empty  */
#line 78 "module/sql_parser.y"
                                                                    { sql_select_set_table((yyvsp[0].str)); free((yyvsp[0].str)); }
#line 1287 "module/sql_parser.tab.cpp"
    break;

  case 14: /* stmt: SELECT $@3 select_list FROM IDENTIFIER $@4 select_where ';'  */
#line 78 "module/sql_parser.y"
                                                                                                                             { sql_push_select_finalize(); }
#line 1293 "module/sql_parser.tab.cpp"
    break;

  case 15: /* $@5: %empty  */
#line 79 "module/sql_parser.y"
                      { sql_push_update_start((yyvsp[0].str)); free((yyvsp[0].str)); }
#line 1299 "module/sql_parser.tab.cpp"
    break;

  case 16: /* stmt: UPDATE IDENTIFIER $@5 SET assignment_list update_where ';'  */
#line 79 "module/sql_parser.y"
                                                                                                    { sql_push_update_finalize(); }
#line 1305 "module/sql_parser.tab.cpp"
    break;

  case 17: /* $@6: %empty  */
#line 80 "module/sql_parser.y"
                           { sql_push_delete_start((yyvsp[0].str)); free((yyvsp[0].str)); }
#line 1311 "module/sql_parser.tab.cpp"
    break;

  case 18: /* stmt: DELETE FROM IDENTIFIER $@6 delete_where ';'  */
#line 80 "module/sql_parser.y"
                                                                                     { sql_push_delete_finalize(); }
#line 1317 "module/sql_parser.tab.cpp"
    break;

  case 21: /* column_def: IDENTIFIER IDENTIFIER  */
#line 89 "module/sql_parser.y"
                          { sql_push_create_table_column((yyvsp[-1].str), (yyvsp[0].str), 0, 0); free((yyvsp[-1].str)); free((yyvsp[0].str)); }
#line 1323 "module/sql_parser.tab.cpp"
    break;

  case 22: /* column_def: IDENTIFIER IDENTIFIER IDENTIFIER  */
#line 90 "module/sql_parser.y"
                                     { int notnull=0, indexed=0; if (strcasecmp((yyvsp[0].str), "NOT_NULL")==0) notnull=1; if (strcasecmp((yyvsp[0].str), "INDEXED")==0) indexed=1; sql_push_create_table_column((yyvsp[-2].str),(yyvsp[-1].str),notnull,indexed); free((yyvsp[-2].str)); free((yyvsp[-1].str)); free((yyvsp[0].str));}
#line 1329 "module/sql_parser.tab.cpp"
    break;

  case 23: /* id_list: IDENTIFIER  */
#line 94 "module/sql_parser.y"
               { sql_insert_add_column((yyvsp[0].str)); free((yyvsp[0].str)); }
#line 1335 "module/sql_parser.tab.cpp"
    break;

  case 24: /* id_list: id_list ',' IDENTIFIER  */
#line 95 "module/sql_parser.y"
                           { sql_insert_add_column((yyvsp[0].str)); free((yyvsp[0].str)); }
#line 1341 "module/sql_parser.tab.cpp"
    break;

  case 25: /* insert_row_list: insert_row  */
#line 99 "module/sql_parser.y"
               { }
#line 1347 "module/sql_parser.tab.cpp"
    break;

  case 26: /* insert_row_list: insert_row_list ',' insert_row  */
#line 100 "module/sql_parser.y"
                                   { }
#line 1353 "module/sql_parser.tab.cpp"
    break;

  case 27: /* $@7: %empty  */
#line 104 "module/sql_parser.y"
        { sql_insert_add_row_start(); }
#line 1359 "module/sql_parser.tab.cpp"
    break;

  case 28: /* insert_row: '(' $@7 value_list ')'  */
#line 104 "module/sql_parser.y"
                                                       { sql_insert_add_row_end(); }
#line 1365 "module/sql_parser.tab.cpp"
    break;

  case 31: /* insert_value: NUMBER  */
#line 113 "module/sql_parser.y"
           { sql_insert_add_row_value((yyvsp[0].str), 0, 0); free((yyvsp[0].str)); }
#line 1371 "module/sql_parser.tab.cpp"
    break;

  case 32: /* insert_value: STRING_LITERAL  */
#line 114 "module/sql_parser.y"
                   { sql_insert_add_row_value((yyvsp[0].str), 1, 0); free((yyvsp[0].str)); }
#line 1377 "module/sql_parser.tab.cpp"
    break;

  case 33: /* insert_value: NULL_TOKEN  */
#line 115 "module/sql_parser.y"
               { sql_insert_add_row_value(nullptr, 0, 1); }
#line 1383 "module/sql_parser.tab.cpp"
    break;

  case 34: /* value: NUMBER  */
#line 119 "module/sql_parser.y"
           { (yyval.str) = (yyvsp[0].str); }
#line 1389 "module/sql_parser.tab.cpp"
    break;

  case 35: /* value: STRING_LITERAL  */
#line 120 "module/sql_parser.y"
                   { (yyval.str) = (yyvsp[0].str); }
#line 1395 "module/sql_parser.tab.cpp"
    break;

  case 36: /* value: NULL_TOKEN  */
#line 121 "module/sql_parser.y"
               { (yyval.str) = nullptr; }
#line 1401 "module/sql_parser.tab.cpp"
    break;

  case 37: /* select_list: '*'  */
#line 125 "module/sql_parser.y"
        { sql_select_add_all(); }
#line 1407 "module/sql_parser.tab.cpp"
    break;

  case 41: /* select_item: IDENTIFIER  */
#line 135 "module/sql_parser.y"
               { sql_select_add_column((yyvsp[0].str), nullptr); free((yyvsp[0].str)); }
#line 1413 "module/sql_parser.tab.cpp"
    break;

  case 42: /* select_item: IDENTIFIER AS IDENTIFIER  */
#line 136 "module/sql_parser.y"
                             { sql_select_add_column((yyvsp[-2].str), (yyvsp[0].str)); free((yyvsp[-2].str)); free((yyvsp[0].str)); }
#line 1419 "module/sql_parser.tab.cpp"
    break;

  case 44: /* select_where: WHERE cond  */
#line 141 "module/sql_parser.y"
               { sql_select_set_where((yyvsp[0].cond)->left.c_str(), (yyvsp[0].cond)->op.c_str(), (yyvsp[0].cond)->right.c_str(), (yyvsp[0].cond)->right2.c_str()); delete (yyvsp[0].cond); }
#line 1425 "module/sql_parser.tab.cpp"
    break;

  case 46: /* update_where: WHERE cond  */
#line 146 "module/sql_parser.y"
               { sql_update_set_where((yyvsp[0].cond)->left.c_str(), (yyvsp[0].cond)->op.c_str(), (yyvsp[0].cond)->right.c_str(), (yyvsp[0].cond)->right2.c_str()); delete (yyvsp[0].cond); }
#line 1431 "module/sql_parser.tab.cpp"
    break;

  case 48: /* delete_where: WHERE cond  */
#line 151 "module/sql_parser.y"
               { sql_delete_set_where((yyvsp[0].cond)->left.c_str(), (yyvsp[0].cond)->op.c_str(), (yyvsp[0].cond)->right.c_str(), (yyvsp[0].cond)->right2.c_str()); delete (yyvsp[0].cond); }
#line 1437 "module/sql_parser.tab.cpp"
    break;

  case 51: /* assignment: IDENTIFIER '=' value  */
#line 160 "module/sql_parser.y"
                         { sql_update_add_assignment((yyvsp[-2].str), (yyvsp[0].str)); free((yyvsp[-2].str)); if((yyvsp[0].str)) free((yyvsp[0].str)); }
#line 1443 "module/sql_parser.tab.cpp"
    break;

  case 52: /* cond: IDENTIFIER BETWEEN value AND value  */
#line 164 "module/sql_parser.y"
                                       { (yyval.cond) = new Condition(); (yyval.cond)->left = (yyvsp[-4].str); (yyval.cond)->op = "BETWEEN"; (yyval.cond)->right = (yyvsp[-2].str) ? (yyvsp[-2].str) : ""; (yyval.cond)->right2 = (yyvsp[0].str) ? (yyvsp[0].str) : ""; free((yyvsp[-4].str)); if ((yyvsp[-2].str)) free((yyvsp[-2].str)); if ((yyvsp[0].str)) free((yyvsp[0].str)); }
#line 1449 "module/sql_parser.tab.cpp"
    break;

  case 53: /* cond: IDENTIFIER LIKE STRING_LITERAL  */
#line 165 "module/sql_parser.y"
                                   { (yyval.cond) = new Condition(); (yyval.cond)->left = (yyvsp[-2].str); (yyval.cond)->op = "LIKE"; (yyval.cond)->right = (yyvsp[0].str) ? (yyvsp[0].str) : ""; (yyval.cond)->right2 = ""; free((yyvsp[-2].str)); free((yyvsp[0].str)); }
#line 1455 "module/sql_parser.tab.cpp"
    break;

  case 54: /* cond: IDENTIFIER EQ value  */
#line 166 "module/sql_parser.y"
                        { (yyval.cond) = new Condition(); (yyval.cond)->left = (yyvsp[-2].str); (yyval.cond)->op = "=="; (yyval.cond)->right = (yyvsp[0].str) ? (yyvsp[0].str) : ""; (yyval.cond)->right2 = ""; free((yyvsp[-2].str)); if ((yyvsp[0].str)) free((yyvsp[0].str)); }
#line 1461 "module/sql_parser.tab.cpp"
    break;

  case 55: /* cond: IDENTIFIER NEQ value  */
#line 167 "module/sql_parser.y"
                         { (yyval.cond) = new Condition(); (yyval.cond)->left = (yyvsp[-2].str); (yyval.cond)->op = "!="; (yyval.cond)->right = (yyvsp[0].str) ? (yyvsp[0].str) : ""; (yyval.cond)->right2 = ""; free((yyvsp[-2].str)); if ((yyvsp[0].str)) free((yyvsp[0].str)); }
#line 1467 "module/sql_parser.tab.cpp"
    break;

  case 56: /* cond: IDENTIFIER LE value  */
#line 168 "module/sql_parser.y"
                        { (yyval.cond) = new Condition(); (yyval.cond)->left = (yyvsp[-2].str); (yyval.cond)->op = "<="; (yyval.cond)->right = (yyvsp[0].str) ? (yyvsp[0].str) : ""; (yyval.cond)->right2 = ""; free((yyvsp[-2].str)); if ((yyvsp[0].str)) free((yyvsp[0].str)); }
#line 1473 "module/sql_parser.tab.cpp"
    break;

  case 57: /* cond: IDENTIFIER GE value  */
#line 169 "module/sql_parser.y"
                        { (yyval.cond) = new Condition(); (yyval.cond)->left = (yyvsp[-2].str); (yyval.cond)->op = ">="; (yyval.cond)->right = (yyvsp[0].str) ? (yyvsp[0].str) : ""; (yyval.cond)->right2 = ""; free((yyvsp[-2].str)); if ((yyvsp[0].str)) free((yyvsp[0].str)); }
#line 1479 "module/sql_parser.tab.cpp"
    break;

  case 58: /* cond: IDENTIFIER LT value  */
#line 170 "module/sql_parser.y"
                        { (yyval.cond) = new Condition(); (yyval.cond)->left = (yyvsp[-2].str); (yyval.cond)->op = "<"; (yyval.cond)->right = (yyvsp[0].str) ? (yyvsp[0].str) : ""; (yyval.cond)->right2 = ""; free((yyvsp[-2].str)); if ((yyvsp[0].str)) free((yyvsp[0].str)); }
#line 1485 "module/sql_parser.tab.cpp"
    break;

  case 59: /* cond: IDENTIFIER GT value  */
#line 171 "module/sql_parser.y"
                        { (yyval.cond) = new Condition(); (yyval.cond)->left = (yyvsp[-2].str); (yyval.cond)->op = ">"; (yyval.cond)->right = (yyvsp[0].str) ? (yyvsp[0].str) : ""; (yyval.cond)->right2 = ""; free((yyvsp[-2].str)); if ((yyvsp[0].str)) free((yyvsp[0].str)); }
#line 1491 "module/sql_parser.tab.cpp"
    break;


#line 1495 "module/sql_parser.tab.cpp"

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
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
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
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

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

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
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
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
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
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 174 "module/sql_parser.y"


void yyerror(const char *s) {
    fprintf(stderr, "Parse error: %s\n", s);
}
