#include "index_manager.h"
#include <fstream>
#include <sstream>
#include <algorithm>

struct Column {
    std::string name;
    std::string type;
    bool notNull = false;
    bool indexed = false;
};

static std::vector<std::vector<std::string>> loadDataRows(const fs::path& tablePath) {
    std::vector<std::vector<std::string>> rows;
    std::ifstream in(tablePath / "data.txt");
    std::string line;

    while (std::getline(in, line)) {
        std::vector<std::string> row;
        std::string cur;

        for (char ch : line) {
            if (ch == '|') {
                row.push_back(cur);
                cur.clear();
            } else {
                cur += ch;
            }
        }

        row.push_back(cur);
        rows.push_back(row);
    }

    return rows;
}

void IndexManager::buildIndexes(const fs::path& tablePath, const std::vector<Column>& schema) {
    indexes.clear();

    for (size_t i = 0; i < schema.size(); i++) {
        if (schema[i].indexed) {
            indexes[schema[i].name] = std::make_unique<BPlusTree>(4, schema[i].type);
        }
    }

    auto rows = loadDataRows(tablePath);

    for (size_t rowId = 0; rowId < rows.size(); rowId++) {
        for (size_t col = 0; col < schema.size(); col++) {
            if (!schema[col].indexed) continue;
            if (col >= rows[rowId].size()) continue;
            if (rows[rowId][col] == "NULL") continue;

            indexes[schema[col].name]->insert(rows[rowId][col], rowId);
        }
    }
}

bool IndexManager::findRowId(const std::string& columnName, const std::string& key, size_t& rowId) const {
    auto it = indexes.find(columnName);
    if (it == indexes.end()) {
        return false;
    }

    return it->second->search(key, rowId);
}

bool IndexManager::checkUnique(const std::string& columnName, const std::string& key) const {
    auto it = indexes.find(columnName);
    if (it == indexes.end()) return true;

    size_t dummy;
    return !it->second->search(key, dummy);
}

void IndexManager::insertKey(const std::string& columnName, const std::string& key, size_t rowId) {
    auto it = indexes.find(columnName);
    if (it != indexes.end()) {
        it->second->insert(key, rowId);
    }
}

bool IndexManager::hasIndex(const std::string& columnName) const {
    return indexes.find(columnName) != indexes.end();
}