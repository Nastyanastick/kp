#pragma once

#include "bplustree.h"
#include <map>
#include <memory>
#include <string>
#include <vector>
#include <filesystem>

namespace fs = std::filesystem;

struct Column;

class IndexManager {
private:
    std::map<std::string, std::unique_ptr<BPlusTree>> indexes;

public:
    void buildIndexes(const fs::path& tablePath, const std::vector<Column>& schema);
    bool checkUnique(const std::string& columnName, const std::string& key) const;
    void insertKey(const std::string& columnName, const std::string& key, size_t rowId);
    bool hasIndex(const std::string& columnName) const;
    bool findRowId(const std::string& columnName, const std::string& key, size_t& rowId) const;
};