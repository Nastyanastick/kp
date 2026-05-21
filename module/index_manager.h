#pragma once

#include "bplustree.h"
#include "value.h"
#include <unordered_map>
#include <string>
#include <vector>

class IndexManager {
public:
    IndexManager(int order = 4);

    void createIndex(const std::string& table,
                     const std::string& column,
                     const std::string& type);

    void insertKey(const std::string& table,
                   const std::string& column,
                   const Value& value,
                   size_t rowId);

    void deleteKey(const std::string& table,
                   const std::string& column,
                   const Value& value);

    std::vector<size_t> find(const std::string& table,
                             const std::string& column,
                             const Value& value);

    std::vector<size_t> findRange(const std::string& table,
                                  const std::string& column,
                                  const Value& left,
                                  const Value& right);

    bool hasIndex(const std::string& table,
                  const std::string& column) const;

    void saveIndex(const std::string& table, const std::string& column,
               const std::string& path);

    void loadIndex(const std::string& table, const std::string& column,
               const std::string& type,
               const std::string& path);


private:
    std::string valueToKey(const Value& v) const;

    int order;

    // table -> column -> B+ tree
    std::unordered_map<std::string,
        std::unordered_map<std::string, BPlusTree*>> indexes;
};
