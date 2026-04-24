#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <filesystem>
#include <algorithm>
#include <cctype>
#include <set>
#include "module/index_manager.h"

namespace fs = std::filesystem;

const std::string DATA_DIR = "data";
std::string currentDatabase; // текущая база данных

void deleteFrom(const std::string& body);

struct Column { // один столбец таблицы
    std::string name;
    std::string type; // int или string
    bool notNull = false;
    bool indexed = false;
};

struct Condition {
    std::string columnName;
    std::string op;
    std::string value;
};

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

bool rowMatchesCondition(
    const std::vector<std::string>& row,
    const std::vector<Column>& schema,
    const Condition& cond,
    std::string& error
) {
    int columnIndex = -1;

    for (size_t i = 0; i < schema.size(); i++) {
        if (schema[i].name == cond.columnName) {
            columnIndex = static_cast<int>(i);
            break;
        }
    }

    if (columnIndex == -1) {
        error = "unknown column in WHERE: " + cond.columnName;
        return false;
    }

    std::string type = schema[columnIndex].type;
    std::string right = cond.value;

    if (type == "int") {
        if (!std::all_of(right.begin(), right.end(), ::isdigit)) {
            error = "expected int in WHERE for column " + cond.columnName;
            return false;
        }
    } else if (type == "string") {
        if (right.size() < 2 || right.front() != '"' || right.back() != '"') {
            error = "expected string literal in WHERE for column " + cond.columnName;
            return false;
        }
        right = right.substr(1, right.size() - 2);
    }

    std::string left = (columnIndex < static_cast<int>(row.size())) ? row[columnIndex] : "NULL";
    return compareValues(left, cond.op, right, type);
}





std::vector<std::string> splitCommands(const std::string& text) {
    std::vector<std::string> commands;
    std::string current;

    for (char ch : text) {
        current += ch;
        if (ch == ';') {
            commands.push_back(current);
            current.clear();
        }
    }

    return commands;
}

std::string trim(const std::string& s) {
    size_t start = 0;
    while (start < s.size() && std::isspace(static_cast<unsigned char>(s[start]))) {
        start++;
    }

    size_t end = s.size();
    while (end > start && std::isspace(static_cast<unsigned char>(s[end - 1]))) {
        end--;
    }

    return s.substr(start, end - start);
}

bool parseCondition(const std::string& text, Condition& cond, std::string& error) {
    std::vector<std::string> operators = {"==", "!=", "<=", ">=", "<", ">"};

    for (const auto& op : operators) {
        size_t pos = text.find(op);
        if (pos != std::string::npos) {
            cond.columnName = trim(text.substr(0, pos));
            cond.op = op;
            cond.value = trim(text.substr(pos + op.size()));

            if (cond.columnName.empty() || cond.value.empty()) {
                error = "invalid WHERE condition";
                return false;
            }

            return true;
        }
    }

    error = "unsupported WHERE condition";
    return false;
}

std::string toUpper(const std::string& s) {
    std::string result = s;
    for (char& ch : result) {
        ch = static_cast<char>(std::toupper(static_cast<unsigned char>(ch)));
    }
    return result;
}

bool isValidName(const std::string& name) {
    if (name.empty()) {
        return false;
    }

    if (std::isdigit(static_cast<unsigned char>(name[0]))) {
        return false;
    }

    for (char ch : name) {
        if (!(std::isalnum(static_cast<unsigned char>(ch)) || ch == '_')) {
            return false;
        }
    }

    return true;
}

void ensureDataDirectory() {
    if (!fs::exists(DATA_DIR)) {
        fs::create_directory(DATA_DIR);
    }
}

void createDatabase(const std::string& dbName) {
    if (!isValidName(dbName)) {
        std::cout << "Error: invalid database name\n";
        return;
    }

    ensureDataDirectory();

    fs::path dbPath = fs::path(DATA_DIR) / dbName;

    if (fs::exists(dbPath)) {
        std::cout << "Error: database already exists\n";
        return;
    }

    fs::create_directory(dbPath);
    std::cout << "Database '" << dbName << "' created\n";
}

void dropDatabase(const std::string& dbName) {
    if (!isValidName(dbName)) {
        std::cout << "Error: invalid database name\n";
        return;
    }

    fs::path dbPath = fs::path(DATA_DIR) / dbName;

    if (!fs::exists(dbPath)) {
        std::cout << "Error: database does not exist\n";
        return;
    }

    fs::remove_all(dbPath);

    if (currentDatabase == dbName) {
        currentDatabase.clear();
    }

    std::cout << "Database '" << dbName << "' dropped\n";
}

void useDatabase(const std::string& dbName) {
    if (!isValidName(dbName)) {
        std::cout << "Error: invalid database name\n";
        return;
    }

    fs::path dbPath = fs::path(DATA_DIR) / dbName;

    if (!fs::exists(dbPath)) {
        std::cout << "Error: database does not exist\n";
        return;
    }

    currentDatabase = dbName;
    std::cout << "Using database '" << dbName << "'\n";
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

bool saveSchema(const fs::path& tablePath, const std::vector<Column>& columns) { // сохраняет описание таблицы в файл 
    std::ofstream out(tablePath / "schema.txt");
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

std::vector<Column> loadSchema(const fs::path& tablePath) {
    std::vector<Column> columns;
    std::ifstream in(tablePath / "schema.txt");

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

std::vector<std::string> splitValues(const std::string& text) {
    std::vector<std::string> values;
    std::string current;
    bool inQuotes = false;

    for (char ch : text) {
        if (ch == '"') {
            inQuotes = !inQuotes;
            current += ch;
        } else if (ch == ',' && !inQuotes) {
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

std::vector<std::vector<std::string>> loadData(const fs::path& tablePath) {
    std::vector<std::vector<std::string>> rows;

    std::ifstream in(tablePath / "data.txt");
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

void insertInto(const std::string& body) {
    if (currentDatabase.empty()) {
        std::cout << "Error: no database selected\n";
        return;
    }

    size_t posValues = toUpper(body).find("VALUE");
    if (posValues == std::string::npos) {
        std::cout << "Error: invalid INSERT syntax\n";
        return;
    }

    std::string left = trim(body.substr(0, posValues));
    std::string right = trim(body.substr(posValues + 5));

    size_t open1 = left.find('(');
    size_t close1 = left.find(')');

    if (open1 == std::string::npos || close1 == std::string::npos) {
        std::cout << "Error: invalid column list\n";
        return;
    }

    std::string tableName = trim(left.substr(0, open1));
    std::string columnsText = left.substr(open1 + 1, close1 - open1 - 1);

    fs::path tablePath = fs::path(DATA_DIR) / currentDatabase / tableName;
    if (!fs::exists(tablePath)) {
        std::cout << "Error: table does not exist\n";
        return;
    }

    auto schema = loadSchema(tablePath);

    IndexManager indexManager;
    indexManager.buildIndexes(tablePath, schema);

    auto columnNames = splitByCommaTopLevel(columnsText);

    size_t open2 = right.find('(');
    size_t close2 = right.find(')');

    std::string valuesText = right.substr(open2 + 1, close2 - open2 - 1);
    auto values = splitValues(valuesText);

    if (columnNames.size() != values.size()) {
        std::cout << "Error: columns and values count mismatch\n";
        return;
    }

    std::vector<std::string> row(schema.size(), "NULL");

    for (size_t i = 0; i < columnNames.size(); i++) {
        std::string colName = trim(columnNames[i]);
        std::string val = trim(values[i]);

        int index = -1;
        for (size_t j = 0; j < schema.size(); j++) {
            if (schema[j].name == colName) {
                index = j;
                break;
            }
        }

        if (index == -1) {
            std::cout << "Error: unknown column " << colName << "\n";
            return;
        }

        if (schema[index].type == "int") {
            if (!std::all_of(val.begin(), val.end(), ::isdigit)) {
                std::cout << "Error: expected int for " << colName << "\n";
                return;
            }
        } else if (schema[index].type == "string") {
            if (val.size() < 2 || val.front() != '"' || val.back() != '"') {
                std::cout << "Error: invalid string for " << colName << "\n";
                return;
            }
            val = val.substr(1, val.size() - 2);
        }

        row[index] = val;

        if (schema[index].indexed) {
            if (!indexManager.checkUnique(colName, val)) {
                std::cout << "Error: duplicate value for INDEXED column " << colName << "\n";
                return;
            }
        }
    }

    for (size_t i = 0; i < schema.size(); i++) {
        if (schema[i].notNull && row[i] == "NULL") {
            std::cout << "Error: NULL in NOT_NULL column " << schema[i].name << "\n";
            return;
        }
    }

    auto oldRows = loadData(tablePath);
    size_t newRowId = oldRows.size();
    std::ofstream out(tablePath / "data.txt", std::ios::app);
    for (size_t i = 0; i < row.size(); i++) {
        if (i > 0) out << "|";
        out << row[i];
    }
    out << "\n";
    for (size_t i = 0; i < schema.size(); i++) {
        if (schema[i].indexed && row[i] != "NULL") {
            indexManager.insertKey(schema[i].name, row[i], newRowId);
        }
    }

    std::cout << "1 row inserted\n";
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

    fs::path tablePath = fs::path(DATA_DIR) / currentDatabase / tableName;
    if (fs::exists(tablePath)) {
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

    if (!fs::create_directory(tablePath)) {
        std::cout << "Error: failed to create table directory\n";
        return;
    }

    if (!saveSchema(tablePath, columns)) {
        fs::remove_all(tablePath);
        std::cout << "Error: failed to save schema\n";
        return;
    }

    std::cout << "Table '" << tableName << "' created\n";
}

void dropTable(const std::string& tableName) {
    if (currentDatabase.empty()) {
        std::cout << "Error: no database selected\n";
        return;
    }

    if (!isValidName(tableName)) {
        std::cout << "Error: invalid table name\n";
        return;
    }

    fs::path tablePath = fs::path(DATA_DIR) / currentDatabase / tableName;

    if (!fs::exists(tablePath)) {
        std::cout << "Error: table does not exist\n";
        return;
    }

    fs::remove_all(tablePath);
    std::cout << "Table '" << tableName << "' dropped\n";
}

void deleteFrom(const std::string& body) {
    if (currentDatabase.empty()) {
        std::cout << "Error: no database selected\n";
        return;
    }

    std::string upperBody = toUpper(body);
    size_t wherePos = upperBody.find(" WHERE ");

    if (wherePos == std::string::npos) {
        std::cout << "Error: DELETE requires WHERE\n";
        return;
    }

    std::string tableName = trim(body.substr(0, wherePos));
    std::string condText = trim(body.substr(wherePos + 7));

    std::string error;
    Condition cond;
    if (!parseCondition(condText, cond, error)) {
        std::cout << "Error: " << error << "\n";
        return;
    }

    fs::path tablePath = fs::path(DATA_DIR) / currentDatabase / tableName;
    if (!fs::exists(tablePath)) {
        std::cout << "Error: table does not exist\n";
        return;
    }

    auto schema = loadSchema(tablePath);
    auto rows = loadData(tablePath);

    std::vector<std::vector<std::string>> keptRows;
    int deletedCount = 0;

    for (const auto& row : rows) {
        std::string matchError;
        bool matched = rowMatchesCondition(row, schema, cond, matchError);

        if (!matchError.empty()) {
            std::cout << "Error: " << matchError << "\n";
            return;
        }

        if (matched) {
            deletedCount++;
        } else {
            keptRows.push_back(row);
        }
    }

    std::ofstream out(tablePath / "data.txt", std::ios::trunc);
    if (!out) {
        std::cout << "Error: failed to rewrite data file\n";
        return;
    }

    for (const auto& row : keptRows) {
        for (size_t i = 0; i < row.size(); i++) {
            if (i > 0) out << "|";
            out << row[i];
        }
        out << "\n";
    }

    std::cout << deletedCount << " row(s) deleted\n";
}

void selectFrom(const std::string& body) {
    if (currentDatabase.empty()) {
        std::cout << "Error: no database selected\n";
        return;
    }

    std::string upperBody = toUpper(body);
    std::string tableName;
    bool hasWhere = false;
    Condition cond;

    size_t wherePos = upperBody.find(" WHERE ");

    if (wherePos == std::string::npos) {
        tableName = trim(body);
    } else {
        tableName = trim(body.substr(0, wherePos));
        std::string condText = trim(body.substr(wherePos + 7));

        std::string error;
        if (!parseCondition(condText, cond, error)) {
            std::cout << "Error: " << error << "\n";
            return;
        }

        hasWhere = true;
    }

    fs::path tablePath = fs::path(DATA_DIR) / currentDatabase / tableName;

    if (!fs::exists(tablePath)) {
        std::cout << "Error: table does not exist\n";
        return;
    }

    auto schema = loadSchema(tablePath);
    auto rows = loadData(tablePath);

    IndexManager indexManager;
    indexManager.buildIndexes(tablePath, schema);

    bool useIndex = false;
    size_t indexedRowId = 0;

    if (hasWhere && cond.op == "==" && indexManager.hasIndex(cond.columnName)) {
        std::string key = cond.value;

        int columnIndex = -1;
        for (size_t i = 0; i < schema.size(); i++) {
            if (schema[i].name == cond.columnName) {
                columnIndex = static_cast<int>(i);
                break;
            }
        }

        if (columnIndex != -1) {
            if (schema[columnIndex].type == "string") {
                if (key.size() < 2 || key.front() != '"' || key.back() != '"') {
                    std::cout << "Error: expected string literal in WHERE for column " << cond.columnName << "\n";
                    return;
                }
                key = key.substr(1, key.size() - 2);
            }

            if (indexManager.findRowId(cond.columnName, key, indexedRowId)) {
                useIndex = true;
            } else {
                std::cout << "[\n]\n";
                return;
            }
        }
    }

    std::cout << "[\n";
bool firstPrinted = true;

for (size_t rowIndex = 0; rowIndex < rows.size(); rowIndex++) {
    if (useIndex && rowIndex != indexedRowId) {
        continue;
    }

    const auto& row = rows[rowIndex];

    if (hasWhere && !useIndex) {
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

    for (size_t j = 0; j < schema.size(); j++) {
        std::cout << "\"" << schema[j].name << "\": ";

        std::string val = (j < row.size()) ? row[j] : "NULL";

        if (val == "NULL") {
            std::cout << "null";
        } else if (schema[j].type == "int") {
            std::cout << val;
        } else {
            std::cout << "\"" << val << "\"";
        }

        if (j + 1 < schema.size()) {
            std::cout << ", ";
        }
    }

    std::cout << "}";
    firstPrinted = false;
}

    std::cout << "\n]\n";
}

void processCommand(const std::string& command) {
    std::string cleaned = trim(command);

    if (cleaned.empty()) {
        return;
    }

    if (cleaned.back() == ';') {
        cleaned.pop_back();
        cleaned = trim(cleaned);
    }

    std::string upper = toUpper(cleaned);

    if (upper.rfind("CREATE DATABASE ", 0) == 0) {
        std::string dbName = trim(cleaned.substr(16));
        createDatabase(dbName);
    } else if (upper.rfind("DROP DATABASE ", 0) == 0) {
        std::string dbName = trim(cleaned.substr(14));
        dropDatabase(dbName);
    } else if (upper.rfind("USE ", 0) == 0) {
        std::string dbName = trim(cleaned.substr(4));
        useDatabase(dbName);
    } else if (upper.rfind("CREATE TABLE ", 0) == 0) {
        std::string body = trim(cleaned.substr(13));
        createTable(body);
    } else if (upper.rfind("DROP TABLE ", 0) == 0) {
        std::string tableName = trim(cleaned.substr(11));
        dropTable(tableName);
    } else if (upper.rfind("INSERT INTO ", 0) == 0) {
        std::string body = trim(cleaned.substr(12));
        insertInto(body);
    } else if (upper.rfind("SELECT * FROM ", 0) == 0) {
        std::string body = trim(cleaned.substr(14));
        selectFrom(body);
    } else if (upper.rfind("DELETE FROM ", 0) == 0) {
        std::string body = trim(cleaned.substr(12));
        deleteFrom(body);
    } else {
        std::cout << "Error: unknown command\n";
    }
}

void runInteractive() {
    std::cout << "Interactive mode. Enter commands ending with ';'\n";

    std::string line;
    std::string buffer;

    while (true) {
        std::getline(std::cin, line);
        if (!std::cin) {
            break;
        }

        buffer += line + '\n';

        auto commands = splitCommands(buffer);

        size_t processedLength = 0;
        for (const auto& cmd : commands) {
            processCommand(cmd);
            processedLength += cmd.size();
        }

        buffer = buffer.substr(processedLength);
    }
}

void runBatch(const std::string& filename) {
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Error: cannot open file " << filename << std::endl;
        return;
    }

    std::stringstream ss;
    ss << file.rdbuf();
    std::string text = ss.str();

    auto commands = splitCommands(text);
    for (const auto& cmd : commands) {
        processCommand(cmd);
    }
}

int main(int argc, char* argv[]) {
    try {
        ensureDataDirectory();

        if (argc == 1) {
            runInteractive();
        } else if (argc == 2) {
            runBatch(argv[1]);
        } else {
            std::cerr << "Usage:\n";
            std::cerr << "  ./prog\n";
            std::cerr << "  ./prog script.txt\n";
            return 1;
        }
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "Unknown fatal error\n";
        return 1;
    }

    return 0;
}