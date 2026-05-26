#include "sql_ast.h"
#include <vector>
#include <string>
#include <cstdlib>
#include <cstring>

typedef void* YY_BUFFER_STATE;

int yyparse();
YY_BUFFER_STATE yy_scan_string(const char*);
int yylex_destroy();

static std::vector<sql::Command>* g_out = nullptr;

extern "C" void sql_push_create_database(const char* name) {
    if (!g_out) return;
    sql::Command cmd;
    cmd.type = sql::CmdType::CREATE_DATABASE;
    cmd.dbName = name ? name : "";
    g_out->push_back(cmd);
}

extern "C" void sql_push_drop_database(const char* name) {
    if (!g_out) return;
    sql::Command cmd;
    cmd.type = sql::CmdType::DROP_DATABASE;
    cmd.dbName = name ? name : "";
    g_out->push_back(cmd);
}

extern "C" void sql_push_use_database(const char* name) {
    if (!g_out) return;
    sql::Command cmd;
    cmd.type = sql::CmdType::USE_DATABASE;
    cmd.dbName = name ? name : "";
    g_out->push_back(cmd);
}

static sql::CreateTableCmd g_ct;
extern "C" void sql_push_create_table_start(const char* name) {
    g_ct = sql::CreateTableCmd();
    g_ct.tableName = name ? name : "";
}
extern "C" void sql_push_create_table_column(const char* name, const char* type, int notnull, int indexed) {
    Column c;
    c.name = name ? name : "";
    c.type = type ? type : "";
    c.notNull = (notnull != 0);
    c.indexed = (indexed != 0);
    g_ct.columns.push_back(c);
}
extern "C" void sql_push_create_table_end() {
    if (!g_out) return;
    sql::Command cmd;
    cmd.type = sql::CmdType::CREATE_TABLE;
    cmd.createTable = g_ct;
    g_out->push_back(cmd);
}

extern "C" void sql_push_drop_table(const char* name) {
    if (!g_out) return;
    sql::Command cmd;
    cmd.type = sql::CmdType::DROP_TABLE;
    cmd.dropTableName = name ? name : "";
    g_out->push_back(cmd);
}

static sql::InsertCmd g_ins;
static bool g_in_row = false;
extern "C" void sql_push_insert(const char* tableName) {
    g_ins = sql::InsertCmd();
    g_ins.tableName = tableName ? tableName : "";
}
extern "C" void sql_insert_add_column(const char* name) {
    if (!name) return;
    g_ins.columns.push_back(name);
}
extern "C" void sql_insert_add_row_start() {
    g_in_row = true;
    g_ins.rows.emplace_back();
}
extern "C" void sql_insert_add_row_value(const char* val, int isString, int isNull) {
    if (!g_in_row) return;
    if (isNull) {
        g_ins.rows.back().push_back(std::string("NULL"));
    } else if (isString) {
        g_ins.rows.back().push_back(val ? std::string(val) : std::string(""));
    } else {
        g_ins.rows.back().push_back(val ? std::string(val) : std::string(""));
    }
}
extern "C" void sql_insert_add_row_end() {
    g_in_row = false;
}

static sql::SelectCmd g_sel;
extern "C" void sql_push_select_start() { g_sel = sql::SelectCmd(); }
extern "C" void sql_select_add_all() { sql::SelectColumn c; c.all = true; g_sel.columns.push_back(c); }
extern "C" void sql_select_add_column(const char* name, const char* alias) { sql::SelectColumn c; c.all = false; c.name = name?name:""; c.alias = alias?alias:c.name; g_sel.columns.push_back(c); }
extern "C" void sql_select_set_table(const char* name) { g_sel.tableName = name?name:""; }
extern "C" void sql_select_set_where(const char* left, const char* op, const char* right, const char* right2) { g_sel.where.left = left?left:""; g_sel.where.op = op?op:""; g_sel.where.right = right?right:""; g_sel.where.right2 = right2?right2:""; g_sel.hasWhere = true; }

static sql::UpdateCmd g_upd;
extern "C" void sql_push_update_start(const char* tableName) { g_upd = sql::UpdateCmd(); g_upd.tableName = tableName?tableName:""; }
extern "C" void sql_update_add_assignment(const char* col, const char* val) { g_upd.assignments.emplace_back(col?col:"", val?val:"NULL"); }
extern "C" void sql_update_set_where(const char* left, const char* op, const char* right, const char* right2) { g_upd.where.left = left?left:""; g_upd.where.op = op?op:""; g_upd.where.right = right?right:""; g_upd.where.right2 = right2?right2:""; g_upd.hasWhere = true; }

static sql::DeleteCmd g_del;
extern "C" void sql_push_delete_start(const char* tableName) { g_del = sql::DeleteCmd(); g_del.tableName = tableName?tableName:""; }
extern "C" void sql_delete_set_where(const char* left, const char* op, const char* right, const char* right2) { g_del.where.left = left?left:""; g_del.where.op = op?op:""; g_del.where.right = right?right:""; g_del.where.right2 = right2?right2:""; g_del.hasWhere = true; }

extern "C" void sql_push_insert_finalize() {
    if (!g_out) return; sql::Command cmd; cmd.type = sql::CmdType::INSERT; cmd.insert = g_ins; g_out->push_back(cmd);
}
extern "C" void sql_push_select_finalize() {
    if (!g_out) return; sql::Command cmd; cmd.type = sql::CmdType::SELECT; cmd.select = g_sel; g_out->push_back(cmd);
}
extern "C" void sql_push_update_finalize() {
    if (!g_out) return; sql::Command cmd; cmd.type = sql::CmdType::UPDATE; cmd.update = g_upd; g_out->push_back(cmd);
}
extern "C" void sql_push_delete_finalize() {
    if (!g_out) return; sql::Command cmd; cmd.type = sql::CmdType::DELETE; cmd.del = g_del; g_out->push_back(cmd);
}

bool parse_sql(const std::string& text, std::vector<sql::Command>& out) {
    g_out = &out;
    // scan and parse
    yy_scan_string(text.c_str());
    int r = yyparse();
    yylex_destroy();
    g_out = nullptr;
    return r == 0;
}
