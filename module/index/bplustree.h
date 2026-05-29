#pragma once

#include <string>
#include <vector>

struct BPlusNode {
    bool isLeaf;
    std::vector<std::string> keys;
    std::vector<size_t> values;
    std::vector<BPlusNode*> children;
    BPlusNode* next;
    BPlusNode* parent;

    int pageId;

    BPlusNode(bool leaf);
};

class BPlusTree {
private:
    BPlusNode* root;
    int order;
    std::string keyType;

    int compareKeys(const std::string& a, const std::string& b) const;
    BPlusNode* findLeaf(const std::string& key) const;

    void splitLeaf(BPlusNode* leaf);
    void splitInternal(BPlusNode* node);
    void insertIntoParent(BPlusNode* left, const std::string& key, BPlusNode* right);

    void removeFromLeaf(BPlusNode* leaf, const std::string& key);
    void handleUnderflow(BPlusNode* node);
    void borrowFromLeft(BPlusNode* node, BPlusNode* leftSibling, int parentIndex);
    void borrowFromRight(BPlusNode* node, BPlusNode* rightSibling, int parentIndex);
    void mergeWithLeft(BPlusNode* node, BPlusNode* leftSibling, int parentIndex);
    void mergeWithRight(BPlusNode* node, BPlusNode* rightSibling, int parentIndex);

    void assignPageIds(BPlusNode* node, int& nextId);
    std::string nodeToJson(BPlusNode* node) const;
    void collectAllPages(BPlusNode* node, std::vector<std::string>& pages, int& maxPageId);

public:
    BPlusTree(int order, const std::string& keyType);
    ~BPlusTree();

    bool insert(const std::string& key, size_t value);
    bool remove(const std::string& key);

    bool search(const std::string& key, size_t& value) const;
    std::vector<size_t> searchAll(const std::string& key) const;
    std::vector<size_t> rangeSearch(const std::string& left, const std::string& right) const;

    BPlusNode* getRoot() const;

    std::vector<std::pair<std::string, size_t>> getAllKeyValues() const;

    void shiftRowIdsAfterDeleted(const std::vector<size_t>& deletedRowIds);

    void saveToJsonFile(const std::string& path, const std::string& column);
    bool loadFromJsonFile(const std::string& path);
    int getMaxPageId() const;
};
