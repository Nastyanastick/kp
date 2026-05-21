#pragma once

#include <string>
#include <vector>

struct Column {
    std::string name;
    std::string type;
    bool notNull = false;
    bool indexed = false;
};

struct Condition {
    std::string columnName;
    std::string op;
    std::string value;
};

void createTable(const std::string& body);
void dropTable(const std::string& tableName);
void insertInto(const std::string& body);
void selectFrom(const std::string& body);
void deleteFrom(const std::string& body);

bool parseColumnDefinition(const std::string& text, Column& column, std::string& error);
bool parseCondition(const std::string& text, Condition& cond, std::string& error);

std::vector<std::string> splitByCommaTopLevel(const std::string& text);