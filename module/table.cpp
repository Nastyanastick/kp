#include "table.h"
#include "parser.h"
#include "database.h"
#include "index_manager.h"
#include "string_pool.h"

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

std::vector<std::string> splitByCommaTopLevel(const std::string& text) { // разбивает список столбцов по запятым
    std::vector<std::string> parts;
    std::string current;
    int parenthesesDepth = 0;

    for (char ch : text) {
        if (ch == '(') {
            parenthesesDepth++;
        } else if (ch == ')') {
            parenthesesDepth--;
        }

        if (ch == ',' && parenthesesDepth == 0) {
            parts.push_back(trim(current));
            current.clear();
        } else {
            current += ch;
        }
    }

    if (!trim(current).empty()) {
        parts.push_back(trim(current));
    }

    return parts;
}

bool parseColumnDefinition(const std::string& text, Column& column, std::string& error) { // разбирает один столбец
    std::istringstream iss(text);

    std::string name;
    std::string type;
    std::string modifier;
    std::string extra;

    if (!(iss >> name >> type)) {
        error = "invalid column definition";
        return false;
    }

    if (!isValidName(name)) {
        error = "invalid column name: " + name;
        return false;
    }

    if (type != "int" && type != "string") {
        error = "unsupported column type: " + type;
        return false;
    }

    column.name = name;
    column.type = type;

    if (iss >> modifier) {
        std::string upperModifier = toUpper(modifier);

        if (upperModifier == "NOT_NULL") {
            column.notNull = true;
        } else if (upperModifier == "INDEXED") {
            column.indexed = true;
            column.notNull = true; // по заданию INDEXED не может быть NULL
        } else {
            error = "unknown column modifier: " + modifier;
            return false;
        }

        if (iss >> extra) {
            error = "too many tokens in column definition";
            return false;
        }
    }

    return true;
}

bool parseCondition(const std::string& text, Condition& cond, std::string& error) {
    std::string upperText = toUpper(text);

    size_t betweenPos = upperText.find(" BETWEEN ");
    if (betweenPos != std::string::npos) {
        size_t andPos = upperText.find(" AND ", betweenPos + 9);

        if (andPos == std::string::npos) {
            error = "invalid BETWEEN syntax";
            return false;
        }

        cond.left = trim(text.substr(0, betweenPos));
        cond.op = "BETWEEN";
        cond.right = trim(text.substr(betweenPos + 9, andPos - (betweenPos + 9)));
        cond.right2 = trim(text.substr(andPos + 5));

        if (cond.left.empty() || cond.right.empty() || cond.right2.empty()) {
            error = "invalid BETWEEN condition";
            return false;
        }

        return true;
    }

    size_t likePos = upperText.find(" LIKE ");
    if (likePos != std::string::npos) {
        cond.left = trim(text.substr(0, likePos));
        cond.op = "LIKE";
        cond.right = trim(text.substr(likePos + 6));
        cond.right2 = "";

        if (cond.left.empty() || cond.right.empty()) {
            error = "invalid LIKE condition";
            return false;
        }

        return true;
    }

    std::vector<std::string> operators = {"==", "!=", "<=", ">=", "<", ">"};

    for (const auto& op : operators) {
        size_t pos = text.find(op);
        if (pos != std::string::npos) {
            cond.left = trim(text.substr(0, pos));
            cond.op = op;
            cond.right = trim(text.substr(pos + op.size()));
            cond.right2 = "";

            if (cond.left.empty() || cond.right.empty()) {
                error = "invalid WHERE condition";
                return false;
            }

            return true;
        }
    }

    error = "unsupported WHERE condition";
    return false;
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

        if (!isStringLiteral(cond.right)) {
            error = "LIKE pattern must be string";
            return false;
        }

        std::string pattern =
            cond.right.substr(1, cond.right.size() - 2);

        try {
            return std::regex_match(leftValue, std::regex(pattern));
        }
        catch (const std::regex_error&) {
            error = "invalid regex";
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

void createTable(const std::string& commandBody) {
    if (currentDatabase.empty()) {
        std::cout << "Error: no database selected\n";
        return;
    }

    size_t openPos = commandBody.find('(');
    size_t closePos = commandBody.rfind(')');

    if (openPos == std::string::npos || closePos == std::string::npos || closePos <= openPos) {
        std::cout << "Error: invalid CREATE TABLE syntax\n";
        return;
    }

    std::string tableName = trim(commandBody.substr(0, openPos));
    if (!isValidName(tableName)) {
        std::cout << "Error: invalid table name\n";
        return;
    }

    std::string columnsText = trim(commandBody.substr(openPos + 1, closePos - openPos - 1));
    if (columnsText.empty()) {
        std::cout << "Error: table must contain at least one column\n";
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

    std::vector<std::string> columnParts = splitByCommaTopLevel(columnsText);
    std::vector<Column> columns;
    std::set<std::string> usedNames;

    for (const auto& part : columnParts) {
        Column col;
        std::string error;

        if (!parseColumnDefinition(part, col, error)) {
            std::cout << "Error: " << error << '\n';
            return;
        }

        if (usedNames.count(col.name)) {
            std::cout << "Error: duplicate column name: " << col.name << '\n';
            return;
        }

        usedNames.insert(col.name);
        columns.push_back(col);
    }

    if (!createDirectory(tablePath)) {
        std::cout << "Error: failed to create table directory\n";
        return;
    }

    if (!saveSchema(tablePath, columns)) {
        removeAll(tablePath);
        std::cout << "Error: failed to save schema\n";
        return;
    }

    StringPool pool;
    pool.save(tablePath + "/string_pool.txt");
    
    std::cout << "Table '" << tableName << "' created\n";
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

std::vector<std::string> splitValues(const std::string& text) {
    std::vector<std::string> values;
    std::string current;
    bool insideQuotes = false;

    for (char ch : text) {
        if (ch == '"') {
            insideQuotes = !insideQuotes;
            current += ch;
        } else if (ch == ',' && !insideQuotes) {
            values.push_back(trim(current));
            current.clear();
        } else {
            current += ch;
        }
    }

    if (!trim(current).empty()) {
        values.push_back(trim(current));
    }

    return values;
}

std::vector<std::string> splitInsertRows(const std::string& text) {
    std::vector<std::string> rows;
    std::string current;
    bool insideQuotes = false;
    int depth = 0;

    for (char ch : text) {
        if (ch == '"') {
            insideQuotes = !insideQuotes;
        }

        if (!insideQuotes && ch == '(') {
            if (depth == 0) {
                current.clear();
            } else {
                current += ch;
            }
            depth++;
        } else if (!insideQuotes && ch == ')') {
            depth--;

            if (depth == 0) {
                rows.push_back(trim(current));
                current.clear();
            } else {
                current += ch;
            }
        } else {
            if (depth > 0) {
                current += ch;
            }
        }
    }

    return rows;
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

void insertInto(const std::string& body) {
    size_t posValues = toUpper(body).find("VALUE");

    if (posValues == std::string::npos) {
        std::cout << "Error: invalid INSERT syntax\n";
        return;
    }

    std::string left = trim(body.substr(0, posValues));
    std::string right = trim(body.substr(posValues + 5));

    size_t open1 = left.find('(');
    size_t close1 = left.find(')');

    if (open1 == std::string::npos || close1 == std::string::npos || close1 < open1) {
        std::cout << "Error: invalid column list\n";
        return;
    }

    std::string tableName = trim(left.substr(0, open1));
    std::string columnsText = left.substr(open1 + 1, close1 - open1 - 1);

    std::string tablePath = resolveTablePath(tableName);

    if (tablePath.empty()) {
        std::cout << "Error: no database selected or invalid table name\n";
        return;
    }

    if (!pathExists(tablePath)) {
        std::cout << "Error: table does not exist\n";
        return;
    }

    auto schema = loadSchema(tablePath);
    auto columnNames = splitByCommaTopLevel(columnsText);

    StringPool pool;
    pool.load(tablePath + "/string_pool.txt");

    if (columnNames.empty()) {
        std::cout << "Error: empty column list\n";
        return;
    }

    std::set<std::string> usedColumns;

    for (const auto& colRaw : columnNames) {
        std::string colName = trim(colRaw);

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

    std::vector<std::string> rowTexts = splitInsertRows(right);

    if (rowTexts.empty()) {
        std::cout << "Error: invalid VALUES list\n";
        return;
    }

    IndexManager indexManager;
    indexManager.buildIndexes(tablePath, schema);
    indexManager.saveIndexes();

    std::vector<std::vector<std::string>> rowsToInsert;
    std::set<std::string> batchIndexedValues;

    for (const auto& rowText : rowTexts) {
        auto values = splitValues(rowText);

        if (columnNames.size() != values.size()) {
            std::cout << "Error: columns and values count mismatch\n";
            return;
        }

        std::vector<std::string> row(schema.size(), "NULL");

        for (size_t i = 0; i < columnNames.size(); i++) {
            std::string colName = trim(columnNames[i]);
            std::string val = trim(values[i]);

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
                if (val.size() < 2 || val.front() != '"' || val.back() != '"') {
                    std::cout << "Error: invalid string for " << colName << "\n";
                    return;
                }

                val = val.substr(1, val.size() - 2);
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

void deleteFrom(const std::string& body) {
    size_t wherePos = toUpper(body).find("WHERE");

    if (wherePos == std::string::npos){
        std::cout << "Error: invalid DELETE syntax\n";
        return;
    }

    std::string tableName = trim(body.substr(0, wherePos));

    std::string conditionText =
        trim(body.substr(wherePos + 5));

    std::string tablePath = resolveTablePath(tableName);

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

    if (!parseCondition(conditionText, cond, error)) {
        std::cout << "Error: " << error << "\n";
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


void selectFrom(const std::string& body) {
    std::string upperBody = toUpper(body);

    size_t fromPos = upperBody.find(" FROM ");
    if (fromPos == std::string::npos) {
        std::cout << "Error: invalid SELECT syntax\n";
        return;
    }

    std::string columnsText = trim(body.substr(0, fromPos));
    std::string afterFrom = trim(body.substr(fromPos + 6));

    std::string upperAfterFrom = toUpper(afterFrom);
    std::string tableName;
    bool hasWhere = false;
    Condition cond;

    size_t wherePos = upperAfterFrom.find(" WHERE ");

    if (wherePos == std::string::npos) {
        tableName = trim(afterFrom);
    } else {
        tableName = trim(afterFrom.substr(0, wherePos));
        std::string condText = trim(afterFrom.substr(wherePos + 7));

        std::string error;
        if (!parseCondition(condText, cond, error)) {
            std::cout << "Error: " << error << "\n";
            return;
        }

        hasWhere = true;
    }

    std::string tablePath = resolveTablePath(tableName);

    if (tablePath.empty()) {
        std::cout << "Error: no database selected\n";
        return;
    }

    if (!pathExists(tablePath)) {
        std::cout << "Error: table does not exist\n";
        return;
    }

    auto schema = loadSchema(tablePath);
    StringPool pool;
    pool.load(tablePath + "/string_pool.txt");
    auto rows = loadDataWithPool(tablePath, pool);

    std::vector<size_t> selectedIndexes;
    std::vector<std::string> outputNames;

    if (columnsText == "*") {
        for (size_t i = 0; i < schema.size(); i++) {
            selectedIndexes.push_back(i);
            outputNames.push_back(schema[i].name);
        }
    } else {
        auto parts = splitByCommaTopLevel(columnsText);

        for (const auto& partRaw : parts) {
            std::string part = trim(partRaw);
            std::string upperPart = toUpper(part);

            std::string columnName;
            std::string alias;

            size_t asPos = upperPart.find(" AS ");
            if (asPos == std::string::npos) {
                columnName = trim(part);
                alias = columnName;
            } else {
                columnName = trim(part.substr(0, asPos));
                alias = trim(part.substr(asPos + 4));
            }
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

    IndexManager indexManager;
    indexManager.buildIndexes(tablePath, schema);
    indexManager.saveIndexes();

    bool useIndex = false;
    std::set<size_t> indexedRowIds;

    if (hasWhere) {
        int indexedColumn = -1;

        for (size_t i = 0; i < schema.size(); i++) {
            if (schema[i].name == cond.left && schema[i].indexed) {
                indexedColumn = static_cast<int>(i);
                break;
            }
        }

        if (indexedColumn != -1 && indexManager.hasIndex(cond.left)) {
            std::string colName = cond.left;
            std::string colType = schema[indexedColumn].type;

            if (cond.op == "==") {
                std::string key = cond.right;

                if (colType == "string") {
                    if (key.find("pool:") == 0) {
                        size_t id = std::stoull(key.substr(5));
                        key = pool.resolve(id);
                    } else if (isStringLiteral(key)) {
                        key = key.substr(1, key.size() - 2);
                    } else {
                        std::cout << "Error: expected string literal in WHERE\n";
                        return;
                    }
                } else if (colType == "int") {
                    if (!isIntegerValue(key)) {
                        std::cout << "Error: expected int in WHERE\n";
                        return;
                    }
                }

                size_t rowId;
                if (indexManager.findRowId(colName, key, rowId)) {
                    indexedRowIds.insert(rowId);
                }

                useIndex = true;
            }

            else if (cond.op == "BETWEEN") {
                std::string leftKey = cond.right;
                std::string rightKey = cond.right2;

                if (colType == "string") {
                    if (leftKey.find("pool:") == 0) {
                        size_t id = std::stoull(leftKey.substr(5));
                        leftKey = pool.resolve(id);
                    } else if (isStringLiteral(leftKey)) {
                        leftKey = leftKey.substr(1, leftKey.size() - 2);
                    } else {
                        std::cout << "Error: expected string literal in BETWEEN\n";
                        return;
                    }
                    
                    if (rightKey.find("pool:") == 0) {
                        size_t id = std::stoull(rightKey.substr(5));
                        rightKey = pool.resolve(id);
                    } else if (isStringLiteral(rightKey)) {
                        rightKey = rightKey.substr(1, rightKey.size() - 2);
                    } else {
                        std::cout << "Error: expected string literal in BETWEEN\n";
                        return;
                    }
                } else if (colType == "int") {
                    if (!isIntegerValue(leftKey) || !isIntegerValue(rightKey)) {
                        std::cout << "Error: BETWEEN requires int bounds\n";
                        return;
                    }
                }

                auto ids = indexManager.findRange(colName, leftKey, rightKey);
                indexedRowIds.insert(ids.begin(), ids.end());
                useIndex = true;
            }

            else if (cond.op == ">" || cond.op == ">=" || cond.op == "<") {
                std::string bound = cond.right;

                if (colType == "string") {
                    if (!isStringLiteral(bound)) {
                        std::cout << "Error: expected string literal in WHERE\n";
                        return;
                    }

                    bound = bound.substr(1, bound.size() - 2);
                } else if (colType == "int") {
                    if (!isIntegerValue(bound)) {
                        std::cout << "Error: expected int in WHERE\n";
                        return;
                    }
                }

                std::string leftKey;
                std::string rightKey;

                if (colType == "int") {
                    leftKey = std::to_string(std::numeric_limits<int>::min());
                    rightKey = std::to_string(std::numeric_limits<int>::max());
                } else {
                    leftKey = "";
                    rightKey = std::string(100, char(127));
                }

                if (cond.op == ">" || cond.op == ">=") {
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

    std::cout << "[\n";
    bool firstPrinted = true;

    for (size_t rowIndex = 0; rowIndex < rows.size(); rowIndex++) {
        if (useIndex && indexedRowIds.count(rowIndex) == 0) {
            continue;
        }

        const auto& row = rows[rowIndex];

        if (hasWhere) {
            std::string error;
            bool matched = rowMatchesCondition(row, schema, cond, error);

            if (!error.empty()) {
                std::cout << "Error: " << error << "\n";
                return;
            }

            if (!matched) {
                continue;
            }
        }

        if (!firstPrinted) {
            std::cout << ",\n";
        }

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

            if (k + 1 < selectedIndexes.size()) {
                std::cout << ", ";
            }
        }

        std::cout << "}";
        firstPrinted = false;
    }

    std::cout << "\n]\n";
}

int findColumnIndex(
    const std::vector<Column>& schema,
    const std::string& columnName
) {
    for (size_t i = 0; i < schema.size(); i++) {
        if (schema[i].name == columnName) {
            return static_cast<int>(i);
        }
    }

    return -1;
}

void updateRows(const std::string& body) {
    std::string upperBody = toUpper(body);

    size_t setPos = upperBody.find(" SET ");
    size_t wherePos = upperBody.find(" WHERE ");

    if (setPos == std::string::npos || wherePos == std::string::npos) {
        std::cout << "Error: invalid UPDATE syntax\n";
        return;
    }

    std::string tableName = trim(body.substr(0, setPos));
    std::string setPart = trim(body.substr(setPos + 5, wherePos - (setPos + 5)));
    std::string wherePart = trim(body.substr(wherePos + 7));

    std::string tablePath = resolveTablePath(tableName);

    if (tablePath.empty() || !pathExists(tablePath)) {
        std::cout << "Error: table does not exist\n";
        return;
    }

    auto schema = loadSchema(tablePath);
    StringPool pool;
    pool.load(tablePath + "/string_pool.txt");
    auto rows = loadDataWithPool(tablePath, pool);

    auto assignments = splitByCommaTopLevel(setPart);

    std::vector<int> setIndexes;
    std::vector<std::string> setValues;

    for (const auto& assignment : assignments) {
        size_t eqPos = assignment.find('=');

        if (eqPos == std::string::npos) {
            std::cout << "Error: invalid SET syntax\n";
            return;
        }

        std::string colName = trim(assignment.substr(0, eqPos));
        std::string value = trim(assignment.substr(eqPos + 1));

        int idx = findColumnIndex(schema, colName);

        if (idx == -1) {
            std::cout << "Error: column does not exist\n";
            return;
        }

        if (value == "NULL") {
            if (schema[idx].notNull || schema[idx].indexed) {
                std::cout << "Error: NULL in NOT_NULL/INDEXED column " << colName << "\n";
                return;
            }
        } else if (schema[idx].type == "int") {
            if (!isIntegerValue(value)) {
                std::cout << "Error: invalid int value\n";
                return;
            }
        } else if (schema[idx].type == "string") {
            if (value.size() < 2 || value.front() != '"' || value.back() != '"') {
                std::cout << "Error: invalid string value\n";
                return;
            }

            value = value.substr(1, value.size() - 2);
            value = "pool:" + std::to_string(pool.intern(value));
        }

        setIndexes.push_back(idx);
        setValues.push_back(value);
    }

    Condition cond;
    std::string error;

    if (!parseCondition(wherePart, cond, error)) {
        std::cout << "Error: " << error << "\n";
        return;
    }

    std::vector<std::vector<std::string>> newRows = rows;
    int updatedCount = 0;

    for (size_t r = 0; r < newRows.size(); r++) {
        std::string errorText;

        bool matched = rowMatchesCondition(newRows[r], schema, cond, errorText);

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

    std::cout << updatedCount << " row(s) updated\n";
}

