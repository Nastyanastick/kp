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

#line 141 "module/sql_parser.tab.cpp"

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
  YYSYMBOL_SELECT = 15,                    /* SELECT  */
  YYSYMBOL_FROM = 16,                      /* FROM  */
  YYSYMBOL_WHERE = 17,                     /* WHERE  */
  YYSYMBOL_UPDATE = 18,                    /* UPDATE  */
  YYSYMBOL_SET = 19,                       /* SET  */
  YYSYMBOL_DELETE = 20,                    /* DELETE  */
  YYSYMBOL_AS = 21,                        /* AS  */
  YYSYMBOL_BETWEEN = 22,                   /* BETWEEN  */
  YYSYMBOL_LIKE = 23,                      /* LIKE  */
  YYSYMBOL_SUM = 24,                       /* SUM  */
  YYSYMBOL_COUNT = 25,                     /* COUNT  */
  YYSYMBOL_AVG = 26,                       /* AVG  */
  YYSYMBOL_AND = 27,                       /* AND  */
  YYSYMBOL_OR = 28,                        /* OR  */
  YYSYMBOL_EQ = 29,                        /* EQ  */
  YYSYMBOL_NEQ = 30,                       /* NEQ  */
  YYSYMBOL_LE = 31,                        /* LE  */
  YYSYMBOL_GE = 32,                        /* GE  */
  YYSYMBOL_LT = 33,                        /* LT  */
  YYSYMBOL_GT = 34,                        /* GT  */
  YYSYMBOL_35_ = 35,                       /* ';'  */
  YYSYMBOL_36_ = 36,                       /* '('  */
  YYSYMBOL_37_ = 37,                       /* ')'  */
  YYSYMBOL_38_ = 38,                       /* ','  */
  YYSYMBOL_39_ = 39,                       /* '*'  */
  YYSYMBOL_40_ = 40,                       /* '='  */
  YYSYMBOL_YYACCEPT = 41,                  /* $accept  */
  YYSYMBOL_commands = 42,                  /* commands  */
  YYSYMBOL_stmt = 43,                      /* stmt  */
  YYSYMBOL_44_1 = 44,                      /* $@1  */
  YYSYMBOL_45_2 = 45,                      /* $@2  */
  YYSYMBOL_46_3 = 46,                      /* $@3  */
  YYSYMBOL_47_4 = 47,                      /* $@4  */
  YYSYMBOL_48_5 = 48,                      /* $@5  */
  YYSYMBOL_49_6 = 49,                      /* $@6  */
  YYSYMBOL_50_7 = 50,                      /* $@7  */
  YYSYMBOL_column_list = 51,               /* column_list  */
  YYSYMBOL_column_def = 52,                /* column_def  */
  YYSYMBOL_id_list = 53,                   /* id_list  */
  YYSYMBOL_insert_row_list = 54,           /* insert_row_list  */
  YYSYMBOL_insert_row = 55,                /* insert_row  */
  YYSYMBOL_56_8 = 56,                      /* $@8  */
  YYSYMBOL_insert_value_list = 57,         /* insert_value_list  */
  YYSYMBOL_insert_value = 58,              /* insert_value  */
  YYSYMBOL_compare_value = 59,             /* compare_value  */
  YYSYMBOL_assign_value = 60,              /* assign_value  */
  YYSYMBOL_select_list = 61,               /* select_list  */
  YYSYMBOL_select_items = 62,              /* select_items  */
  YYSYMBOL_select_item = 63,               /* select_item  */
  YYSYMBOL_select_where = 64,              /* select_where  */
  YYSYMBOL_update_where = 65,              /* update_where  */
  YYSYMBOL_delete_where = 66,              /* delete_where  */
  YYSYMBOL_assignment_list = 67,           /* assignment_list  */
  YYSYMBOL_assignment = 68,                /* assignment  */
  YYSYMBOL_cond = 69,                      /* cond  */
  YYSYMBOL_or_expr = 70,                   /* or_expr  */
  YYSYMBOL_and_expr = 71,                  /* and_expr  */
  YYSYMBOL_primary_expr = 72,              /* primary_expr  */
  YYSYMBOL_comparison_expr = 73            /* comparison_expr  */
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
typedef yytype_uint8 yy_state_t;

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
#define YYLAST   146

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  41
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  33
/* YYNRULES -- Number of rules.  */
#define YYNRULES  79
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  160

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   289


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
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
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
  "USE", "TABLE", "INSERT", "INTO", "VALUES", "SELECT", "FROM", "WHERE",
  "UPDATE", "SET", "DELETE", "AS", "BETWEEN", "LIKE", "SUM", "COUNT",
  "AVG", "AND", "OR", "EQ", "NEQ", "LE", "GE", "LT", "GT", "';'", "'('",
  "')'", "','", "'*'", "'='", "$accept", "commands", "stmt", "$@1", "$@2",
  "$@3", "$@4", "$@5", "$@6", "$@7", "column_list", "column_def",
  "id_list", "insert_row_list", "insert_row", "$@8", "insert_value_list",
  "insert_value", "compare_value", "assign_value", "select_list",
  "select_items", "select_item", "select_where", "update_where",
  "delete_where", "assignment_list", "assignment", "cond", "or_expr",
  "and_expr", "primary_expr", "comparison_expr", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-113)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-13)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
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

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
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

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
    -113,  -113,  -113,  -113,  -113,  -113,  -113,  -113,  -113,  -113,
    -113,    30,  -113,    -1,    31,  -113,  -113,   -29,  -112,  -113,
    -113,  -113,    99,  -113,  -113,  -113,  -113,    50,   -68,    51,
       9,    11,  -113
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,     1,    10,    37,    40,    41,    17,    77,    34,    49,
      66,    67,    69,    71,    72,    95,   130,   131,   142,   107,
      31,    32,    33,   103,    81,    64,    61,    62,    84,    85,
      86,    87,    88
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
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

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
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

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
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

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
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
#line 107 "module/sql_parser.y"
        { sql_push_create_database((yyvsp[-1].str)); free((yyvsp[-1].str)); }
#line 1300 "module/sql_parser.tab.cpp"
    break;

  case 5: /* stmt: DROP DATABASE IDENTIFIER ';'  */
#line 110 "module/sql_parser.y"
        { sql_push_drop_database((yyvsp[-1].str)); free((yyvsp[-1].str)); }
#line 1306 "module/sql_parser.tab.cpp"
    break;

  case 6: /* stmt: USE IDENTIFIER ';'  */
#line 113 "module/sql_parser.y"
        { sql_push_use_database((yyvsp[-1].str)); free((yyvsp[-1].str)); }
#line 1312 "module/sql_parser.tab.cpp"
    break;

  case 7: /* $@1: %empty  */
#line 116 "module/sql_parser.y"
        { sql_push_create_table_start((yyvsp[0].str)); free((yyvsp[0].str)); }
#line 1318 "module/sql_parser.tab.cpp"
    break;

  case 8: /* stmt: CREATE TABLE IDENTIFIER $@1 '(' column_list ')' ';'  */
#line 118 "module/sql_parser.y"
        { sql_push_create_table_end(); }
#line 1324 "module/sql_parser.tab.cpp"
    break;

  case 9: /* stmt: DROP TABLE IDENTIFIER ';'  */
#line 121 "module/sql_parser.y"
        { sql_push_drop_table((yyvsp[-1].str)); free((yyvsp[-1].str)); }
#line 1330 "module/sql_parser.tab.cpp"
    break;

  case 10: /* $@2: %empty  */
#line 124 "module/sql_parser.y"
        { sql_push_insert((yyvsp[0].str)); free((yyvsp[0].str)); }
#line 1336 "module/sql_parser.tab.cpp"
    break;

  case 11: /* stmt: INSERT INTO IDENTIFIER $@2 '(' id_list ')' VALUES insert_row_list ';'  */
#line 126 "module/sql_parser.y"
        { sql_push_insert_finalize(); }
#line 1342 "module/sql_parser.tab.cpp"
    break;

  case 12: /* $@3: %empty  */
#line 129 "module/sql_parser.y"
        { sql_push_insert((yyvsp[0].str)); free((yyvsp[0].str)); }
#line 1348 "module/sql_parser.tab.cpp"
    break;

  case 13: /* stmt: INSERT INTO IDENTIFIER $@3 VALUES insert_row_list ';'  */
#line 131 "module/sql_parser.y"
        { sql_push_insert_finalize(); }
#line 1354 "module/sql_parser.tab.cpp"
    break;

  case 14: /* $@4: %empty  */
#line 134 "module/sql_parser.y"
        { sql_push_select_start(); }
#line 1360 "module/sql_parser.tab.cpp"
    break;

  case 15: /* $@5: %empty  */
#line 136 "module/sql_parser.y"
        { sql_select_set_table((yyvsp[0].str)); free((yyvsp[0].str)); }
#line 1366 "module/sql_parser.tab.cpp"
    break;

  case 16: /* stmt: SELECT $@4 select_list FROM IDENTIFIER $@5 select_where ';'  */
#line 138 "module/sql_parser.y"
        { sql_push_select_finalize(); }
#line 1372 "module/sql_parser.tab.cpp"
    break;

  case 17: /* $@6: %empty  */
#line 141 "module/sql_parser.y"
        { sql_push_update_start((yyvsp[0].str)); free((yyvsp[0].str)); }
#line 1378 "module/sql_parser.tab.cpp"
    break;

  case 18: /* stmt: UPDATE IDENTIFIER $@6 SET assignment_list update_where ';'  */
#line 143 "module/sql_parser.y"
        { sql_push_update_finalize(); }
#line 1384 "module/sql_parser.tab.cpp"
    break;

  case 19: /* $@7: %empty  */
#line 146 "module/sql_parser.y"
        { sql_push_delete_start((yyvsp[0].str)); free((yyvsp[0].str)); }
#line 1390 "module/sql_parser.tab.cpp"
    break;

  case 20: /* stmt: DELETE FROM IDENTIFIER $@7 delete_where ';'  */
#line 148 "module/sql_parser.y"
        { sql_push_delete_finalize(); }
#line 1396 "module/sql_parser.tab.cpp"
    break;

  case 23: /* column_def: IDENTIFIER IDENTIFIER  */
#line 158 "module/sql_parser.y"
        {
            sql_push_create_table_column((yyvsp[-1].str), (yyvsp[0].str), 0, 0);
            free((yyvsp[-1].str)); free((yyvsp[0].str));
        }
#line 1405 "module/sql_parser.tab.cpp"
    break;

  case 24: /* column_def: IDENTIFIER IDENTIFIER IDENTIFIER  */
#line 164 "module/sql_parser.y"
        {
            int notnull = 0;
            int indexed = 0;

            if (iequals((yyvsp[0].str), "NOT_NULL")) notnull = 1;
            else if (iequals((yyvsp[0].str), "INDEXED")) indexed = 1;

            sql_push_create_table_column((yyvsp[-2].str), (yyvsp[-1].str), notnull, indexed);
            free((yyvsp[-2].str)); free((yyvsp[-1].str)); free((yyvsp[0].str));
        }
#line 1420 "module/sql_parser.tab.cpp"
    break;

  case 25: /* id_list: IDENTIFIER  */
#line 178 "module/sql_parser.y"
        { sql_insert_add_column((yyvsp[0].str)); free((yyvsp[0].str)); }
#line 1426 "module/sql_parser.tab.cpp"
    break;

  case 26: /* id_list: id_list ',' IDENTIFIER  */
#line 181 "module/sql_parser.y"
        { sql_insert_add_column((yyvsp[0].str)); free((yyvsp[0].str)); }
#line 1432 "module/sql_parser.tab.cpp"
    break;

  case 29: /* $@8: %empty  */
#line 191 "module/sql_parser.y"
        { sql_insert_add_row_start(); }
#line 1438 "module/sql_parser.tab.cpp"
    break;

  case 30: /* insert_row: '(' $@8 insert_value_list ')'  */
#line 193 "module/sql_parser.y"
        { sql_insert_add_row_end(); }
#line 1444 "module/sql_parser.tab.cpp"
    break;

  case 33: /* insert_value: NUMBER  */
#line 203 "module/sql_parser.y"
        {
            sql_insert_add_row_value((yyvsp[0].str), 0, 0);
            free((yyvsp[0].str));
        }
#line 1453 "module/sql_parser.tab.cpp"
    break;

  case 34: /* insert_value: STRING_LITERAL  */
#line 209 "module/sql_parser.y"
        {
            sql_insert_add_row_value((yyvsp[0].str), 1, 0);
            free((yyvsp[0].str));
        }
#line 1462 "module/sql_parser.tab.cpp"
    break;

  case 35: /* insert_value: NULL_TOKEN  */
#line 215 "module/sql_parser.y"
        {
            sql_insert_add_row_value(nullptr, 0, 1);
        }
#line 1470 "module/sql_parser.tab.cpp"
    break;

  case 36: /* compare_value: NUMBER  */
#line 222 "module/sql_parser.y"
        { (yyval.str) = (yyvsp[0].str); }
#line 1476 "module/sql_parser.tab.cpp"
    break;

  case 37: /* compare_value: STRING_LITERAL  */
#line 225 "module/sql_parser.y"
        {
            (yyval.str) = quote_string((yyvsp[0].str));
            free((yyvsp[0].str));
        }
#line 1485 "module/sql_parser.tab.cpp"
    break;

  case 38: /* compare_value: NULL_TOKEN  */
#line 231 "module/sql_parser.y"
        { (yyval.str) = strdup("NULL"); }
#line 1491 "module/sql_parser.tab.cpp"
    break;

  case 39: /* assign_value: NUMBER  */
#line 236 "module/sql_parser.y"
        { (yyval.str) = (yyvsp[0].str); }
#line 1497 "module/sql_parser.tab.cpp"
    break;

  case 40: /* assign_value: STRING_LITERAL  */
#line 239 "module/sql_parser.y"
        { (yyval.str) = (yyvsp[0].str); }
#line 1503 "module/sql_parser.tab.cpp"
    break;

  case 41: /* assign_value: NULL_TOKEN  */
#line 242 "module/sql_parser.y"
        { (yyval.str) = strdup("NULL"); }
#line 1509 "module/sql_parser.tab.cpp"
    break;

  case 42: /* select_list: '*'  */
#line 247 "module/sql_parser.y"
        { sql_select_add_all(); }
#line 1515 "module/sql_parser.tab.cpp"
    break;

  case 46: /* select_item: IDENTIFIER  */
#line 259 "module/sql_parser.y"
        { sql_select_add_column((yyvsp[0].str), nullptr); free((yyvsp[0].str)); }
#line 1521 "module/sql_parser.tab.cpp"
    break;

  case 47: /* select_item: IDENTIFIER AS IDENTIFIER  */
#line 262 "module/sql_parser.y"
        { sql_select_add_column((yyvsp[-2].str), (yyvsp[0].str)); free((yyvsp[-2].str)); free((yyvsp[0].str)); }
#line 1527 "module/sql_parser.tab.cpp"
    break;

  case 48: /* select_item: SUM '(' IDENTIFIER ')'  */
#line 265 "module/sql_parser.y"
        { sql_select_add_aggregate("SUM", (yyvsp[-1].str), nullptr); free((yyvsp[-1].str)); }
#line 1533 "module/sql_parser.tab.cpp"
    break;

  case 49: /* select_item: SUM '(' IDENTIFIER ')' AS IDENTIFIER  */
#line 268 "module/sql_parser.y"
        { sql_select_add_aggregate("SUM", (yyvsp[-3].str), (yyvsp[0].str)); free((yyvsp[-3].str)); free((yyvsp[0].str)); }
#line 1539 "module/sql_parser.tab.cpp"
    break;

  case 50: /* select_item: COUNT '(' '*' ')'  */
#line 271 "module/sql_parser.y"
        { sql_select_add_aggregate("COUNT", "*", nullptr); }
#line 1545 "module/sql_parser.tab.cpp"
    break;

  case 51: /* select_item: COUNT '(' '*' ')' AS IDENTIFIER  */
#line 274 "module/sql_parser.y"
        { sql_select_add_aggregate("COUNT", "*", (yyvsp[0].str)); free((yyvsp[0].str)); }
#line 1551 "module/sql_parser.tab.cpp"
    break;

  case 52: /* select_item: COUNT '(' IDENTIFIER ')'  */
#line 277 "module/sql_parser.y"
        { sql_select_add_aggregate("COUNT", (yyvsp[-1].str), nullptr); free((yyvsp[-1].str)); }
#line 1557 "module/sql_parser.tab.cpp"
    break;

  case 53: /* select_item: COUNT '(' IDENTIFIER ')' AS IDENTIFIER  */
#line 280 "module/sql_parser.y"
        { sql_select_add_aggregate("COUNT", (yyvsp[-3].str), (yyvsp[0].str)); free((yyvsp[-3].str)); free((yyvsp[0].str)); }
#line 1563 "module/sql_parser.tab.cpp"
    break;

  case 54: /* select_item: AVG '(' IDENTIFIER ')'  */
#line 283 "module/sql_parser.y"
        { sql_select_add_aggregate("AVG", (yyvsp[-1].str), nullptr); free((yyvsp[-1].str)); }
#line 1569 "module/sql_parser.tab.cpp"
    break;

  case 55: /* select_item: AVG '(' IDENTIFIER ')' AS IDENTIFIER  */
#line 286 "module/sql_parser.y"
        { sql_select_add_aggregate("AVG", (yyvsp[-3].str), (yyvsp[0].str)); free((yyvsp[-3].str)); free((yyvsp[0].str)); }
#line 1575 "module/sql_parser.tab.cpp"
    break;

  case 56: /* select_where: %empty  */
#line 291 "module/sql_parser.y"
        { sql_select_set_where(nullptr); }
#line 1581 "module/sql_parser.tab.cpp"
    break;

  case 57: /* select_where: WHERE cond  */
#line 294 "module/sql_parser.y"
        { sql_select_set_where((yyvsp[0].cond)); }
#line 1587 "module/sql_parser.tab.cpp"
    break;

  case 58: /* update_where: %empty  */
#line 299 "module/sql_parser.y"
        { sql_update_set_where(nullptr); }
#line 1593 "module/sql_parser.tab.cpp"
    break;

  case 59: /* update_where: WHERE cond  */
#line 302 "module/sql_parser.y"
        { sql_update_set_where((yyvsp[0].cond)); }
#line 1599 "module/sql_parser.tab.cpp"
    break;

  case 60: /* delete_where: %empty  */
#line 307 "module/sql_parser.y"
        { sql_delete_set_where(nullptr); }
#line 1605 "module/sql_parser.tab.cpp"
    break;

  case 61: /* delete_where: WHERE cond  */
#line 310 "module/sql_parser.y"
        { sql_delete_set_where((yyvsp[0].cond)); }
#line 1611 "module/sql_parser.tab.cpp"
    break;

  case 64: /* assignment: IDENTIFIER '=' assign_value  */
#line 320 "module/sql_parser.y"
        {
            sql_update_add_assignment((yyvsp[-2].str), (yyvsp[0].str));
            free((yyvsp[-2].str));
            if ((yyvsp[0].str)) free((yyvsp[0].str));
        }
#line 1621 "module/sql_parser.tab.cpp"
    break;

  case 65: /* cond: or_expr  */
#line 329 "module/sql_parser.y"
        { (yyval.cond) = (yyvsp[0].cond); }
#line 1627 "module/sql_parser.tab.cpp"
    break;

  case 66: /* or_expr: and_expr  */
#line 334 "module/sql_parser.y"
        { (yyval.cond) = (yyvsp[0].cond); }
#line 1633 "module/sql_parser.tab.cpp"
    break;

  case 67: /* or_expr: or_expr OR and_expr  */
#line 337 "module/sql_parser.y"
        {
            (yyval.cond) = new ConditionNode(ConditionNode::OR);
            (yyval.cond)->leftChild = (yyvsp[-2].cond);
            (yyval.cond)->rightChild = (yyvsp[0].cond);
        }
#line 1643 "module/sql_parser.tab.cpp"
    break;

  case 68: /* and_expr: primary_expr  */
#line 346 "module/sql_parser.y"
        { (yyval.cond) = (yyvsp[0].cond); }
#line 1649 "module/sql_parser.tab.cpp"
    break;

  case 69: /* and_expr: and_expr AND primary_expr  */
#line 349 "module/sql_parser.y"
        {
            (yyval.cond) = new ConditionNode(ConditionNode::AND);
            (yyval.cond)->leftChild = (yyvsp[-2].cond);
            (yyval.cond)->rightChild = (yyvsp[0].cond);
        }
#line 1659 "module/sql_parser.tab.cpp"
    break;

  case 70: /* primary_expr: comparison_expr  */
#line 358 "module/sql_parser.y"
        { (yyval.cond) = (yyvsp[0].cond); }
#line 1665 "module/sql_parser.tab.cpp"
    break;

  case 71: /* primary_expr: '(' or_expr ')'  */
#line 361 "module/sql_parser.y"
        { (yyval.cond) = (yyvsp[-1].cond); }
#line 1671 "module/sql_parser.tab.cpp"
    break;

  case 72: /* comparison_expr: IDENTIFIER BETWEEN compare_value AND compare_value  */
#line 366 "module/sql_parser.y"
        {
            (yyval.cond) = new ConditionNode((yyvsp[-4].str), "BETWEEN", (yyvsp[-2].str) ? (yyvsp[-2].str) : "", (yyvsp[0].str) ? (yyvsp[0].str) : "");
            free((yyvsp[-4].str));
            if ((yyvsp[-2].str)) free((yyvsp[-2].str));
            if ((yyvsp[0].str)) free((yyvsp[0].str));
        }
#line 1682 "module/sql_parser.tab.cpp"
    break;

  case 73: /* comparison_expr: IDENTIFIER LIKE compare_value  */
#line 374 "module/sql_parser.y"
        {
            (yyval.cond) = new ConditionNode((yyvsp[-2].str), "LIKE", (yyvsp[0].str) ? (yyvsp[0].str) : "");
            free((yyvsp[-2].str));
            if ((yyvsp[0].str)) free((yyvsp[0].str));
        }
#line 1692 "module/sql_parser.tab.cpp"
    break;

  case 74: /* comparison_expr: IDENTIFIER EQ compare_value  */
#line 381 "module/sql_parser.y"
        {
            (yyval.cond) = new ConditionNode((yyvsp[-2].str), "==", (yyvsp[0].str) ? (yyvsp[0].str) : "");
            free((yyvsp[-2].str));
            if ((yyvsp[0].str)) free((yyvsp[0].str));
        }
#line 1702 "module/sql_parser.tab.cpp"
    break;

  case 75: /* comparison_expr: IDENTIFIER NEQ compare_value  */
#line 388 "module/sql_parser.y"
        {
            (yyval.cond) = new ConditionNode((yyvsp[-2].str), "!=", (yyvsp[0].str) ? (yyvsp[0].str) : "");
            free((yyvsp[-2].str));
            if ((yyvsp[0].str)) free((yyvsp[0].str));
        }
#line 1712 "module/sql_parser.tab.cpp"
    break;

  case 76: /* comparison_expr: IDENTIFIER LE compare_value  */
#line 395 "module/sql_parser.y"
        {
            (yyval.cond) = new ConditionNode((yyvsp[-2].str), "<=", (yyvsp[0].str) ? (yyvsp[0].str) : "");
            free((yyvsp[-2].str));
            if ((yyvsp[0].str)) free((yyvsp[0].str));
        }
#line 1722 "module/sql_parser.tab.cpp"
    break;

  case 77: /* comparison_expr: IDENTIFIER GE compare_value  */
#line 402 "module/sql_parser.y"
        {
            (yyval.cond) = new ConditionNode((yyvsp[-2].str), ">=", (yyvsp[0].str) ? (yyvsp[0].str) : "");
            free((yyvsp[-2].str));
            if ((yyvsp[0].str)) free((yyvsp[0].str));
        }
#line 1732 "module/sql_parser.tab.cpp"
    break;

  case 78: /* comparison_expr: IDENTIFIER LT compare_value  */
#line 409 "module/sql_parser.y"
        {
            (yyval.cond) = new ConditionNode((yyvsp[-2].str), "<", (yyvsp[0].str) ? (yyvsp[0].str) : "");
            free((yyvsp[-2].str));
            if ((yyvsp[0].str)) free((yyvsp[0].str));
        }
#line 1742 "module/sql_parser.tab.cpp"
    break;

  case 79: /* comparison_expr: IDENTIFIER GT compare_value  */
#line 416 "module/sql_parser.y"
        {
            (yyval.cond) = new ConditionNode((yyvsp[-2].str), ">", (yyvsp[0].str) ? (yyvsp[0].str) : "");
            free((yyvsp[-2].str));
            if ((yyvsp[0].str)) free((yyvsp[0].str));
        }
#line 1752 "module/sql_parser.tab.cpp"
    break;


#line 1756 "module/sql_parser.tab.cpp"

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
