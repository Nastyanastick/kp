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

public:
    BPlusTree(int order, const std::string& keyType);
    ~BPlusTree();

    bool search(const std::string& key, size_t& value) const;
    bool insert(const std::string& key, size_t value);


    BPlusNode* getRoot() const;
};