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
    // создаём пустое дерево
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
