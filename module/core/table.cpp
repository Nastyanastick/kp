#include "table.h"
#include "../parser/parser.h"
#include "database.h"
#include "../index/index_manager.h"
#include "../memory/string_pool.h"
#include "../parser/sql_ast.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <direct.h>
#include <io.h>
#include <cstdlib>
#include <algorithm>
#include <set>
#include <cctype>
#include <regex>
#include <limits>

static bool pathExists(const std::string& path) {
    return _access(path.c_str(), 0) == 0;
}

static bool createDirectory(const std::string& path) {
    return _mkdir(path.c_str()) == 0;
}

static void removeAll(const std::string& path) {
    std::string command = "rmdir /s /q \"" + path + "\"";
    system(command.c_str());
}

std::string makePath(const std::string& a, const std::string& b) {
    if (a.empty()) {
        return b;
    }
    return a + "/" + b;
}

std::string resolveTablePath(const std::string& tableName) {
    size_t dotPos = tableName.find('.');

    if (dotPos == std::string::npos) {
        if (currentDatabase.empty()) {
            return "";
        }
        return DATA_DIR + "/" + currentDatabase + "/" + tableName;
    }

    std::string dbName = trim(tableName.substr(0, dotPos));
    std::string pureTableName = trim(tableName.substr(dotPos + 1));

    if (!isValidName(dbName) || !isValidName(pureTableName)) {
        return "";
    }

    return DATA_DIR + "/" + dbName + "/" + pureTableName;
}

std::string getPureTableName(const std::string& tableName) {
    size_t dotPos = tableName.find('.');
    if (dotPos == std::string::npos) {
        return tableName;
    }
    return trim(tableName.substr(dotPos + 1));
}

bool compareValues(const std::string& left, const std::string& op, const std::string& right, const std::string& type) {
    if (left == "NULL") {
        return false;
    }

    if (type == "int") {
        int a = std::stoi(left);
        int b = std::stoi(right);

        if (op == "==") return a == b;
        if (op == "!=") return a != b;
        if (op == "<")  return a < b;
        if (op == ">")  return a > b;
        if (op == "<=") return a <= b;
        if (op == ">=") return a >= b;
    } else if (type == "string") {
        std::string a = left;
        std::string b = right;

        if (op == "==") return a == b;
        if (op == "!=") return a != b;
        if (op == "<")  return a < b;
        if (op == ">")  return a > b;
        if (op == "<=") return a <= b;
        if (op == ">=") return a >= b;
    }

    return false;
}

bool isIntegerValue(const std::string& value);

bool isStringLiteral(const std::string& s) {
    return s.size() >= 2 && s.front() == '"' && s.back() == '"';
}

bool getOperandValue(
    const std::string& operand,
    const std::vector<std::string>& row,
    const std::vector<Column>& schema,
    std::string& value,
    std::string& type,
    std::string& error
) {
    for (size_t i = 0; i < schema.size(); i++) {
        if (schema[i].name == operand) {
            value = row[i];
            type = schema[i].type;
            return true;
        }
    }

    if (isIntegerValue(operand)) {
        value = operand;
        type = "int";
        return true;
    }

    if (isStringLiteral(operand)) {
        value = operand.substr(1, operand.size() - 2);
        type = "string";
        return true;
    }

    error = "unknown column or invalid constant: " + operand;
    return false;
}

bool evaluateComparison(
    const std::vector<std::string>& row,
    const std::vector<Column>& schema,
    const ConditionNode& cond,
    std::string& error
);

bool evaluateConditionTree(
    const std::vector<std::string>& row,
    const std::vector<Column>& schema,
    const ConditionNode* node,
    std::string& error
) {
    if (!node) {
        return true;  
    }
    
    switch (node->type) {
        case ConditionNode::AND:
            return evaluateConditionTree(row, schema, node->leftChild, error) &&
                   evaluateConditionTree(row, schema, node->rightChild, error);
        
        case ConditionNode::OR:
            return evaluateConditionTree(row, schema, node->leftChild, error) ||
                   evaluateConditionTree(row, schema, node->rightChild, error);
        
        case ConditionNode::COMPARISON:
            return evaluateComparison(row, schema, *node, error);
        
        default:
            error = "Unknown condition type";
            return false;
    }
}

bool evaluateComparison(
    const std::vector<std::string>& row,
    const std::vector<Column>& schema,
    const ConditionNode& cond,
    std::string& error
) {
    std::string leftValue, leftType;
    if (!getOperandValue(cond.left, row, schema, leftValue, leftType, error)) {
        return false;
    }
    
    if (cond.op == "BETWEEN") {
        if (leftType == "int") {
            if (!isIntegerValue(cond.right) || !isIntegerValue(cond.right2)) {
                error = "BETWEEN requires int bounds";
                return false;
            }
            int value = std::stoi(leftValue);
            int l = std::stoi(cond.right);
            int r = std::stoi(cond.right2);
            return value >= l && value < r;
        }
        if (leftType == "string") {
            if (!isStringLiteral(cond.right) || !isStringLiteral(cond.right2)) {
                error = "BETWEEN requires string bounds";
                return false;
            }
            std::string l = cond.right.substr(1, cond.right.size() - 2);
            std::string r = cond.right2.substr(1, cond.right2.size() - 2);
            return leftValue >= l && leftValue < r;
        }
        error = "BETWEEN on unsupported type";
        return false;
    }
    
    if (cond.op == "LIKE") {
        if (leftType != "string") {
            error = "LIKE works only with strings";
            return false;
        }
        std::string pattern = cond.right;
        if (isStringLiteral(pattern)) {
            pattern = pattern.substr(1, pattern.size() - 2);
        }
        std::string regexPattern;
        for (char ch : pattern) {
            if (ch == '%') {
                regexPattern += ".*";
            } else if (ch == '_') {
                regexPattern += ".";
            } else {
                if (strchr(".[]{}()\\*+?|^$", ch)) {
                    regexPattern += '\\';
                }
                regexPattern += ch;
            }
        }
        try {
            return std::regex_match(leftValue, std::regex(regexPattern));
        } catch (const std::regex_error&) {
            error = "invalid LIKE pattern";
            return false;
        }
    }
    
    std::string rightValue, rightType;
    if (!getOperandValue(cond.right, row, schema, rightValue, rightType, error)) {
        return false;
    }
    
    if (leftType != rightType) {
        error = "type mismatch in condition: " + leftType + " vs " + rightType;
        return false;
    }
    
    return compareValues(leftValue, cond.op, rightValue, leftType);
}

bool rowMatchesCondition(
    const std::vector<std::string>& row,
    const std::vector<Column>& schema,
    const ConditionNode* cond,
    std::string& error
) {
    return evaluateConditionTree(row, schema, cond, error);
}

std::vector<Column> loadSchema(const std::string& tablePath) {
    std::vector<Column> columns;
    std::ifstream in(tablePath + "/schema.txt");

    std::string line;
    while (std::getline(in, line)) {
        std::istringstream ss(line);

        std::string name, type, notNull, indexed;

        std::getline(ss, name, '|');
        std::getline(ss, type, '|');
        std::getline(ss, notNull, '|');
        std::getline(ss, indexed, '|');

        Column col;
        col.name = name;
        col.type = type;
        col.notNull = (notNull == "1");
        col.indexed = (indexed == "1");

        columns.push_back(col);
    }

    return columns;
}

std::vector<std::vector<std::string>> loadData(const std::string& tablePath) {
    std::vector<std::vector<std::string>> rows;

    std::ifstream in(tablePath + "/data.txt");
    std::string line;

    while (std::getline(in, line)) {
        std::vector<std::string> row;
        std::string current;

        for (char ch : line) {
            if (ch == '|') {
                row.push_back(current);
                current.clear();
            } else {
                current += ch;
            }
        }

        row.push_back(current);
        rows.push_back(row);
    }

    return rows;
}

std::vector<std::vector<std::string>> loadDataWithPool(const std::string& tablePath, StringPool& pool) {
    std::vector<std::vector<std::string>> rows;
    
    std::ifstream in(tablePath + "/data.txt");
    if (!in) return rows;
    
    std::string line;
    while (std::getline(in, line)) {
        std::vector<std::string> row;
        std::string current;
        
        for (char ch : line) {
            if (ch == '|') {
                row.push_back(current);
                current.clear();
            } else {
                current += ch;
            }
        }
        row.push_back(current);
        for (size_t i = 0; i < row.size(); i++) {
            if (row[i].find("pool:") == 0) {
                size_t id = std::stoull(row[i].substr(5));
                row[i] = pool.resolve(id);
            }
        }
        
        rows.push_back(row);
    }
    
    return rows;
}

void saveDataWithPool(const std::string& tablePath, const std::vector<std::vector<std::string>>& rows, StringPool& pool, const std::vector<Column>& schema) {
    std::ofstream out(tablePath + "/data.txt");
    if (!out) return;
    
    for (const auto& row : rows) {
        for (size_t i = 0; i < row.size(); i++) {
            if (i > 0) out << "|";
            
            if (row[i] == "NULL") {
                out << "NULL";
            } else if (schema[i].type == "string") {
                size_t id = pool.intern(row[i]);
                out << "pool:" << id;
            } else {
                out << row[i];
            }
        }
        out << "\n";
    }
}

bool saveSchema(const std::string& tablePath, const std::vector<Column>& columns) { 
    std::ofstream out(tablePath + "/schema.txt");
    if (!out) {
        return false;
    }

    for (const auto& col : columns) {
        out << col.name << '|'
            << col.type << '|'
            << (col.notNull ? 1 : 0) << '|'
            << (col.indexed ? 1 : 0) << '\n';
    }

    return true;
}

void dropTable(const std::string& tableName) {
    std::string tablePath = resolveTablePath(tableName);

    if (tablePath.empty()) {
        std::cout << "Error: no database selected or invalid table name\n";
        return;
    }

    if (!pathExists(tablePath)) {
        std::cout << "Error: table does not exist\n";
        return;
    }

    removeAll(tablePath);
    std::cout << "Table '" << getPureTableName(tableName) << "' dropped\n";
}

bool isIntegerValue(const std::string& value) {
    if (value.empty()) {
        return false;
    }

    size_t start = 0;

    if (value[0] == '-') {
        if (value.size() == 1) {
            return false;
        }
        start = 1;
    }

    for (size_t i = start; i < value.size(); i++) {
        if (!std::isdigit(static_cast<unsigned char>(value[i]))) {
            return false;
        }
    }

    return true;
}

void createTableFromAST(const sql::CreateTableCmd& cmd) {
    if (currentDatabase.empty()) {
        std::cout << "Error: no database selected\n";
        return;
    }

    std::string tableName = trim(cmd.tableName);
    if (!isValidName(tableName)) {
        std::cout << "Error: invalid table name\n";
        return;
    }

    std::string tablePath = resolveTablePath(tableName);

    if (tablePath.empty()) {
        std::cout << "Error: no database selected\n";
        return;
    }
    if (pathExists(tablePath)) {
        std::cout << "Error: table already exists\n";
        return;
    }

    if (cmd.columns.empty()) {
        std::cout << "Error: table must contain at least one column\n";
        return;
    }

    std::set<std::string> usedNames;
    for (const auto& col : cmd.columns) {
        if (!isValidName(col.name)) {
            std::cout << "Error: invalid column name: " << col.name << "\n";
            return;
        }
        if (col.type != "int" && col.type != "string") {
            std::cout << "Error: unsupported column type: " << col.type << "\n";
            return;
        }
        if (usedNames.count(col.name)) {
            std::cout << "Error: duplicate column name: " << col.name << "\n";
            return;
        }
        usedNames.insert(col.name);
    }

    if (!createDirectory(tablePath)) {
        std::cout << "Error: failed to create table directory\n";
        return;
    }

    if (!saveSchema(tablePath, cmd.columns)) {
        removeAll(tablePath);
        std::cout << "Error: failed to save schema\n";
        return;
    }

    StringPool pool;
    pool.save(tablePath + "/string_pool.txt");
    std::cout << "Table '" << tableName << "' created\n";
}

void insertFromAST(const sql::InsertCmd& cmd) {
    std::string tablePath = resolveTablePath(cmd.tableName);

    if (tablePath.empty()) {
        std::cout << "Error: no database selected or invalid table name\n";
        return;
    }

    if (!pathExists(tablePath)) {
        std::cout << "Error: table does not exist\n";
        return;
    }

    auto schema = loadSchema(tablePath);

    std::vector<std::string> insertColumns = cmd.columns;

    if (insertColumns.empty()) {
        for (const auto& col : schema) {
            insertColumns.push_back(col.name);
        }
    }

    std::set<std::string> usedColumns;
    for (const auto& colName : insertColumns) {
        if (usedColumns.count(colName)) {
            std::cout << "Error: duplicate column in INSERT: " << colName << "\n";
            return;
        }

        bool found = false;
        for (const auto& col : schema) {
            if (col.name == colName) {
                found = true;
                break;
            }
        }

        if (!found) {
            std::cout << "Error: unknown column " << colName << "\n";
            return;
        }

        usedColumns.insert(colName);
    }

    StringPool pool;
    pool.load(tablePath + "/string_pool.txt");

    if (cmd.rows.empty()) {
        std::cout << "Error: invalid VALUES list\n";
        return;
    }

    IndexManager indexManager;
    indexManager.buildIndexes(tablePath, schema);

    std::vector<std::vector<std::string>> rowsToInsert;
    std::set<std::string> batchIndexedValues;

    for (const auto& rowVals : cmd.rows) {
        if (rowVals.size() != insertColumns.size()) {
            std::cout << "Error: columns and values count mismatch\n";
            return;
        }

        std::vector<std::string> row(schema.size(), "NULL");

        for (size_t i = 0; i < insertColumns.size(); i++) {
            std::string colName = trim(insertColumns[i]);
            std::string val = trim(rowVals[i]);

            int colIndex = -1;
            for (size_t j = 0; j < schema.size(); j++) {
                if (schema[j].name == colName) {
                    colIndex = static_cast<int>(j);
                    break;
                }
            }

            if (colIndex == -1) {
                std::cout << "Error: unknown column " << colName << "\n";
                return;
            }

            if (val == "NULL") {
                if (schema[colIndex].notNull || schema[colIndex].indexed) {
                    std::cout << "Error: NULL in NOT_NULL/INDEXED column " << colName << "\n";
                    return;
                }
            } else if (schema[colIndex].type == "int") {
                if (!isIntegerValue(val)) {
                    std::cout << "Error: expected int for " << colName << "\n";
                    return;
                }
            } else if (schema[colIndex].type == "string") {
            }

            row[colIndex] = val;
        }

        for (size_t i = 0; i < schema.size(); i++) {
            if ((schema[i].notNull || schema[i].indexed) && row[i] == "NULL") {
                std::cout << "Error: NULL in NOT_NULL/INDEXED column " << schema[i].name << "\n";
                return;
            }

            if (schema[i].indexed) {
                std::string key = schema[i].name + "|" + row[i];

                if (!indexManager.checkUnique(schema[i].name, row[i])) {
                    std::cout << "Error: duplicate value for INDEXED column " << schema[i].name << "\n";
                    return;
                }

                if (batchIndexedValues.count(key)) {
                    std::cout << "Error: duplicate value for INDEXED column " << schema[i].name << "\n";
                    return;
                }

                batchIndexedValues.insert(key);
            }
        }

        rowsToInsert.push_back(row);
    }

    auto existingRows = loadDataWithPool(tablePath, pool);

    std::vector<std::vector<std::string>> allRows = existingRows;

    for (const auto& row : rowsToInsert) {
        allRows.push_back(row);
    }

    saveDataWithPool(tablePath, allRows, pool, schema);
    pool.save(tablePath + "/string_pool.txt");

    IndexManager newIndexManager;
    newIndexManager.buildIndexes(tablePath, schema);
    newIndexManager.saveIndexes();

    std::cout << rowsToInsert.size() << " row(s) inserted\n";
}


void deleteFromAST(const sql::DeleteCmd& cmd) {
    std::string tablePath = resolveTablePath(cmd.tableName);

    if (tablePath.empty() || !pathExists(tablePath)) {
        std::cout << "Error: table does not exist\n";
        return;
    }

    auto schema = loadSchema(tablePath);
    StringPool pool;
    pool.load(tablePath + "/string_pool.txt");
    auto rows = loadDataWithPool(tablePath, pool);

    std::string error;

    if (!cmd.where) {
        std::cout << "Error: DELETE requires WHERE\n";
        return;
    }

    std::vector<std::vector<std::string>> remainingRows;
    int deletedCount = 0;

    for (const auto& row : rows) {
        std::string matchError;
        if (rowMatchesCondition(row, schema, (const ConditionNode*)cmd.where, matchError)) {
            if (!matchError.empty()) {
                std::cout << "Error: " << matchError << "\n";
                return;
            }
            deletedCount++;
        } else {
            remainingRows.push_back(row);
        }
    }

    saveDataWithPool(tablePath, remainingRows, pool, schema);
    pool.save(tablePath + "/string_pool.txt");

    IndexManager indexManager;
    indexManager.buildIndexes(tablePath, schema);
    indexManager.saveIndexes();


    std::cout << deletedCount << " row(s) deleted\n";
}

void updateFromAST(const sql::UpdateCmd& cmd) {
    std::string tablePath = resolveTablePath(cmd.tableName);

    if (tablePath.empty() || !pathExists(tablePath)) {
        std::cout << "Error: table does not exist\n";
        return;
    }

    auto schema = loadSchema(tablePath);

    StringPool pool;
    pool.load(tablePath + "/string_pool.txt");

    auto rows = loadDataWithPool(tablePath, pool);

    std::vector<int> setIndexes;
    std::vector<std::string> setValues;

    for (const auto& assignment : cmd.assignments) {
        std::string colName = assignment.first;
        std::string value = assignment.second;

        int colIndex = -1;

        for (size_t i = 0; i < schema.size(); i++) {
            if (schema[i].name == colName) {
                colIndex = static_cast<int>(i);
                break;
            }
        }

        if (colIndex == -1) {
            std::cout << "Error: column does not exist\n";
            return;
        }

        if (value == "NULL") {
            if (schema[colIndex].notNull || schema[colIndex].indexed) {
                std::cout << "Error: NULL in NOT_NULL/INDEXED column " << colName << "\n";
                return;
            }
        } else if (schema[colIndex].type == "int") {
            if (!isIntegerValue(value)) {
                std::cout << "Error: invalid int value\n";
                return;
            }
        }

        setIndexes.push_back(colIndex);
        setValues.push_back(value);
    }

    if (!cmd.where) {
        std::cout << "Error: UPDATE requires WHERE\n";
        return;
    }

    std::vector<std::vector<std::string>> newRows = rows;
    int updatedCount = 0;

    for (size_t r = 0; r < newRows.size(); r++) {
        std::string errorText;

        bool matched = rowMatchesCondition(newRows[r], schema, (const ConditionNode*)cmd.where, errorText);

        if (!errorText.empty()) {
            std::cout << "Error: " << errorText << "\n";
            return;
        }

        if (matched) {
            for (size_t i = 0; i < setIndexes.size(); i++) {
                newRows[r][setIndexes[i]] = setValues[i];
            }
            updatedCount++;
        }
    }

    for (size_t col = 0; col < schema.size(); col++) {
        if (!schema[col].indexed) {
            continue;
        }

        std::set<std::string> values;

        for (const auto& row : newRows) {
            std::string value = row[col];

            if (value == "NULL") {
                std::cout << "Error: NULL in INDEXED column " << schema[col].name << "\n";
                return;
            }

            if (values.count(value)) {
                std::cout << "Error: duplicate value for INDEXED column " << schema[col].name << "\n";
                return;
            }

            values.insert(value);
        }
    }

    saveDataWithPool(tablePath, newRows, pool, schema);
    pool.save(tablePath + "/string_pool.txt");

    IndexManager indexManager;
    indexManager.buildIndexes(tablePath, schema);
    indexManager.saveIndexes();

    std::cout << updatedCount << " row(s) updated\n";
}

void selectFromAST(const sql::SelectCmd& cmd) {
    std::string tablePath = resolveTablePath(cmd.tableName);
    if (tablePath.empty()) { std::cout << "Error: no database selected\n"; return; }
    if (!pathExists(tablePath)) { std::cout << "Error: table does not exist\n"; return; }

    auto schema = loadSchema(tablePath);
    StringPool pool; pool.load(tablePath + "/string_pool.txt");
    auto rows = loadDataWithPool(tablePath, pool);

    IndexManager indexManager;
    indexManager.buildIndexes(tablePath, schema);
    indexManager.saveIndexes();

    bool useIndex = false;
    std::set<size_t> indexedRowIds;

    const ConditionNode* whereCond = (const ConditionNode*)cmd.where;

    if (whereCond && whereCond->type == ConditionNode::COMPARISON) {
        int indexedColumn = -1;
        for (size_t i = 0; i < schema.size(); i++) {
            if (schema[i].name == whereCond->left && schema[i].indexed) {
                indexedColumn = (int)i;
                break;
            }
        }

        if (indexedColumn != -1 && indexManager.hasIndex(whereCond->left) && whereCond->op != "LIKE") {
            std::string colName = whereCond->left;

            if (whereCond->op == "==") {
                std::string key = whereCond->right;
                size_t rowId;
                if (indexManager.findRowId(colName, key, rowId)) {
                    indexedRowIds.insert(rowId);
                }
                useIndex = true;
            } else if (whereCond->op == "BETWEEN") {
                auto ids = indexManager.findRange(colName, whereCond->right, whereCond->right2);
                indexedRowIds.insert(ids.begin(), ids.end());
                useIndex = true;
            } else if (whereCond->op == ">" || whereCond->op == ">=" || whereCond->op == "<" || whereCond->op == "<=") {
                std::string bound = whereCond->right;
                std::string leftKey, rightKey;

                if (schema[indexedColumn].type == "int") {
                    leftKey = std::to_string(std::numeric_limits<int>::min());
                    rightKey = std::to_string(std::numeric_limits<int>::max());
                } else {
                    leftKey = "";
                    rightKey = std::string(100, char(127));
                }

                if (whereCond->op == ">" || whereCond->op == ">=") {
                    leftKey = bound;
                } else {
                    rightKey = bound;
                }

                auto ids = indexManager.findRange(colName, leftKey, rightKey);
                indexedRowIds.insert(ids.begin(), ids.end());
                useIndex = true;
            }
        }
    }

    bool hasAggregate = false;
    for (const auto& sc : cmd.columns) {
        if (sc.aggregate) {
            hasAggregate = true;
            break;
        }
    }

    if (hasAggregate) {
        std::vector<std::string> outputNames;
        std::vector<long long> sums;
        std::vector<int> counts;

        for (const auto& sc : cmd.columns) {
            if (!sc.aggregate) {
                std::cout << "Error: cannot mix aggregate and regular columns\n";
                return;
            }

            std::string outName = sc.alias.empty()
                ? sc.aggFunc + "(" + sc.aggArg + ")"
                : sc.alias;

            if (!isValidName(outName) && sc.alias.empty() == false) {
                std::cout << "Error: invalid alias name: " << outName << "\n";
                return;
            }

            outputNames.push_back(outName);
            sums.push_back(0);
            counts.push_back(0);
        }

        for (size_t rowIndex = 0; rowIndex < rows.size(); rowIndex++) {
            if (useIndex && indexedRowIds.count(rowIndex) == 0) {
                continue;
            }

            const auto& row = rows[rowIndex];

            if (whereCond) {
                std::string error;
                bool matched = rowMatchesCondition(row, schema, whereCond, error);

                if (!error.empty()) {
                    std::cout << "Error: " << error << "\n";
                    return;
                }

                if (!matched) {
                    continue;
                }
            }

            for (size_t a = 0; a < cmd.columns.size(); a++) {
                const auto& sc = cmd.columns[a];

                if (sc.aggFunc == "COUNT" && sc.aggArg == "*") {
                    counts[a]++;
                    continue;
                }

                int colIndex = -1;
                for (size_t i = 0; i < schema.size(); i++) {
                    if (schema[i].name == sc.aggArg) {
                        colIndex = static_cast<int>(i);
                        break;
                    }
                }

                if (colIndex == -1) {
                    std::cout << "Error: unknown column in aggregate: " << sc.aggArg << "\n";
                    return;
                }

                std::string val = row[colIndex];
                if (val == "NULL") {
                    continue;
                }

                if (sc.aggFunc == "COUNT") {
                    counts[a]++;
                } else if (sc.aggFunc == "SUM" || sc.aggFunc == "AVG") {
                    if (schema[colIndex].type != "int") {
                        std::cout << "Error: " << sc.aggFunc << " works only with int columns\n";
                        return;
                    }

                    sums[a] += std::stoll(val);
                    counts[a]++;
                } else {
                    std::cout << "Error: unknown aggregate function: " << sc.aggFunc << "\n";
                    return;
                }
            }
        }

        std::cout << "[\n";
        std::cout << "  {";

        for (size_t a = 0; a < cmd.columns.size(); a++) {
            if (a > 0) {
                std::cout << ", ";
            }

            const auto& sc = cmd.columns[a];
            std::cout << "\"" << outputNames[a] << "\": ";

            if (sc.aggFunc == "COUNT") {
                std::cout << counts[a];
            } else if (sc.aggFunc == "SUM") {
                std::cout << sums[a];
            } else if (sc.aggFunc == "AVG") {
                if (counts[a] == 0) {
                    std::cout << "null";
                } else {
                    double avg = static_cast<double>(sums[a]) / counts[a];
                    std::cout << avg;
                }
            }
        }

        std::cout << "}\n";
        std::cout << "]\n";
        return;
    }

    std::vector<size_t> selectedIndexes;
    std::vector<std::string> outputNames;

    if (!cmd.columns.empty() && cmd.columns[0].all) {
        for (size_t i = 0; i < schema.size(); i++) {
            selectedIndexes.push_back(i);
            outputNames.push_back(schema[i].name);
        }
    } else {
        for (const auto& sc : cmd.columns) {
            std::string columnName = sc.name;
            std::string alias = sc.alias.empty() ? sc.name : sc.alias;

            if (!isValidName(alias)) {
                std::cout << "Error: invalid alias name: " << alias << "\n";
                return;
            }

            bool found = false;
            for (size_t i = 0; i < schema.size(); i++) {
                if (schema[i].name == columnName) {
                    selectedIndexes.push_back(i);
                    outputNames.push_back(alias);
                    found = true;
                    break;
                }
            }

            if (!found) {
                std::cout << "Error: unknown column in SELECT: " << columnName << "\n";
                return;
            }
        }
    }

    std::cout << "[\n";
    bool firstPrinted = true;

    for (size_t rowIndex = 0; rowIndex < rows.size(); rowIndex++) {
        if (useIndex && indexedRowIds.count(rowIndex) == 0) continue;
        const auto& row = rows[rowIndex];

        if (whereCond) {
            std::string error;
            bool matched = rowMatchesCondition(row, schema, whereCond, error);
            if (!error.empty()) {
                std::cout << "Error: " << error << "\n";
                return;
            }
            if (!matched) continue;
        }

        if (!firstPrinted) std::cout << ",\n";

        std::cout << "  {";
        for (size_t k = 0; k < selectedIndexes.size(); k++) {
            size_t j = selectedIndexes[k];
            std::cout << "\"" << outputNames[k] << "\": ";
            std::string val = (j < row.size()) ? row[j] : "NULL";
            if (val == "NULL") {
                std::cout << "null";
            } else if (schema[j].type == "int") {
                std::cout << val;
            } else {
                std::string realValue;
                if (val.find("pool:") == 0) {
                    size_t id = std::stoull(val.substr(5));
                    realValue = pool.resolve(id);
                } else {
                    realValue = val;
                }
                std::cout << "\"" << realValue << "\"";
            }
            if (k + 1 < selectedIndexes.size()) std::cout << ", ";
        }
        std::cout << "}";
        firstPrinted = false;
    }

    std::cout << "\n]\n";
}
