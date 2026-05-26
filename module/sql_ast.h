#pragma once
#include <string>
#include <vector>
#include "table.h"

namespace sql {

enum class CmdType {
    CREATE_DATABASE,
    DROP_DATABASE,
    USE_DATABASE,
    CREATE_TABLE,
    DROP_TABLE,
    INSERT,
    SELECT,
    UPDATE,
    DELETE
};

struct CreateTableCmd {
    std::string tableName;
    std::vector<Column> columns;
};

struct InsertCmd {
    std::string tableName;
    std::vector<std::string> columns;
    std::vector<std::vector<std::string>> rows;
};

struct SelectColumn {
    bool all = false;
    std::string name;
    std::string alias;
};

struct SelectCmd {
    std::vector<SelectColumn> columns;
    std::string tableName;
    Condition where;
    bool hasWhere = false;
};

struct UpdateCmd {
    std::string tableName;
    std::vector<std::pair<std::string, std::string>> assignments;
    Condition where;
    bool hasWhere = false;
};

struct DeleteCmd {
    std::string tableName;
    Condition where;
    bool hasWhere = false;
};

struct Command {
    CmdType type;

    std::string dbName;
    CreateTableCmd createTable;
    std::string dropTableName;
    InsertCmd insert;
    SelectCmd select;
    UpdateCmd update;
    DeleteCmd del;
};

}
