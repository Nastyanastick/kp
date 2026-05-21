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

std::vector<size_t> BPlusTree::searchAll(const std::string& key) const {
    std::vector<size_t> result;

    BPlusNode* leaf = findLeaf(key);
    if (!leaf) {
        return result;
    }

    // идём по листу и дальше по next, пока ключи равны
    BPlusNode* current = leaf;
    while (current) {
        for (size_t i = 0; i < current->keys.size(); ++i) {
            int cmp = compareKeys(current->keys[i], key);
            if (cmp == 0) {
                result.push_back(current->values[i]);
            } else if (cmp > 0) {
                // дальше в этом и следующих листах ключи только больше
                return result;
            }
        }
        current = current->next;
    }

    return result;
}

std::vector<size_t> BPlusTree::rangeSearch(const std::string& left, const std::string& right) const {
    std::vector<size_t> result;

    if (!root) {
        return result;
    }

    // начинаем с листа, где мог бы быть left
    BPlusNode* current = findLeaf(left);
    if (!current) {
        return result;
    }

    while (current) {
        for (size_t i = 0; i < current->keys.size(); ++i) {
            const std::string& k = current->keys[i];

            if (compareKeys(k, right) >= 0) {
                // по заданию [left, right) — правая граница не включается
                return result;
            }

            if (compareKeys(k, left) >= 0) {
                result.push_back(current->values[i]);
            }
        }
        current = current->next;
    }

    return result;
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

bool BPlusTree::remove(const std::string& key) {
    if (!root) return false;

    BPlusNode* leaf = findLeaf(key);
    if (!leaf) return false;

    // Проверяем, есть ли ключ
    bool found = false;
    for (auto& k : leaf->keys) {
        if (compareKeys(k, key) == 0) {
            found = true;
            break;
        }
    }
    if (!found) return false;

    // Удаляем из листа
    removeFromLeaf(leaf, key);

    // Если лист — корень и он пустой
    if (leaf == root && leaf->keys.empty()) {
        delete root;
        root = nullptr;
        return true;
    }

    // Если произошёл underflow — исправляем
    if (leaf->keys.size() < (order - 1) / 2) {
        handleUnderflow(leaf);
    }

    return true;
}

void BPlusTree::handleUnderflow(BPlusNode* node) {
    if (node == root) return;

    BPlusNode* parent = node->parent;

    int index = -1;
    for (int i = 0; i < (int)parent->children.size(); i++) {
        if (parent->children[i] == node) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        throw std::runtime_error("Internal error: node not found in parent");
    }

    BPlusNode* leftSibling  = (index > 0) ? parent->children[index - 1] : nullptr;
    BPlusNode* rightSibling = (index + 1 < (int)parent->children.size()) ? parent->children[index + 1] : nullptr;

    int minKeys = (order - 1) / 2;

    // 1) Пытаемся занять у левого соседа
    if (leftSibling && leftSibling->keys.size() > minKeys) {
        borrowFromLeft(node, leftSibling, index);
        return;
    }

    // 2) Пытаемся занять у правого соседа
    if (rightSibling && rightSibling->keys.size() > minKeys) {
        borrowFromRight(node, rightSibling, index);
        return;
    }

    // 3) Если borrow невозможен — делаем merge
    if (leftSibling) {
        mergeWithLeft(node, leftSibling, index);
        return;
    }

    if (rightSibling) {
        mergeWithRight(node, rightSibling, index);
        return;
    }

}

void BPlusTree::mergeWithLeft(BPlusNode* node, BPlusNode* leftSibling, int parentIndex) {
    // Переносим все ключи node в конец leftSibling
    leftSibling->keys.insert(leftSibling->keys.end(),
                             node->keys.begin(), node->keys.end());
    leftSibling->values.insert(leftSibling->values.end(),
                               node->values.begin(), node->values.end());

    // Обновляем next-цепочку листьев
    leftSibling->next = node->next;

    // Удаляем node из родителя
    BPlusNode* parent = node->parent;

    parent->keys.erase(parent->keys.begin() + parentIndex - 1);
    parent->children.erase(parent->children.begin() + parentIndex);

    delete node;

    // Если родитель стал пустым — поднимаем корень
    if (parent == root && parent->keys.empty()) {
        root = leftSibling;
        leftSibling->parent = nullptr;
        delete parent;
        return;
    }

    // Если у родителя underflow — обрабатываем
    if (parent->keys.size() < (order - 1) / 2) {
        handleUnderflow(parent);
    }
}

void BPlusTree::mergeWithRight(BPlusNode* node, BPlusNode* rightSibling, int parentIndex) {
    // Переносим все ключи rightSibling в конец node
    node->keys.insert(node->keys.end(),
                      rightSibling->keys.begin(), rightSibling->keys.end());
    node->values.insert(node->values.end(),
                        rightSibling->values.begin(), rightSibling->values.end());

    // Обновляем next-цепочку
    node->next = rightSibling->next;

    BPlusNode* parent = node->parent;

    parent->keys.erase(parent->keys.begin() + parentIndex);
    parent->children.erase(parent->children.begin() + parentIndex + 1);

    delete rightSibling;

    if (parent == root && parent->keys.empty()) {
        root = node;
        node->parent = nullptr;
        delete parent;
        return;
    }

    if (parent->keys.size() < (order - 1) / 2) {
        handleUnderflow(parent);
    }
}


void BPlusTree::removeFromLeaf(BPlusNode* leaf, const std::string& key) {
    for (size_t i = 0; i < leaf->keys.size(); ++i) {
        if (compareKeys(leaf->keys[i], key) == 0) {
            leaf->keys.erase(leaf->keys.begin() + i);
            leaf->values.erase(leaf->values.begin() + i);
            return;
        }
    }
}

void BPlusTree::borrowFromLeft(BPlusNode* node, BPlusNode* leftSibling, int parentIndex) {
    // Берём последний ключ левого соседа
    std::string borrowedKey = leftSibling->keys.back();
    size_t borrowedValue = leftSibling->values.back();

    leftSibling->keys.pop_back();
    leftSibling->values.pop_back();

    // Вставляем в начало текущего узла
    node->keys.insert(node->keys.begin(), borrowedKey);
    node->values.insert(node->values.begin(), borrowedValue);

    // Обновляем ключ в родителе
    node->parent->keys[parentIndex - 1] = node->keys.front();
}

void BPlusTree::borrowFromRight(BPlusNode* node, BPlusNode* rightSibling, int parentIndex) {
    // Берём первый ключ правого соседа
    std::string borrowedKey = rightSibling->keys.front();
    size_t borrowedValue = rightSibling->values.front();

    rightSibling->keys.erase(rightSibling->keys.begin());
    rightSibling->values.erase(rightSibling->values.begin());

    // Вставляем в конец текущего узла
    node->keys.push_back(borrowedKey);
    node->values.push_back(borrowedValue);

    // Обновляем ключ в родителе
    node->parent->keys[parentIndex] = rightSibling->keys.front();
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