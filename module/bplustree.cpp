#include "bplustree.h"
#include <algorithm>
#include <stdexcept>

BPlusNode::BPlusNode(bool leaf)
    : isLeaf(leaf), next(nullptr), parent(nullptr) {}

BPlusTree::BPlusTree(int order, const std::string& keyType)
    : root(nullptr), order(order), keyType(keyType) {}

BPlusTree::~BPlusTree() {
}

int BPlusTree::compareKeys(const std::string& a, const std::string& b) const {
    if (keyType == "int") {
        int x = std::stoi(a);
        int y = std::stoi(b);

        if (x < y) return -1;
        if (x > y) return 1;
        return 0;
    }

    if (a < b) return -1;
    if (a > b) return 1;
    return 0;
}

BPlusNode* BPlusTree::findLeaf(const std::string& key) const {
    if (!root) {
        return nullptr;
    }

    BPlusNode* current = root;

    while (!current->isLeaf) {
        size_t i = 0;
        while (i < current->keys.size() && compareKeys(key, current->keys[i]) >= 0) {
            i++;
        }
        current = current->children[i];
    }

    return current;
}

bool BPlusTree::search(const std::string& key, size_t& value) const {
    BPlusNode* leaf = findLeaf(key);
    if (!leaf) {
        return false;
    }

    for (size_t i = 0; i < leaf->keys.size(); i++) {
        if (compareKeys(leaf->keys[i], key) == 0) {
            value = leaf->values[i];
            return true;
        }
    }

    return false;
}

bool BPlusTree::insert(const std::string& key, size_t value) {
    if (!root) {
        root = new BPlusNode(true);
        root->keys.push_back(key);
        root->values.push_back(value);
        return true;
    }

    BPlusNode* leaf = findLeaf(key);

    size_t dummy;
    if (search(key, dummy)) {
        return false;
    }

    size_t pos = 0;
    while (pos < leaf->keys.size() && compareKeys(leaf->keys[pos], key) < 0) {
        pos++;
    }

    leaf->keys.insert(leaf->keys.begin() + pos, key);
    leaf->values.insert(leaf->values.begin() + pos, value);

    if (leaf->keys.size() >= order) {
        splitLeaf(leaf);
    }

    return true;
}

BPlusNode* BPlusTree::getRoot() const {
    return root;
}

void BPlusTree::splitLeaf(BPlusNode* leaf) {
    int mid = leaf->keys.size() / 2;

    BPlusNode* newLeaf = new BPlusNode(true);

    newLeaf->keys.assign(leaf->keys.begin() + mid, leaf->keys.end());
    newLeaf->values.assign(leaf->values.begin() + mid, leaf->values.end());

    leaf->keys.resize(mid);
    leaf->values.resize(mid);

    newLeaf->next = leaf->next;
    leaf->next = newLeaf;

    newLeaf->parent = leaf->parent;

    std::string newKey = newLeaf->keys[0];

    insertIntoParent(leaf, newKey, newLeaf);
}

void BPlusTree::insertIntoParent(BPlusNode* left, const std::string& key, BPlusNode* right) {
    if (left->parent == nullptr) {
        BPlusNode* newRoot = new BPlusNode(false);
        newRoot->keys.push_back(key);
        newRoot->children.push_back(left);
        newRoot->children.push_back(right);

        left->parent = newRoot;
        right->parent = newRoot;

        root = newRoot;
        return;
    }

    BPlusNode* parent = left->parent;

    size_t pos = 0;
    while (pos < parent->children.size() && parent->children[pos] != left) {
        pos++;
    }

    parent->keys.insert(parent->keys.begin() + pos, key);
    parent->children.insert(parent->children.begin() + pos + 1, right);

    right->parent = parent;

    if (parent->keys.size() >= order) {
        splitInternal(parent);
    }
}

void BPlusTree::splitInternal(BPlusNode* node) {
    int mid = node->keys.size() / 2;

    std::string upKey = node->keys[mid];

    BPlusNode* newNode = new BPlusNode(false);

    newNode->keys.assign(node->keys.begin() + mid + 1, node->keys.end());
    node->keys.resize(mid);

    newNode->children.assign(node->children.begin() + mid + 1, node->children.end());
    node->children.resize(mid + 1);

    for (auto child : newNode->children) {
        child->parent = newNode;
    }

    newNode->parent = node->parent;

    insertIntoParent(node, upKey, newNode);
}