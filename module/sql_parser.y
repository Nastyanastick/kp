%{
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
%}

%code requires {
#include "table.h"
}

%union {
    char* str;
    Condition* cond;
}

%token <str> IDENTIFIER
%token <str> STRING_LITERAL
%token <str> NUMBER
%token NULL_TOKEN

%token CREATE DROP DATABASE USE TABLE INSERT INTO VALUES VALUE SELECT FROM WHERE UPDATE SET DELETE AS BETWEEN LIKE
%token AND

%token EQ NEQ LE GE LT GT

%left ','

%type <cond> cond
%type <str> value

%%

commands:
    /* empty */
  | commands stmt
  ;

stmt:
    CREATE DATABASE IDENTIFIER ';' { sql_push_create_database($3); free($3); }
  | DROP DATABASE IDENTIFIER ';' { sql_push_drop_database($3); free($3); }
  | USE IDENTIFIER ';' { sql_push_use_database($2); free($2); }
  | CREATE TABLE IDENTIFIER { sql_push_create_table_start($3); free($3); } '(' column_list ')' ';' { sql_push_create_table_end(); }
  | DROP TABLE IDENTIFIER ';' { sql_push_drop_table($3); free($3); }
  | INSERT INTO IDENTIFIER { sql_push_insert($3); free($3); } '(' id_list ')' VALUES '(' insert_row_list ')' ';' { sql_push_insert_finalize(); }
  | SELECT { sql_push_select_start(); } select_list FROM IDENTIFIER { sql_select_set_table($5); free($5); } select_where ';' { sql_push_select_finalize(); }
  | UPDATE IDENTIFIER { sql_push_update_start($2); free($2); } SET assignment_list update_where ';' { sql_push_update_finalize(); }
  | DELETE FROM IDENTIFIER { sql_push_delete_start($3); free($3); } delete_where ';' { sql_push_delete_finalize(); }
  ;

column_list:
    column_def
  | column_list ',' column_def
  ;

column_def:
    IDENTIFIER IDENTIFIER { sql_push_create_table_column($1, $2, 0, 0); free($1); free($2); }
  | IDENTIFIER IDENTIFIER IDENTIFIER { int notnull=0, indexed=0; if (strcasecmp($3, "NOT_NULL")==0) notnull=1; if (strcasecmp($3, "INDEXED")==0) indexed=1; sql_push_create_table_column($1,$2,notnull,indexed); free($1); free($2); free($3);} 
  ;

id_list:
    IDENTIFIER { sql_insert_add_column($1); free($1); }
  | id_list ',' IDENTIFIER { sql_insert_add_column($3); free($3); }
  ;

insert_row_list:
    insert_row { }
  | insert_row_list ',' insert_row { }
  ;

insert_row:
    '(' { sql_insert_add_row_start(); } value_list ')' { sql_insert_add_row_end(); }
  ;

value_list:
    insert_value
  | value_list ',' insert_value
  ;

insert_value:
    NUMBER { sql_insert_add_row_value($1, 0, 0); free($1); }
  | STRING_LITERAL { sql_insert_add_row_value($1, 1, 0); free($1); }
  | NULL_TOKEN { sql_insert_add_row_value(nullptr, 0, 1); }
  ;

value:
    NUMBER { $$ = $1; }
  | STRING_LITERAL { $$ = $1; }
  | NULL_TOKEN { $$ = nullptr; }
  ;

select_list:
    '*' { sql_select_add_all(); }
  | select_items
  ;

select_items:
    select_item
  | select_items ',' select_item
  ;

select_item:
    IDENTIFIER { sql_select_add_column($1, nullptr); free($1); }
  | IDENTIFIER AS IDENTIFIER { sql_select_add_column($1, $3); free($1); free($3); }
  ;

select_where:
    /* empty */
  | WHERE cond { sql_select_set_where($2->left.c_str(), $2->op.c_str(), $2->right.c_str(), $2->right2.c_str()); delete $2; }
  ;

update_where:
    /* empty */
  | WHERE cond { sql_update_set_where($2->left.c_str(), $2->op.c_str(), $2->right.c_str(), $2->right2.c_str()); delete $2; }
  ;

delete_where:
    /* empty */
  | WHERE cond { sql_delete_set_where($2->left.c_str(), $2->op.c_str(), $2->right.c_str(), $2->right2.c_str()); delete $2; }
  ;

assignment_list:
    assignment
  | assignment_list ',' assignment
  ;

assignment:
    IDENTIFIER '=' value { sql_update_add_assignment($1, $3); free($1); if($3) free($3); }
  ;

cond:
    IDENTIFIER BETWEEN value AND value { $$ = new Condition(); $$->left = $1; $$->op = "BETWEEN"; $$->right = $3 ? $3 : ""; $$->right2 = $5 ? $5 : ""; free($1); if ($3) free($3); if ($5) free($5); }
  | IDENTIFIER LIKE STRING_LITERAL { $$ = new Condition(); $$->left = $1; $$->op = "LIKE"; $$->right = $3 ? $3 : ""; $$->right2 = ""; free($1); free($3); }
  | IDENTIFIER EQ value { $$ = new Condition(); $$->left = $1; $$->op = "=="; $$->right = $3 ? $3 : ""; $$->right2 = ""; free($1); if ($3) free($3); }
  | IDENTIFIER NEQ value { $$ = new Condition(); $$->left = $1; $$->op = "!="; $$->right = $3 ? $3 : ""; $$->right2 = ""; free($1); if ($3) free($3); }
  | IDENTIFIER LE value { $$ = new Condition(); $$->left = $1; $$->op = "<="; $$->right = $3 ? $3 : ""; $$->right2 = ""; free($1); if ($3) free($3); }
  | IDENTIFIER GE value { $$ = new Condition(); $$->left = $1; $$->op = ">="; $$->right = $3 ? $3 : ""; $$->right2 = ""; free($1); if ($3) free($3); }
  | IDENTIFIER LT value { $$ = new Condition(); $$->left = $1; $$->op = "<"; $$->right = $3 ? $3 : ""; $$->right2 = ""; free($1); if ($3) free($3); }
  | IDENTIFIER GT value { $$ = new Condition(); $$->left = $1; $$->op = ">"; $$->right = $3 ? $3 : ""; $$->right2 = ""; free($1); if ($3) free($3); }
  ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Parse error: %s\n", s);
}
