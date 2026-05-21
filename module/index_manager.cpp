#include "index_manager.h"

IndexManager::IndexManager(int order)
    : order(order)
{
}

std::string IndexManager::valueToKey(const Value& v) const {
    if (v.type == Value::INT) {
        return std::to_string(v.intValue);
    }
    if (v.type == Value::STRING) {
        return v.stringValue;
    }
    return "";
}

void IndexManager::createIndex(const std::string& table,
                               const std::string& column,
                               const std::string& type)
{
    if (indexes[table].count(column) == 0) {
        indexes[table][column] = new BPlusTree(order, type);
    }
}

void IndexManager::insertKey(const std::string& table,
                             const std::string& column,
                             const Value& value,
                             size_t rowId)
{
    if (!hasIndex(table, column)) return;

    std::string key = valueToKey(value);
    indexes[table][column]->insert(key, rowId);
}

void IndexManager::deleteKey(const std::string& table,
                             const std::string& column,
                             const Value& value)
{
    if (!hasIndex(table, column)) return;

    std::string key = valueToKey(value);
    indexes[table][column]->remove(key);
}

std::vector<size_t> IndexManager::find(const std::string& table,
                                       const std::string& column,
                                       const Value& value)
{
    if (!hasIndex(table, column)) return {};

    std::string key = valueToKey(value);
    return indexes[table][column]->searchAll(key);
}

std::vector<size_t> IndexManager::findRange(const std::string& table,
                                            const std::string& column,
                                            const Value& left,
                                            const Value& right)
{
    if (!hasIndex(table, column)) return {};

    return indexes[table][column]->rangeSearch(
        valueToKey(left),
        valueToKey(right)
    );
}

bool IndexManager::hasIndex(const std::string& table,
                            const std::string& column) const
{
    auto it = indexes.find(table);
    if (it == indexes.end()) return false;

    return it->second.count(column) > 0;
}

void IndexManager::saveIndex(const std::string& table,
                             const std::string& column,
                             const std::string& path)
{
    if (!hasIndex(table, column)) return;

    std::ofstream out(path);
    if (!out.is_open()) return;

    BPlusTree* tree = indexes[table][column];
    BPlusNode* leaf = tree->getRoot();

    // спускаемся до самого левого листа
    while (leaf && !leaf->isLeaf) {
        leaf = leaf->children[0];
    }

    // идём по листьям через next
    while (leaf) {
        for (size_t i = 0; i < leaf->keys.size(); i++) {
            out << leaf->keys[i] << "|" << leaf->values[i] << "\n";
        }
        leaf = leaf->next;
    }
}


void IndexManager::loadIndex(const std::string& table,
                             const std::string& column,
                             const std::string& type,
                             const std::string& path)
{
    createIndex(table, column, type);

    std::ifstream in(path);
    if (!in.is_open()) return;

    std::string line;
    while (std::getline(in, line)) {
        size_t pos = line.find('|');
        if (pos == std::string::npos) continue;

        std::string key = line.substr(0, pos);
        size_t rowId = std::stoull(line.substr(pos + 1));

        indexes[table][column]->insert(key, rowId);
    }
}

Value IndexManager::makeValue(const std::string& raw, const std::string& type) const {
    Value v;

    if (raw == "NULL") {
        v.type = Value::NULL_VALUE;
        v.isNull = true;
        return v;
    }

    v.isNull = false;

    if (type == "int") {
        v.type = Value::INT;
        v.intValue = std::stoi(raw);
    } else {
        v.type = Value::STRING;
        v.stringValue = raw;
    }

    return v;
}

void IndexManager::buildIndexes(const std::filesystem::path& tablePath,
                                const std::vector<Column>& schema) {
    activeTable = tablePath.filename().string();

    for (const auto& col : schema) {
        if (col.indexed) {
            createIndex(activeTable, col.name, col.type);
        }
    }

    std::ifstream in(tablePath / "data.txt");
    if (!in.is_open()) {
        return;
    }

    std::string line;
    size_t rowId = 0;

    while (std::getline(in, line)) {
        std::vector<std::string> values;
        std::string current;

        for (char ch : line) {
            if (ch == '|') {
                values.push_back(current);
                current.clear();
            } else {
                current += ch;
            }
        }

        values.push_back(current);

        for (size_t i = 0; i < schema.size() && i < values.size(); i++) {
            if (schema[i].indexed) {
                Value v = makeValue(values[i], schema[i].type);
                insertKey(activeTable, schema[i].name, v, rowId);
            }
        }

        rowId++;
    }
}

bool IndexManager::hasIndex(const std::string& column) const {
    return hasIndex(activeTable, column);
}

bool IndexManager::checkUnique(const std::string& column,
                               const std::string& rawValue) {
    if (!hasIndex(activeTable, column)) {
        return true;
    }

    std::string key = rawValue;

    auto itTable = indexes.find(activeTable);
    if (itTable == indexes.end()) {
        return true;
    }

    auto itCol = itTable->second.find(column);
    if (itCol == itTable->second.end()) {
        return true;
    }

    size_t dummy;
    return !itCol->second->search(key, dummy);
}

bool IndexManager::findRowId(const std::string& column,
                             const std::string& rawValue,
                             size_t& rowId) {
    if (!hasIndex(activeTable, column)) {
        return false;
    }

    auto result = indexes[activeTable][column]->searchAll(rawValue);

    if (result.empty()) {
        return false;
    }

    rowId = result[0];
    return true;
}

void IndexManager::insertKey(const std::string& column,
                             const std::string& rawValue,
                             size_t rowId) {
    if (!hasIndex(activeTable, column)) {
        return;
    }

    indexes[activeTable][column]->insert(rawValue, rowId);
}

std::vector<size_t> IndexManager::findRange(
    const std::string& column,
    const std::string& left,
    const std::string& right
) {
    if (!hasIndex(activeTable, column)) {
        return {};
    }

    return indexes[activeTable][column]->rangeSearch(left, right);
}

void IndexManager::saveIndexes() {
    std::filesystem::path dir = activeTable;

    for (const auto& [column, tree] : indexes[activeTable]) {
        std::ofstream out(dir / (column + ".idx"));

        if (!out) {
            continue;
        }

        auto values = tree->getAllKeyValues();

        for (const auto& pair : values) {
            out << pair.first << "|" << pair.second << "\n";
        }
    }
}

void IndexManager::loadIndexes(
    const std::filesystem::path& tablePath,
    const std::vector<Column>& schema
) {
    activeTable = tablePath.string();

    indexes[activeTable].clear();

    for (const auto& col : schema) {
        if (!col.indexed) {
            continue;
        }

        indexes[activeTable][col.name] =
            new BPlusTree(order, col.type);

        std::ifstream in(tablePath / (col.name + ".idx"));

        if (!in) {
            continue;
        }

        std::string line;

        while (std::getline(in, line)) {
            size_t sep = line.find('|');

            if (sep == std::string::npos) {
                continue;
            }

            std::string key = line.substr(0, sep);
            size_t rowId =
                static_cast<size_t>(std::stoull(line.substr(sep + 1)));

            indexes[activeTable][col.name]->insert(key, rowId);
        }
    }
}