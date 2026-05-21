#pragma once

#include <string>
#include <vector>

struct BPlusNode {
    bool isLeaf;
    std::vector<std::string> keys;
    std::vector<size_t> values; // ток дляя листьев
    std::vector<BPlusNode*> children; // ток для внутр 
    BPlusNode* next;
    BPlusNode* parent;

    BPlusNode(bool leaf);
};

class BPlusTree {
private:
    BPlusNode* root;
    int order;
    std::string keyType; // int или string

    int compareKeys(const std::string& a, const std::string& b) const;
    BPlusNode* findLeaf(const std::string& key) const;

    void splitLeaf(BPlusNode* leaf);
    void insertIntoParent(BPlusNode* left, const std::string& key, BPlusNode* right);
    void splitInternal(BPlusNode* node);

    void removeFromLeaf(BPlusNode* leaf, const std::string& key);
    void handleUnderflow(BPlusNode* node);
    void borrowFromLeft(BPlusNode* node, BPlusNode* leftSibling, int parentIndex);
    void borrowFromRight(BPlusNode* node, BPlusNode* rightSibling, int parentIndex);
    void mergeWithLeft(BPlusNode* node, BPlusNode* leftSibling, int parentIndex);
    void mergeWithRight(BPlusNode* node, BPlusNode* rightSibling, int parentIndex);


public:
    BPlusTree(int order, const std::string& keyType);
    ~BPlusTree();

    bool search(const std::string& key, size_t& value) const;
    bool insert(const std::string& key, size_t value);

    std::vector<size_t> searchAll(const std::string& key) const;
    std::vector<size_t> rangeSearch(const std::string& left, const std::string& right) const;

    bool remove(const std::string& key);

    BPlusNode* getRoot() const;
};