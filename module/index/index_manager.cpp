#include "index_manager.h"

IndexManager::IndexManager(int order)
    : order(order)
{
}

std::string IndexManager::valueToKey(
    const Value& v
) const {
    if (v.type == Value::INT) {
        return std::to_string(v.intValue);
    }

    if (v.type == Value::STRING) {
        return v.stringValue;
    }

    return "";
}

void IndexManager::createIndex(
    const std::string& table,
    const std::string& column,
    const std::string& type
) {
    if (indexes[table].count(column) == 0) {
        indexes[table][column] =
            new BPlusTree(order, type);
    }

    if (!activeTablePath.empty() &&
        table == activeTable) {

        const std::filesystem::path idxPath =
            std::filesystem::path(activeTablePath) /
            (column + ".idx");

        indexes[table][column]->setStoragePath(
            idxPath.string(),
            column
        );
    }
}

void IndexManager::insertKey(
    const std::string& table,
    const std::string& column,
    const Value& value,
    const RecordID& recordId
) {
    if (!hasIndex(table, column)) {
        return;
    }

    const std::string key =
        valueToKey(value);

    indexes[table][column]->insert(
        key,
        recordId
    );
}

void IndexManager::deleteKey(
    const std::string& table,
    const std::string& column,
    const Value& value,
    const RecordID& recordId
) {
    if (!hasIndex(table, column)) {
        return;
    }

    const std::string key =
        valueToKey(value);

    indexes[table][column]->remove(key, recordId);
}

std::vector<RecordID> IndexManager::find(
    const std::string& table,
    const std::string& column,
    const Value& value
) {
    if (!hasIndex(table, column)) {
        return {};
    }

    const std::string key =
        valueToKey(value);

    return indexes[table][column]->searchAll(key);
}

std::vector<RecordID> IndexManager::findRange(
    const std::string& table,
    const std::string& column,
    const Value& left,
    const Value& right
) {
    if (!hasIndex(table, column)) {
        return {};
    }

    return indexes[table][column]->rangeSearch(
        valueToKey(left),
        valueToKey(right)
    );
}

bool IndexManager::hasIndex(
    const std::string& table,
    const std::string& column
) const {
    auto it = indexes.find(table);

    if (it == indexes.end()) {
        return false;
    }

    return it->second.count(column) > 0;
}

void IndexManager::saveIndex(
    const std::string& table,
    const std::string& column,
    const std::string& path
) {
    if (!hasIndex(table, column)) {
        return;
    }

    BPlusTree* tree =
        indexes[table][column];

    tree->saveToJsonFile(
        path,
        column
    );
}

void IndexManager::loadIndex(
    const std::string& table,
    const std::string& column,
    const std::string& type,
    const std::string& path
) {
    createIndex(
        table,
        column,
        type
    );

    indexes[table][column]->setStoragePath(
        path,
        column
    );

    if (!indexes[table][column]->loadFromJsonFile(path)) {
        return;
    }

    indexes[table][column]->setStoragePath(
        path,
        column
    );
}

Value IndexManager::makeValue(
    const std::string& raw,
    const std::string& type
) const {
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

void IndexManager::buildIndexes(
    const std::filesystem::path& tablePath,
    const std::vector<Column>& schema
) {
    activeTable =
        tablePath.filename().string();

    activeTablePath =
        tablePath.string();

    for (const auto& col : schema) {
        if (col.indexed) {
            createIndex(
                activeTable,
                col.name,
                col.type
            );
        }
    }

    TableStorage storage(
        tablePath.string()
    );

    if (!storage.exists()) {
        return;
    }

    const std::vector<RecordID> recordIds =
        storage.getAllRecordIds();

    for (const RecordID& recordId : recordIds) {
        std::string record;

        if (!storage.readRecord(
                recordId,
                record)) {
            continue;
        }

        std::vector<std::string> values;
        std::string current;

        for (char ch : record) {
            if (ch == '|') {
                values.push_back(current);
                current.clear();
            } else {
                current += ch;
            }
        }

        values.push_back(current);

        for (size_t i = 0;
             i < schema.size() &&
             i < values.size();
             ++i) {

            if (!schema[i].indexed) {
                continue;
            }

            Value v = makeValue(
                values[i],
                schema[i].type
            );

            insertKey(
                activeTable,
                schema[i].name,
                v,
                recordId
            );
        }
    }
}

bool IndexManager::hasIndex(
    const std::string& column
) const {
    return hasIndex(
        activeTable,
        column
    );
}

bool IndexManager::checkUnique(
    const std::string& column,
    const std::string& rawValue
) {
    if (!hasIndex(activeTable, column)) {
        return true;
    }

    const std::string key = rawValue;

    auto itTable =
        indexes.find(activeTable);

    if (itTable == indexes.end()) {
        return true;
    }

    auto itCol =
        itTable->second.find(column);

    if (itCol == itTable->second.end()) {
        return true;
    }

    RecordID dummy;

    return !itCol->second->search(
        key,
        dummy
    );
}

bool IndexManager::findRowId(
    const std::string& column,
    const std::string& rawValue,
    RecordID& recordId
) {
    if (!hasIndex(activeTable, column)) {
        return false;
    }

    const std::vector<RecordID> result =
        indexes[activeTable][column]->searchAll(
            rawValue
        );

    if (result.empty()) {
        return false;
    }

    recordId = result[0];

    return true;
}

void IndexManager::insertKey(
    const std::string& column,
    const std::string& rawValue,
    const RecordID& recordId
) {
    if (!hasIndex(activeTable, column)) {
        return;
    }

    indexes[activeTable][column]->insert(
        rawValue,
        recordId
    );
}

std::vector<RecordID>
IndexManager::findRange(
    const std::string& column,
    const std::string& left,
    const std::string& right
) {
    if (!hasIndex(activeTable, column)) {
        return {};
    }

    return indexes[activeTable][column]->rangeSearch(
        left,
        right
    );
}

void IndexManager::saveIndexes() {
    if (activeTable.empty()) {
        return;
    }

    const std::filesystem::path dir =
        activeTablePath;

    for (const auto& entry :
         indexes[activeTable]) {

        const std::string& column =
            entry.first;

        BPlusTree* tree =
            entry.second;

        const std::string idxPath =
            (dir / (column + ".idx")).string();

        tree->saveToJsonFile(
            idxPath,
            column
        );
    }
}

void IndexManager::loadIndexes(
    const std::filesystem::path& tablePath,
    const std::vector<Column>& schema
) {
    activeTable =
        tablePath.filename().string();

    activeTablePath =
        tablePath.string();

    indexes[activeTable].clear();

    for (const auto& col : schema) {
        if (!col.indexed) {
            continue;
        }

        const std::string idxPath =
            (tablePath / (col.name + ".idx")).string();

        BPlusTree* tree =
            new BPlusTree(
                order,
                col.type
            );

        tree->setStoragePath(
            idxPath,
            col.name
        );

        if (tree->loadFromJsonFile(idxPath)) {
            tree->setStoragePath(
                idxPath,
                col.name
            );
        }

        indexes[activeTable][col.name] =
            tree;
    }
}