#pragma once
#include <string>
#include <vector>
#include "../core/table.h"

struct ConditionNode {
    enum Type {
        COMPARISON,  
        AND,        
        OR         
    };
    
    Type type;
    std::string left;
    std::string op;
    std::string right;
    std::string right2;  
    ConditionNode* leftChild = nullptr;
    ConditionNode* rightChild = nullptr;
    
    ConditionNode() = default;
    explicit ConditionNode(Type t) : type(t) {}
    ConditionNode(const std::string& l, const std::string& o, 
                  const std::string& r, const std::string& r2 = "")
        : type(COMPARISON), left(l), op(o), right(r), right2(r2) {}
    ~ConditionNode() {
        delete leftChild;
        delete rightChild;
    }
    ConditionNode(const ConditionNode&) = delete;
    ConditionNode& operator=(const ConditionNode&) = delete;
};

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
    bool aggregate = false;

    std::string name;
    std::string alias;

    std::string aggFunc; 
    std::string aggArg; 
};

struct SelectCmd {
    std::vector<SelectColumn> columns;
    std::string tableName;
    ::ConditionNode* where = nullptr;
};

struct UpdateCmd {
    std::string tableName;
    std::vector<std::pair<std::string, std::string>> assignments;
    ::ConditionNode* where = nullptr;
};

struct DeleteCmd {
    std::string tableName;
    ::ConditionNode* where = nullptr;
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