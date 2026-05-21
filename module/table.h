#pragma once

#include <string>
#include <vector>
#include "value.h"
#include <filesystem>

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

void createTable(const std::string& body);
void dropTable(const std::string& tableName);
void insertInto(const std::string& body);
void selectFrom(const std::string& body);
void deleteFrom(const std::string& body);
void updateRows(const std::string& body);

bool parseColumnDefinition(const std::string& text, Column& column, std::string& error);
bool parseCondition(const std::string& text, Condition& cond, std::string& error);

std::vector<std::string> splitByCommaTopLevel(const std::string& text);
std::vector<std::string> splitValues(const std::string& text);

std::filesystem::path resolveTablePath(const std::string& tableName);
std::string getPureTableName(const std::string& tableName);