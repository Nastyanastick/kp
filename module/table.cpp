#include "table.h"
#include "parser.h"
#include "database.h"
#include "index_manager.h"
#include "string_pool.h"
#include "sql_ast.h"

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
    // ищем колонку
    for (size_t i = 0; i < schema.size(); i++) {
        if (schema[i].name == operand) {
            value = row[i];
            type = schema[i].type;
            return true;
        }
    }

    // int константа
    if (isIntegerValue(operand)) {
        value = operand;
        type = "int";
        return true;
    }

    // string константа
    if (isStringLiteral(operand)) {
        value = operand.substr(1, operand.size() - 2);
        type = "string";
        return true;
    }

    error = "unknown column or invalid constant: " + operand;
    return false;
}

bool rowMatchesCondition(
    const std::vector<std::string>& row,
    const std::vector<Column>& schema,
    const Condition& cond,
    std::string& error
) {
    std::string leftValue;
    std::string leftType;

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
            if (!isStringLiteral(cond.right) ||
                !isStringLiteral(cond.right2)) {
                error = "BETWEEN requires string bounds";
                return false;
            }

            std::string l =
                cond.right.substr(1, cond.right.size() - 2);

            std::string r =
                cond.right2.substr(1, cond.right2.size() - 2);

            return leftValue >= l && leftValue < r;
        }
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
                if (ch == '.' || ch == '^' || ch == '$' || ch == '|' ||
                    ch == '(' || ch == ')' || ch == '[' || ch == ']' ||
                    ch == '{' || ch == '}' || ch == '*' || ch == '+' ||
                    ch == '?' || ch == '\\') {
                    regexPattern += '\\';
                }
                regexPattern += ch;
            }
        }

        try {
            return std::regex_match(leftValue, std::regex(regexPattern));
        }
        catch (const std::regex_error&) {
            error = "invalid LIKE pattern";
            return false;
        }
    }

    std::string rightValue;
    std::string rightType;

    if (!getOperandValue(cond.right, row, schema,
                         rightValue, rightType, error)) {
        return false;
    }

    if (leftType != rightType) {
        error = "type mismatch in condition";
        return false;
    }

    return compareValues(leftValue, cond.op,
                         rightValue, leftType);
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

bool saveSchema(const std::string& tablePath, const std::vector<Column>& columns) { // сохраняет описание таблицы в файл 
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

    // validate columns
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

    if (cmd.columns.empty()) {
        std::cout << "Error: empty column list\n";
        return;
    }

    std::set<std::string> usedColumns;
    for (const auto& colName : cmd.columns) {
        if (usedColumns.count(colName)) {
            std::cout << "Error: duplicate column in INSERT: " << colName << "\n";
            return;
        }
        bool found = false;
        for (const auto& col : schema) if (col.name == colName) { found = true; break; }
        if (!found) { std::cout << "Error: unknown column " << colName << "\n"; return; }
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
    indexManager.saveIndexes();

    std::vector<std::vector<std::string>> rowsToInsert;
    std::set<std::string> batchIndexedValues;

    for (const auto& rowVals : cmd.rows) {
        if (rowVals.size() != cmd.columns.size()) {
            std::cout << "Error: columns and values count mismatch\n";
            return;
        }

        std::vector<std::string> row(schema.size(), "NULL");

        for (size_t i = 0; i < cmd.columns.size(); i++) {
            std::string colName = trim(cmd.columns[i]);
            std::string val = trim(rowVals[i]);

            int colIndex = -1;
            for (size_t j = 0; j < schema.size(); j++) if (schema[j].name == colName) { colIndex = (int)j; break; }

            if (colIndex == -1) { std::cout << "Error: unknown column " << colName << "\n"; return; }

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
                val = "pool:" + std::to_string(pool.intern(val));
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
                std::string checkValue;
                if (schema[i].type == "string" && row[i].find("pool:") == 0) {
                    size_t id = std::stoull(row[i].substr(5));
                    checkValue = pool.resolve(id);
                } else {
                    checkValue = row[i];
                }
                if (!indexManager.checkUnique(schema[i].name, checkValue)) {
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

    std::ofstream out(tablePath + "/data.txt");
    if (!out) {
        std::cout << "Error: failed to open data file\n";
        return;
    }

    for (const auto& row : existingRows) {
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

    for (size_t r = 0; r < rowsToInsert.size(); r++) {
        const auto& row = rowsToInsert[r];

        for (size_t i = 0; i < row.size(); i++) {
            if (i > 0) out << "|";

            if (row[i] == "NULL") {
                out << "NULL";
            } else if (schema[i].type == "string") {
                out << row[i];
            } else {
                out << row[i];
            }
        }
        out << "\n";

        size_t newRowId = existingRows.size() + r;

        for (size_t i = 0; i < schema.size(); i++) {
            if (schema[i].indexed && row[i] != "NULL") {
                std::string realValue;
                if (schema[i].type == "string" && row[i].find("pool:") == 0) {
                    size_t id = std::stoull(row[i].substr(5));
                    realValue = pool.resolve(id);
                } else {
                    realValue = row[i];
                }
                indexManager.insertKey(schema[i].name, realValue, newRowId);
            }
        }
    }

    pool.save(tablePath + "/string_pool.txt");

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

    Condition cond;
    std::string error;

    if (cmd.hasWhere) {
        cond = cmd.where;
    } else {
        std::cout << "Error: invalid DELETE syntax\n";
        return;
    }

    std::vector<std::vector<std::string>> remainingRows;
    int deletedCount = 0;

    for (const auto& row : rows) {
        std::string matchError;
        if (rowMatchesCondition(row, schema, cond, matchError)) {
            deletedCount++;
        } else {
            remainingRows.push_back(row);
        }
    }

    saveDataWithPool(tablePath, remainingRows, pool, schema);
    pool.save(tablePath + "/string_pool.txt");

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
        } else if (schema[colIndex].type == "string") {
            // строка уже приходит из AST без кавычек
        }

        setIndexes.push_back(colIndex);
        setValues.push_back(value);
    }

    if (!cmd.hasWhere) {
        std::cout << "Error: UPDATE requires WHERE\n";
        return;
    }

    std::vector<std::vector<std::string>> newRows = rows;
    int updatedCount = 0;

    for (size_t r = 0; r < newRows.size(); r++) {
        std::string errorText;

        bool matched = rowMatchesCondition(newRows[r], schema, cmd.where, errorText);

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

    std::vector<size_t> selectedIndexes;
    std::vector<std::string> outputNames;

    if (!cmd.columns.empty() && cmd.columns[0].all) {
        for (size_t i = 0; i < schema.size(); i++) { selectedIndexes.push_back(i); outputNames.push_back(schema[i].name); }
    } else {
        for (const auto& sc : cmd.columns) {
            std::string columnName = sc.name;
            std::string alias = sc.alias.empty() ? sc.name : sc.alias;
            if (!isValidName(alias)) { std::cout << "Error: invalid alias name: " << alias << "\n"; return; }
            bool found = false;
            for (size_t i = 0; i < schema.size(); i++) {
                if (schema[i].name == columnName) { selectedIndexes.push_back(i); outputNames.push_back(alias); found = true; break; }
            }
            if (!found) { std::cout << "Error: unknown column in SELECT: " << columnName << "\n"; return; }
        }
    }

    IndexManager indexManager; indexManager.buildIndexes(tablePath, schema); indexManager.saveIndexes();

    bool useIndex = false; std::set<size_t> indexedRowIds;

    if (cmd.hasWhere) {
        Condition cond = cmd.where;
        int indexedColumn = -1;
        for (size_t i = 0; i < schema.size(); i++) if (schema[i].name == cond.left && schema[i].indexed) { indexedColumn = (int)i; break; }

        if (indexedColumn != -1 && indexManager.hasIndex(cond.left) && cond.op != "LIKE") {
            std::string colName = cond.left;
            std::string colType = schema[indexedColumn].type;

            if (cond.op == "==") {
                std::string key = cond.right;
                if (colType == "string") {
                    if (key.find("pool:") == 0) { size_t id = std::stoull(key.substr(5)); key = pool.resolve(id); }
                    else {  }
                }
                size_t rowId; if (indexManager.findRowId(colName, key, rowId)) indexedRowIds.insert(rowId);
                useIndex = true;
            } else if (cond.op == "BETWEEN") {
                std::string leftKey = cond.right; std::string rightKey = cond.right2;
                auto ids = indexManager.findRange(colName, leftKey, rightKey);
                indexedRowIds.insert(ids.begin(), ids.end()); useIndex = true;
            } else {
                std::string bound = cond.right;
                std::string leftKey, rightKey;
                if (schema[indexedColumn].type == "int") {
                    leftKey = std::to_string(std::numeric_limits<int>::min());
                    rightKey = std::to_string(std::numeric_limits<int>::max());
                } else { leftKey = ""; rightKey = std::string(100, char(127)); }
                if (cond.op == ">" || cond.op == ">=") leftKey = bound; else rightKey = bound;
                auto ids = indexManager.findRange(colName, leftKey, rightKey);
                indexedRowIds.insert(ids.begin(), ids.end()); useIndex = true;
            }
        }
    }

    std::cout << "[\n";
    bool firstPrinted = true;

    for (size_t rowIndex = 0; rowIndex < rows.size(); rowIndex++) {
        if (useIndex && indexedRowIds.count(rowIndex) == 0) continue;
        const auto& row = rows[rowIndex];

        if (cmd.hasWhere) {
            std::string error; bool matched = rowMatchesCondition(row, schema, cmd.where, error);
            if (!error.empty()) { std::cout << "Error: " << error << "\n"; return; }
            if (!matched) continue;
        }

        if (!firstPrinted) std::cout << ",\n";

        std::cout << "  {";
        for (size_t k = 0; k < selectedIndexes.size(); k++) {
            size_t j = selectedIndexes[k];
            std::cout << "\"" << outputNames[k] << "\": ";
            std::string val = (j < row.size()) ? row[j] : "NULL";
            if (val == "NULL") { std::cout << "null"; }
            else if (schema[j].type == "int") { std::cout << val; }
            else { std::string realValue; if (val.find("pool:") == 0) { size_t id = std::stoull(val.substr(5)); realValue = pool.resolve(id); } else realValue = val; std::cout << "\"" << realValue << "\""; }
            if (k + 1 < selectedIndexes.size()) std::cout << ", ";
        }
        std::cout << "}";
        firstPrinted = false;
    }

    std::cout << "\n]\n";
}