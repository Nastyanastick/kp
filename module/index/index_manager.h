#pragma once

#include "bplustree.h"
#include "../core/record_id.h"
#include "../core/value.h"
#include "../core/table.h"
#include "../core/table_storage.h"

#include <filesystem>
#include <string>
#include <vector>
#include <map>
#include <fstream>

class IndexManager {
private:
    int order;

    std::map<
        std::string,
        std::map<std::string, BPlusTree*>
    > indexes;

    std::string activeTable;
    std::string activeTablePath;

    std::string valueToKey(const Value& v) const;

    Value makeValue(
        const std::string& raw,
        const std::string& type
    ) const;

public:
    explicit IndexManager(int order = 3);

    void buildIndexes(
        const std::filesystem::path& tablePath,
        const std::vector<Column>& schema
    );

    bool hasIndex(
        const std::string& column
    ) const;

    bool checkUnique(
        const std::string& column,
        const std::string& rawValue
    );

    std::vector<RecordID> findRange(
        const std::string& column,
        const std::string& left,
        const std::string& right
    );

    bool findRowId(
        const std::string& column,
        const std::string& rawValue,
        RecordID& recordId
    );

    void insertKey(
        const std::string& column,
        const std::string& rawValue,
        const RecordID& recordId
    );

    void createIndex(
        const std::string& table,
        const std::string& column,
        const std::string& type
    );

    void insertKey(
        const std::string& table,
        const std::string& column,
        const Value& value,
        const RecordID& recordId
    );

    void deleteKey(
        const std::string& table,
        const std::string& column,
        const Value& value,
        const RecordID& recordId
    );

    std::vector<RecordID> find(
        const std::string& table,
        const std::string& column,
        const Value& value
    );

    std::vector<RecordID> findRange(
        const std::string& table,
        const std::string& column,
        const Value& left,
        const Value& right
    );

    bool hasIndex(
        const std::string& table,
        const std::string& column
    ) const;

    void saveIndex(
        const std::string& table,
        const std::string& column,
        const std::string& path
    );

    void loadIndex(
        const std::string& table,
        const std::string& column,
        const std::string& type,
        const std::string& path
    );

    void saveIndexes();

    void loadIndexes(
        const std::filesystem::path& tablePath,
        const std::vector<Column>& schema
    );
};