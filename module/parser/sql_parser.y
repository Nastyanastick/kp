%{
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
%}

%code requires {
#include "../core/table.h"
#include "sql_ast.h"
}

%union {
    char* str;
    ConditionNode* cond;
}

%token <str> IDENTIFIER
%token <str> STRING_LITERAL
%token <str> NUMBER
%token NULL_TOKEN

%token CREATE DROP DATABASE USE TABLE INSERT INTO VALUES SELECT FROM WHERE UPDATE SET DELETE AS BETWEEN LIKE
%token SUM COUNT AVG
%token AND OR

%token EQ NEQ LE GE LT GT

%left OR
%left AND

%type <cond> cond or_expr and_expr primary_expr comparison_expr
%type <str> compare_value assign_value

%%

commands:
    /* empty */
  | commands stmt
  ;

stmt:
    CREATE DATABASE IDENTIFIER ';'
        { sql_push_create_database($3); free($3); }

  | DROP DATABASE IDENTIFIER ';'
        { sql_push_drop_database($3); free($3); }

  | USE IDENTIFIER ';'
        { sql_push_use_database($2); free($2); }

  | CREATE TABLE IDENTIFIER
        { sql_push_create_table_start($3); free($3); }
    '(' column_list ')' ';'
        { sql_push_create_table_end(); }

  | DROP TABLE IDENTIFIER ';'
        { sql_push_drop_table($3); free($3); }

  | INSERT INTO IDENTIFIER
        { sql_push_insert($3); free($3); }
    '(' id_list ')' VALUES insert_row_list ';'
        { sql_push_insert_finalize(); }

  | INSERT INTO IDENTIFIER
        { sql_push_insert($3); free($3); }
    VALUES insert_row_list ';'
        { sql_push_insert_finalize(); }

  | SELECT
        { sql_push_select_start(); }
    select_list FROM IDENTIFIER
        { sql_select_set_table($5); free($5); }
    select_where ';'
        { sql_push_select_finalize(); }

  | UPDATE IDENTIFIER
        { sql_push_update_start($2); free($2); }
    SET assignment_list update_where ';'
        { sql_push_update_finalize(); }

  | DELETE FROM IDENTIFIER
        { sql_push_delete_start($3); free($3); }
    delete_where ';'
        { sql_push_delete_finalize(); }
  ;

column_list:
    column_def
  | column_list ',' column_def
  ;

column_def:
    IDENTIFIER IDENTIFIER
        {
            sql_push_create_table_column($1, $2, 0, 0);
            free($1); free($2);
        }

  | IDENTIFIER IDENTIFIER IDENTIFIER
        {
            int notnull = 0;
            int indexed = 0;

            if (iequals($3, "NOT_NULL")) notnull = 1;
            else if (iequals($3, "INDEXED")) indexed = 1;

            sql_push_create_table_column($1, $2, notnull, indexed);
            free($1); free($2); free($3);
        }
  ;

id_list:
    IDENTIFIER
        { sql_insert_add_column($1); free($1); }

  | id_list ',' IDENTIFIER
        { sql_insert_add_column($3); free($3); }
  ;

insert_row_list:
    insert_row
  | insert_row_list ',' insert_row
  ;

insert_row:
    '('
        { sql_insert_add_row_start(); }
    insert_value_list ')'
        { sql_insert_add_row_end(); }
  ;

insert_value_list:
    insert_value
  | insert_value_list ',' insert_value
  ;

insert_value:
    NUMBER
        {
            sql_insert_add_row_value($1, 0, 0);
            free($1);
        }

  | STRING_LITERAL
        {
            sql_insert_add_row_value($1, 1, 0);
            free($1);
        }

  | NULL_TOKEN
        {
            sql_insert_add_row_value(nullptr, 0, 1);
        }
  ;

compare_value:
    NUMBER
        { $$ = $1; }

  | STRING_LITERAL
        {
            $$ = quote_string($1);
            free($1);
        }

  | NULL_TOKEN
        { $$ = strdup("NULL"); }
  ;

assign_value:
    NUMBER
        { $$ = $1; }

  | STRING_LITERAL
        { $$ = $1; }

  | NULL_TOKEN
        { $$ = strdup("NULL"); }
  ;

select_list:
    '*'
        { sql_select_add_all(); }

  | select_items
  ;

select_items:
    select_item
  | select_items ',' select_item
  ;

select_item:
    IDENTIFIER
        { sql_select_add_column($1, nullptr); free($1); }

  | IDENTIFIER AS IDENTIFIER
        { sql_select_add_column($1, $3); free($1); free($3); }

  | SUM '(' IDENTIFIER ')'
        { sql_select_add_aggregate("SUM", $3, nullptr); free($3); }

  | SUM '(' IDENTIFIER ')' AS IDENTIFIER
        { sql_select_add_aggregate("SUM", $3, $6); free($3); free($6); }

  | COUNT '(' '*' ')'
        { sql_select_add_aggregate("COUNT", "*", nullptr); }

  | COUNT '(' '*' ')' AS IDENTIFIER
        { sql_select_add_aggregate("COUNT", "*", $6); free($6); }

  | COUNT '(' IDENTIFIER ')'
        { sql_select_add_aggregate("COUNT", $3, nullptr); free($3); }

  | COUNT '(' IDENTIFIER ')' AS IDENTIFIER
        { sql_select_add_aggregate("COUNT", $3, $6); free($3); free($6); }

  | AVG '(' IDENTIFIER ')'
        { sql_select_add_aggregate("AVG", $3, nullptr); free($3); }

  | AVG '(' IDENTIFIER ')' AS IDENTIFIER
        { sql_select_add_aggregate("AVG", $3, $6); free($3); free($6); }
  ;

select_where:
    /* empty */
        { sql_select_set_where(nullptr); }

  | WHERE cond
        { sql_select_set_where($2); }
  ;

update_where:
    /* empty */
        { sql_update_set_where(nullptr); }

  | WHERE cond
        { sql_update_set_where($2); }
  ;

delete_where:
    /* empty */
        { sql_delete_set_where(nullptr); }

  | WHERE cond
        { sql_delete_set_where($2); }
  ;

assignment_list:
    assignment
  | assignment_list ',' assignment
  ;

assignment:
    IDENTIFIER '=' assign_value
        {
            sql_update_add_assignment($1, $3);
            free($1);
            if ($3) free($3);
        }
  ;

cond:
    or_expr
        { $$ = $1; }
  ;

or_expr:
    and_expr
        { $$ = $1; }

  | or_expr OR and_expr
        {
            $$ = new ConditionNode(ConditionNode::OR);
            $$->leftChild = $1;
            $$->rightChild = $3;
        }
  ;

and_expr:
    primary_expr
        { $$ = $1; }

  | and_expr AND primary_expr
        {
            $$ = new ConditionNode(ConditionNode::AND);
            $$->leftChild = $1;
            $$->rightChild = $3;
        }
  ;

primary_expr:
    comparison_expr
        { $$ = $1; }

  | '(' or_expr ')'
        { $$ = $2; }
  ;

comparison_expr:
    IDENTIFIER BETWEEN compare_value AND compare_value
        {
            $$ = new ConditionNode($1, "BETWEEN", $3 ? $3 : "", $5 ? $5 : "");
            free($1);
            if ($3) free($3);
            if ($5) free($5);
        }

  | IDENTIFIER LIKE compare_value
        {
            $$ = new ConditionNode($1, "LIKE", $3 ? $3 : "");
            free($1);
            if ($3) free($3);
        }

  | IDENTIFIER EQ compare_value
        {
            $$ = new ConditionNode($1, "==", $3 ? $3 : "");
            free($1);
            if ($3) free($3);
        }

  | IDENTIFIER NEQ compare_value
        {
            $$ = new ConditionNode($1, "!=", $3 ? $3 : "");
            free($1);
            if ($3) free($3);
        }

  | IDENTIFIER LE compare_value
        {
            $$ = new ConditionNode($1, "<=", $3 ? $3 : "");
            free($1);
            if ($3) free($3);
        }

  | IDENTIFIER GE compare_value
        {
            $$ = new ConditionNode($1, ">=", $3 ? $3 : "");
            free($1);
            if ($3) free($3);
        }

  | IDENTIFIER LT compare_value
        {
            $$ = new ConditionNode($1, "<", $3 ? $3 : "");
            free($1);
            if ($3) free($3);
        }

  | IDENTIFIER GT compare_value
        {
            $$ = new ConditionNode($1, ">", $3 ? $3 : "");
            free($1);
            if ($3) free($3);
        }
  ;

%%

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
