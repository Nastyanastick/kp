#pragma once

#include <string>
#include <vector>
#include "value.h"
#include "string_pool.h"

struct Column {
    std::string name;
    std::string type;
    bool notNull = false;
    bool indexed = false;
};

struct Condition {
    std::string left;
    std::string op;
    std::string right;
    std::string right2;
};

struct Row {
    std::vector<Value> values;
};

struct Table {
    std::string name;
    std::vector<Column> columns;
    std::vector<Row> rows;
};

void dropTable(const std::string& tableName);

namespace sql { struct CreateTableCmd; struct InsertCmd; struct SelectCmd; struct UpdateCmd; struct DeleteCmd; }

void createTableFromAST(const sql::CreateTableCmd& cmd);
void insertFromAST(const sql::InsertCmd& cmd);
void selectFromAST(const sql::SelectCmd& cmd);
void updateFromAST(const sql::UpdateCmd& cmd);
void deleteFromAST(const sql::DeleteCmd& cmd);

std::vector<std::string> splitByCommaTopLevel(const std::string& text);
std::vector<std::string> splitValues(const std::string& text);

std::string resolveTablePath(const std::string& tableName);
std::string getPureTableName(const std::string& tableName);