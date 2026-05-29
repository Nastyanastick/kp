#include "bplustree.h"
#include <algorithm>
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <map>

BPlusNode::BPlusNode(bool leaf)
    : isLeaf(leaf), next(nullptr), parent(nullptr), pageId(-1) {}

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

std::vector<std::pair<std::string, size_t>>
BPlusTree::getAllKeyValues() const {
    std::vector<std::pair<std::string, size_t>> result;

    BPlusNode* current = root;

    if (!current) {
        return result;
    }

    while (!current->isLeaf) {
        current = current->children[0];
    }

    while (current) {
        for (size_t i = 0; i < current->keys.size(); i++) {
            result.push_back({
                current->keys[i],
                current->values[i]
            });
        }

        current = current->next;
    }

    return result;
}


void BPlusTree::assignPageIds(BPlusNode* node, int& nextId) {
    if (!node) return;
    
    node->pageId = nextId++;
    
    if (!node->isLeaf) {
        for (auto child : node->children) {
            assignPageIds(child, nextId);
        }
    }
}

std::string BPlusTree::nodeToJson(BPlusNode* node) const {
    std::stringstream ss;
    ss << "{";
    ss << "\"pageId\":" << node->pageId << ",";
    ss << "\"isLeaf\":" << (node->isLeaf ? "true" : "false") << ",";
    
    // keys
    ss << "\"keys\":[";
    for (size_t i = 0; i < node->keys.size(); i++) {
        if (i > 0) ss << ",";
        ss << "\"" << node->keys[i] << "\"";
    }
    ss << "],";
    
    if (node->isLeaf) {
        ss << "\"rowIds\":[";
        for (size_t i = 0; i < node->values.size(); i++) {
            if (i > 0) ss << ",";
            ss << node->values[i];
        }
        ss << "],";
        
        int nextLeafId = -1;
        if (node->next) {
            nextLeafId = node->next->pageId;
        }
        ss << "\"nextLeafPageId\":" << nextLeafId;
    } else {
        ss << "\"children\":[";
        for (size_t i = 0; i < node->children.size(); i++) {
            if (i > 0) ss << ",";
            ss << node->children[i]->pageId;
        }
        ss << "]";
    }
    
    ss << "}";
    return ss.str();
}

void BPlusTree::collectAllPages(BPlusNode* node, std::vector<std::string>& pages, int& maxPageId) {
    if (!node) return;
    
    maxPageId = std::max(maxPageId, node->pageId);
    
    pages.push_back(nodeToJson(node));
    
    if (!node->isLeaf) {
        for (auto child : node->children) {
            collectAllPages(child, pages, maxPageId);
        }
    }
}

void BPlusTree::saveToJsonFile(const std::string& path, const std::string& column) {
    if (!root) {
        std::ofstream out(path);
        if (!out.is_open()) {
            return;
        }

        out << "{\\n";
        out << "  \"type\": \"bplus_tree_index\",\\n";
        out << "  \"version\": 1,\\n";
        out << "  \"column\": \"" << column << "\",\\n";
        out << "  \"keyType\": \"" << keyType << "\",\\n";
        out << "  \"order\": " << order << ",\\n";
        out << "  \"rootPageId\": -1,\\n";
        out << "  \"nextPageId\": 1,\\n";
        out << "  \"pages\": []\\n";
        out << "}\\n";
        return;
    }
    
    int nextId = 1;
    assignPageIds(root, nextId);
    
    std::vector<std::string> pages;
    int maxPageId = 0;
    collectAllPages(root, pages, maxPageId);
    
    std::ofstream out(path);
    if (!out.is_open()) {
        return;
    }
    
    out << "{\n";
    out << "  \"type\": \"bplus_tree_index\",\n";
    out << "  \"version\": 1,\n";
    out << "  \"column\": \"" << column << "\",\n";
    out << "  \"keyType\": \"" << keyType << "\",\n";
    out << "  \"order\": " << order << ",\n";
    out << "  \"rootPageId\": " << (root ? root->pageId : -1) << ",\n";
    out << "  \"nextPageId\": " << (maxPageId + 1) << ",\n";
    out << "  \"pages\": [\n";
    
    for (size_t i = 0; i < pages.size(); i++) {
        out << "    " << pages[i];
        if (i < pages.size() - 1) {
            out << ",";
        }
        out << "\n";
    }
    
    out << "  ]\n";
    out << "}\n";
    out.close();
}

bool BPlusTree::loadFromJsonFile(const std::string& path) {
    std::ifstream in(path);
    if (!in.is_open()) {
        return false;
    }
    
    std::stringstream buffer;
    buffer << in.rdbuf();
    in.close();
    
    std::string json = buffer.str();
    
    if (json.find("\"type\": \"bplus_tree_index\"") == std::string::npos &&
        json.find("\"type\":\"bplus_tree_index\"") == std::string::npos) {
        return false;
    }
    
    size_t rootPos = json.find("\"rootPageId\":");
    if (rootPos == std::string::npos) {
        return false;
    }
    
    int rootPageId = -1;
    sscanf(json.c_str() + rootPos + 13, "%d", &rootPageId);
    
    if (rootPageId == -1) {
        return false;
    }
    
    size_t pagesStart = json.find("\"pages\":");
    if (pagesStart == std::string::npos) {
        return false;
    }
    
    std::map<int, BPlusNode*> pageMap;
    std::map<int, std::vector<int>> childrenMap;
    std::map<int, int> nextLeafMap;
    
    size_t pos = pagesStart;
    while ((pos = json.find("{\"pageId\":", pos)) != std::string::npos) {
        // Extract pageId
        size_t pageIdStart = pos + 10;
        size_t pageIdEnd = json.find(",", pageIdStart);
        int pageId = std::stoi(json.substr(pageIdStart, pageIdEnd - pageIdStart));
        
        size_t isLeafStart = json.find("\"isLeaf\":", pos) + 9;
        bool isLeaf = json[isLeafStart] == 't';
        
        BPlusNode* node = new BPlusNode(isLeaf);
        node->pageId = pageId;
        pageMap[pageId] = node;
        
        size_t keysStart = json.find("\"keys\":[", pos) + 8;
        size_t keysEnd = json.find("]", keysStart);
        std::string keysStr = json.substr(keysStart, keysEnd - keysStart);
        
        size_t keyPos = 0;
        while ((keyPos = keysStr.find("\"", keyPos)) != std::string::npos) {
            keyPos++;
            size_t keyEnd = keysStr.find("\"", keyPos);
            if (keyEnd == std::string::npos) break;
            std::string key = keysStr.substr(keyPos, keyEnd - keyPos);
            node->keys.push_back(key);
            keyPos = keyEnd + 1;
        }
        
        if (isLeaf) {
            size_t rowIdsStart = json.find("\"rowIds\":[", pos) + 10;
            size_t rowIdsEnd = json.find("]", rowIdsStart);
            std::string rowIdsStr = json.substr(rowIdsStart, rowIdsEnd - rowIdsStart);
            
            if (!rowIdsStr.empty()) {
                size_t rowPos = 0;
                while (rowPos < rowIdsStr.size()) {
                    size_t numEnd = rowIdsStr.find(",", rowPos);
                    if (numEnd == std::string::npos) {
                        numEnd = rowIdsStr.size();
                    }
                    std::string numStr = rowIdsStr.substr(rowPos, numEnd - rowPos);
                    numStr.erase(0, numStr.find_first_not_of(" \t\n\r"));
                    numStr.erase(numStr.find_last_not_of(" \t\n\r") + 1);
                    if (!numStr.empty()) {
                        node->values.push_back(std::stoull(numStr));
                    }
                    rowPos = numEnd + 1;
                }
            }
            
            size_t nextLeafStart = json.find("\"nextLeafPageId\":", pos) + 17;
            size_t nextLeafEnd = json.find("}", nextLeafStart);
            std::string nextLeafStr = json.substr(nextLeafStart, nextLeafEnd - nextLeafStart);
            nextLeafStr.erase(0, nextLeafStr.find_first_not_of(" \t\n\r"));
            nextLeafStr.erase(nextLeafStr.find_last_not_of(" \t\n\r") + 1);
            int nextLeafId = std::stoi(nextLeafStr);
            
            if (nextLeafId != -1) {
                nextLeafMap[pageId] = nextLeafId;
            }
        } else {
            size_t childrenStart = json.find("\"children\":[", pos) + 12;
            size_t childrenEnd = json.find("]", childrenStart);
            std::string childrenStr = json.substr(childrenStart, childrenEnd - childrenStart);
            
            if (!childrenStr.empty()) {
                size_t childPos = 0;
                while (childPos < childrenStr.size()) {
                    size_t numEnd = childrenStr.find(",", childPos);
                    if (numEnd == std::string::npos) {
                        numEnd = childrenStr.size();
                    }
                    std::string numStr = childrenStr.substr(childPos, numEnd - childPos);
                    numStr.erase(0, numStr.find_first_not_of(" \t\n\r"));
                    numStr.erase(numStr.find_last_not_of(" \t\n\r") + 1);
                    if (!numStr.empty()) {
                        childrenMap[pageId].push_back(std::stoi(numStr));
                    }
                    childPos = numEnd + 1;
                }
            }
        }
        
        pos = keysEnd + 1;
    }
    
    if (pageMap.find(rootPageId) == pageMap.end()) {
        // Clean up
        for (auto& p : pageMap) {
            delete p.second;
        }
        return false;
    }
    
    root = pageMap[rootPageId];
    
    for (auto& p : childrenMap) {
        BPlusNode* parent = pageMap[p.first];
        for (int childId : p.second) {
            if (pageMap.find(childId) != pageMap.end()) {
                parent->children.push_back(pageMap[childId]);
                pageMap[childId]->parent = parent;
            }
        }
    }
    
    for (auto& p : nextLeafMap) {
        BPlusNode* leaf = pageMap[p.first];
        if (pageMap.find(p.second) != pageMap.end()) {
            leaf->next = pageMap[p.second];
        }
    }
    
    return true;
}


void BPlusTree::shiftRowIdsAfterDeleted(const std::vector<size_t>& deletedRowIds) {
    if (!root || deletedRowIds.empty()) {
        return;
    }

    std::vector<size_t> sortedDeleted = deletedRowIds;
    std::sort(sortedDeleted.begin(), sortedDeleted.end());
    sortedDeleted.erase(std::unique(sortedDeleted.begin(), sortedDeleted.end()), sortedDeleted.end());

    BPlusNode* current = root;
    while (current && !current->isLeaf) {
        current = current->children[0];
    }

    while (current) {
        for (size_t i = 0; i < current->values.size(); i++) {
            size_t oldRowId = current->values[i];

            size_t shift = 0;
            while (shift < sortedDeleted.size() && sortedDeleted[shift] < oldRowId) {
                shift++;
            }

            if (shift > 0) {
                current->values[i] = oldRowId - shift;
            }
        }

        current = current->next;
    }
}

int BPlusTree::getMaxPageId() const {
    int maxId = 0;
    
    if (!root) return maxId;
    
    std::vector<BPlusNode*> queue;
    queue.push_back(root);
    
    while (!queue.empty()) {
        BPlusNode* node = queue.front();
        queue.erase(queue.begin());
        
        maxId = std::max(maxId, node->pageId);
        
        if (!node->isLeaf) {
            for (auto child : node->children) {
                queue.push_back(child);
            }
        }
    }
    
    return maxId;
}