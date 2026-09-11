#include "bplustree.h"

#include <algorithm>
#include <cstring>
#include <fstream>
#include <limits>
#include <sstream>
#include <stdexcept>

namespace {

const std::uint32_t PAGE_MAGIC = 0x42505431u; // BPT1
const std::uint16_t PAGE_VERSION = 1;
const std::size_t PAGE_HEADER_SIZE = sizeof(std::uint32_t) + sizeof(std::uint16_t)
    + sizeof(std::uint8_t) + sizeof(std::uint32_t) + sizeof(std::uint32_t);
const std::uint8_t LEAF_PAGE = 1;
const std::uint8_t INTERNAL_PAGE = 2;

void appendU16(std::vector<char>& data, std::uint16_t value) {
    data.push_back(static_cast<char>(value & 0xffu));
    data.push_back(static_cast<char>((value >> 8) & 0xffu));
}

void appendU32(std::vector<char>& data, std::uint32_t value) {
    for (int i = 0; i < 4; ++i) {
        data.push_back(static_cast<char>((value >> (8 * i)) & 0xffu));
    }
}

void appendU64(std::vector<char>& data, std::uint64_t value) {
    for (int i = 0; i < 8; ++i) {
        data.push_back(static_cast<char>((value >> (8 * i)) & 0xffu));
    }
}

bool readU16(const char* data, std::size_t size, std::size_t& pos, std::uint16_t& value) {
    if (pos + 2 > size) return false;
    value = static_cast<std::uint16_t>(static_cast<unsigned char>(data[pos])) |
            static_cast<std::uint16_t>(static_cast<unsigned char>(data[pos + 1]) << 8);
    pos += 2;
    return true;
}

bool readU32(const char* data, std::size_t size, std::size_t& pos, std::uint32_t& value) {
    if (pos + 4 > size) return false;
    value = static_cast<std::uint32_t>(static_cast<unsigned char>(data[pos])) |
            (static_cast<std::uint32_t>(static_cast<unsigned char>(data[pos + 1])) << 8) |
            (static_cast<std::uint32_t>(static_cast<unsigned char>(data[pos + 2])) << 16) |
            (static_cast<std::uint32_t>(static_cast<unsigned char>(data[pos + 3])) << 24);
    pos += 4;
    return true;
}

bool readU64(const char* data, std::size_t size, std::size_t& pos, std::uint64_t& value) {
    if (pos + 8 > size) return false;
    value = 0;
    for (int i = 0; i < 8; ++i) {
        value |= static_cast<std::uint64_t>(static_cast<unsigned char>(data[pos + i])) << (8 * i);
    }
    pos += 8;
    return true;
}

bool canFitPage(const std::vector<char>& data) {
    return data.size() <= BPLUS_PAGE_SIZE;
}

std::string trimCopy(std::string value) {
    const std::string whitespace = " \t\r\n";
    const std::size_t first = value.find_first_not_of(whitespace);
    if (first == std::string::npos) return std::string();
    const std::size_t last = value.find_last_not_of(whitespace);
    return value.substr(first, last - first + 1);
}

} // namespace

BPlusNode::BPlusNode(bool leaf)
    : isLeaf(leaf), nextPageId(0), pageId(0) {}

BPlusTree::BPlusTree(int orderValue, const std::string& type)
    : rootPageId(0),
      nextPageId(1),
      order(std::max(3, orderValue)),
      keyType(type) {}

BPlusTree::~BPlusTree() {}

void BPlusTree::setStoragePath(const std::string& path, const std::string& column) {
    metadataPath = path;
    pagesPath = path + ".pages";
    metadataColumn = column;
    ensurePageFile();
}

bool BPlusTree::ensurePageFile() const {
    if (pagesPath.empty()) return false;

    std::ifstream test(pagesPath.c_str(), std::ios::binary);
    if (test.good()) return true;

    std::ofstream out(pagesPath.c_str(), std::ios::binary);
    if (!out.is_open()) return false;
    const char zero = 0;
    out.write(&zero, 1);
    return out.good();
}

std::uint32_t BPlusTree::allocatePage() {
    if (!ensurePageFile()) return 0;

    const std::uint32_t pageId = nextPageId++;
    std::fstream file(pagesPath.c_str(), std::ios::in | std::ios::out | std::ios::binary);
    if (!file.is_open()) return 0;

    const std::uint64_t offset = static_cast<std::uint64_t>(pageId) * BPLUS_PAGE_SIZE;
    file.seekp(static_cast<std::streamoff>(offset), std::ios::beg);
    std::vector<char> zeros(BPLUS_PAGE_SIZE, 0);
    file.write(zeros.data(), static_cast<std::streamsize>(zeros.size()));
    if (!file.good()) return 0;

    persistMetadata();
    return pageId;
}

int BPlusTree::compareKeys(const std::string& a, const std::string& b) const {
    if (keyType == "int") {
        const int x = std::stoi(a);
        const int y = std::stoi(b);
        if (x < y) return -1;
        if (x > y) return 1;
        return 0;
    }
    if (a < b) return -1;
    if (a > b) return 1;
    return 0;
}

bool BPlusTree::writePage(std::uint32_t pageId, const BPlusNode& node) const {
    if (pageId == 0 || !ensurePageFile()) return false;

    std::vector<char> data;
    data.reserve(BPLUS_PAGE_SIZE);
    appendU32(data, PAGE_MAGIC);
    appendU16(data, PAGE_VERSION);
    data.push_back(static_cast<char>(node.isLeaf ? LEAF_PAGE : INTERNAL_PAGE));
    appendU32(data, static_cast<std::uint32_t>(node.keys.size()));
    appendU32(data, node.isLeaf ? node.nextPageId : 0);

    if (node.isLeaf) {
        if (node.keys.size() != node.values.size()) return false;
        for (std::size_t i = 0; i < node.keys.size(); ++i) {
            if (node.keys[i].size() > std::numeric_limits<std::uint32_t>::max()) return false;
            appendU32(data, static_cast<std::uint32_t>(node.keys[i].size()));
            data.insert(data.end(), node.keys[i].begin(), node.keys[i].end());
            appendU64(data, static_cast<std::uint64_t>(node.values[i]));
        }
    } else {
        if (node.children.size() != node.keys.size() + 1) return false;
        for (std::size_t i = 0; i < node.keys.size(); ++i) {
            if (node.keys[i].size() > std::numeric_limits<std::uint32_t>::max()) return false;
            appendU32(data, static_cast<std::uint32_t>(node.keys[i].size()));
            data.insert(data.end(), node.keys[i].begin(), node.keys[i].end());
        }
        for (std::size_t i = 0; i < node.children.size(); ++i) {
            appendU32(data, node.children[i]);
        }
    }

    if (!canFitPage(data)) return false;
    data.resize(BPLUS_PAGE_SIZE, 0);

    std::fstream file(pagesPath.c_str(), std::ios::in | std::ios::out | std::ios::binary);
    if (!file.is_open()) return false;
    const std::uint64_t offset = static_cast<std::uint64_t>(pageId) * BPLUS_PAGE_SIZE;
    file.seekp(static_cast<std::streamoff>(offset), std::ios::beg);
    file.write(data.data(), static_cast<std::streamsize>(data.size()));
    file.flush();
    return file.good();
}

bool BPlusTree::readPage(std::uint32_t pageId, BPlusNode& node) const {
    if (pageId == 0 || !ensurePageFile()) return false;

    std::ifstream file(pagesPath.c_str(), std::ios::binary);
    if (!file.is_open()) return false;

    const std::uint64_t offset = static_cast<std::uint64_t>(pageId) * BPLUS_PAGE_SIZE;
    file.seekg(static_cast<std::streamoff>(offset), std::ios::beg);
    if (!file.good()) return false;

    std::vector<char> buffer(BPLUS_PAGE_SIZE, 0);
    file.read(buffer.data(), static_cast<std::streamsize>(buffer.size()));
    if (file.gcount() != static_cast<std::streamsize>(BPLUS_PAGE_SIZE)) return false;

    std::size_t pos = 0;
    std::uint32_t magic = 0;
    std::uint16_t version = 0;
    std::uint32_t count = 0;
    std::uint32_t nextId = 0;
    if (!readU32(buffer.data(), buffer.size(), pos, magic) || magic != PAGE_MAGIC) return false;
    if (!readU16(buffer.data(), buffer.size(), pos, version) || version != PAGE_VERSION) return false;

    if (pos >= buffer.size()) return false;
    const std::uint8_t type = static_cast<std::uint8_t>(buffer[pos++]);
    if (!readU32(buffer.data(), buffer.size(), pos, count)) return false;
    if (!readU32(buffer.data(), buffer.size(), pos, nextId)) return false;

    node.keys.clear();
    node.values.clear();
    node.children.clear();
    node.pageId = pageId;
    node.isLeaf = type == LEAF_PAGE;
    node.nextPageId = node.isLeaf ? nextId : 0;

    if (!node.isLeaf && type != INTERNAL_PAGE) return false;

    node.keys.reserve(count);
    if (node.isLeaf) node.values.reserve(count);

    for (std::uint32_t i = 0; i < count; ++i) {
        std::uint32_t keyLength = 0;
        if (!readU32(buffer.data(), buffer.size(), pos, keyLength)) return false;
        if (pos + keyLength > buffer.size()) return false;
        node.keys.push_back(std::string(buffer.data() + pos, keyLength));
        pos += keyLength;

        if (node.isLeaf) {
            std::uint64_t value = 0;
            if (!readU64(buffer.data(), buffer.size(), pos, value)) return false;
            node.values.push_back(static_cast<size_t>(value));
        }
    }

    if (!node.isLeaf) {
        node.children.reserve(static_cast<std::size_t>(count) + 1);
        for (std::uint32_t i = 0; i < count + 1; ++i) {
            std::uint32_t child = 0;
            if (!readU32(buffer.data(), buffer.size(), pos, child)) return false;
            node.children.push_back(child);
        }
    }

    return true;
}

size_t BPlusTree::lowerBoundInLeaf(const BPlusNode& leaf,
                                   const std::string& key) const {
    std::size_t lo = 0;
    std::size_t hi = leaf.keys.size();
    while (lo < hi) {
        const std::size_t mid = lo + (hi - lo) / 2;
        if (compareKeys(leaf.keys[mid], key) < 0) lo = mid + 1;
        else hi = mid;
    }
    return lo;
}

size_t BPlusTree::childIndexInInternal(const BPlusNode& node,
                                       const std::string& key) const {
    std::size_t i = 0;
    while (i < node.keys.size() && compareKeys(key, node.keys[i]) >= 0) ++i;
    return i;
}

bool BPlusTree::findLeafId(const std::string& key,
                           std::vector<std::uint32_t>* path,
                           std::uint32_t& leafId) const {
    if (rootPageId == 0) return false;

    BPlusNode page(true);
    std::uint32_t currentId = rootPageId;
    while (true) {
        if (!readPage(currentId, page)) return false;
        if (page.isLeaf) {
            leafId = currentId;
            return true;
        }
        if (path != nullptr) path->push_back(currentId);
        const std::size_t index = childIndexInInternal(page, key);
        if (index >= page.children.size() || page.children[index] == 0) return false;
        currentId = page.children[index];
    }
}

bool BPlusTree::search(const std::string& key, size_t& value) const {
    invalidateRootView();
    std::uint32_t leafId = 0;
    if (!findLeafId(key, nullptr, leafId)) return false;

    BPlusNode leaf(true);
    if (!readPage(leafId, leaf)) return false;
    const std::size_t pos = lowerBoundInLeaf(leaf, key);
    if (pos < leaf.keys.size() && compareKeys(leaf.keys[pos], key) == 0) {
        value = leaf.values[pos];
        return true;
    }
    return false;
}

std::vector<size_t> BPlusTree::searchAll(const std::string& key) const {
    invalidateRootView();
    std::vector<size_t> result;
    std::uint32_t leafId = 0;
    if (!findLeafId(key, nullptr, leafId)) return result;

    BPlusNode leaf(true);
    while (leafId != 0) {
        if (!readPage(leafId, leaf)) break;
        const std::size_t pos = lowerBoundInLeaf(leaf, key);
        for (std::size_t i = pos; i < leaf.keys.size(); ++i) {
            const int cmp = compareKeys(leaf.keys[i], key);
            if (cmp != 0) return result;
            result.push_back(leaf.values[i]);
        }
        leafId = leaf.nextPageId;
        if (leafId != 0 && !leaf.keys.empty() && compareKeys(leaf.keys.back(), key) > 0) break;
    }
    return result;
}

std::vector<size_t> BPlusTree::rangeSearch(const std::string& left,
                                           const std::string& right) const {
    invalidateRootView();
    std::vector<size_t> result;
    std::uint32_t leafId = 0;
    if (!findLeafId(left, nullptr, leafId)) return result;

    BPlusNode leaf(true);
    while (leafId != 0) {
        if (!readPage(leafId, leaf)) break;
        const std::size_t start = lowerBoundInLeaf(leaf, left);
        for (std::size_t i = start; i < leaf.keys.size(); ++i) {
            if (compareKeys(leaf.keys[i], right) >= 0) return result;
            result.push_back(leaf.values[i]);
        }
        leafId = leaf.nextPageId;
    }
    return result;
}

bool BPlusTree::insert(const std::string& key, size_t value) {
    invalidateRootView();
    if (metadataPath.empty()) return false;
    if (!ensurePageFile()) return false;
    if (search(key, value)) return false; // INDEXED => unique

    if (rootPageId == 0) {
        const std::uint32_t rootId = allocatePage();
        if (rootId == 0) return false;
        BPlusNode root(true);
        root.pageId = rootId;
        root.keys.push_back(key);
        root.values.push_back(value);
        if (!writePage(rootId, root)) return false;
        rootPageId = rootId;
        persistMetadata();
        return true;
    }

    std::vector<std::uint32_t> path;
    std::uint32_t leafId = 0;
    if (!findLeafId(key, &path, leafId)) return false;

    BPlusNode page(true);
    if (!readPage(leafId, page)) return false;
    const std::size_t pos = lowerBoundInLeaf(page, key);
    page.keys.insert(page.keys.begin() + static_cast<std::ptrdiff_t>(pos), key);
    page.values.insert(page.values.begin() + static_cast<std::ptrdiff_t>(pos), value);

    if (!writePage(leafId, page)) return false;
    if (page.keys.size() < static_cast<std::size_t>(order)) {
        persistMetadata();
        return true;
    }

    return splitLeafAndInsertIntoParent(leafId, path, page);
}

bool BPlusTree::insertSeparatorIntoParent(std::uint32_t leftId,
                                          const std::string& separator,
                                          std::uint32_t rightId,
                                          std::vector<std::uint32_t>& path,
                                          BPlusNode& page) {
    if (path.empty()) {
        growRoot(leftId, separator, rightId, page);
        return true;
    }

    const std::uint32_t parentId = path.back();
    path.pop_back();
    if (!readPage(parentId, page)) return false;

    std::size_t childPos = 0;
    while (childPos < page.children.size() && page.children[childPos] != leftId) ++childPos;
    if (childPos >= page.children.size()) return false;

    page.keys.insert(page.keys.begin() + static_cast<std::ptrdiff_t>(childPos), separator);
    page.children.insert(page.children.begin() + static_cast<std::ptrdiff_t>(childPos + 1), rightId);

    if (!writePage(parentId, page)) return false;

    if (page.keys.size() < static_cast<std::size_t>(order)) {
        persistMetadata();
        return true;
    }

    return splitInternalAndInsertIntoParent(parentId, path, page);
}

void BPlusTree::growRoot(std::uint32_t leftId,
                         const std::string& separator,
                         std::uint32_t rightId,
                         BPlusNode& page) {
    const std::uint32_t newRootId = allocatePage();
    if (newRootId == 0) throw std::runtime_error("failed to allocate B+ tree root page");

    page = BPlusNode(false);
    page.pageId = newRootId;
    page.keys.push_back(separator);
    page.children.push_back(leftId);
    page.children.push_back(rightId);
    if (!writePage(newRootId, page)) throw std::runtime_error("failed to write B+ tree root page");
    rootPageId = newRootId;
    persistMetadata();
}

bool BPlusTree::splitLeafAndInsertIntoParent(std::uint32_t leafId,
                                             std::vector<std::uint32_t>& path,
                                             BPlusNode& page) {
    const std::size_t mid = page.keys.size() / 2;
    std::vector<std::string> rightKeys(page.keys.begin() + static_cast<std::ptrdiff_t>(mid), page.keys.end());
    std::vector<size_t> rightValues(page.values.begin() + static_cast<std::ptrdiff_t>(mid), page.values.end());
    const std::uint32_t oldNext = page.nextPageId;
    const std::string separator = rightKeys.front(); // первый ключ правого листа
    const std::uint32_t rightId = allocatePage();
    if (rightId == 0) return false;

    page.keys.resize(mid);
    page.values.resize(mid);
    page.nextPageId = rightId;
    if (!writePage(leafId, page)) return false;

    page.pageId = rightId;
    page.keys.swap(rightKeys);
    page.values.swap(rightValues);
    page.nextPageId = oldNext;
    if (!writePage(rightId, page)) return false;

    return insertSeparatorIntoParent(leafId, separator, rightId, path, page);
}

bool BPlusTree::splitInternalAndInsertIntoParent(std::uint32_t nodeId,
                                                std::vector<std::uint32_t>& path,
                                                BPlusNode& page) {
    const std::size_t mid = page.keys.size() / 2;
    const std::string separator = page.keys[mid];
    const std::uint32_t rightId = allocatePage();
    if (rightId == 0) return false;

    std::vector<std::string> leftKeys(page.keys.begin(), page.keys.begin() + static_cast<std::ptrdiff_t>(mid));
    std::vector<std::uint32_t> leftChildren(page.children.begin(), page.children.begin() + static_cast<std::ptrdiff_t>(mid + 1));
    std::vector<std::string> rightKeys(page.keys.begin() + static_cast<std::ptrdiff_t>(mid + 1), page.keys.end());
    std::vector<std::uint32_t> rightChildren(page.children.begin() + static_cast<std::ptrdiff_t>(mid + 1), page.children.end());

    page.keys.swap(leftKeys);
    page.children.swap(leftChildren);
    if (!writePage(nodeId, page)) return false;

    page.pageId = rightId;
    page.keys.swap(rightKeys);
    page.children.swap(rightChildren);
    if (!writePage(rightId, page)) return false;

    return insertSeparatorIntoParent(nodeId, separator, rightId, path, page);
}

bool BPlusTree::removeFromCurrentLeaf(std::uint32_t leafId,
                                      const std::string& key,
                                      BPlusNode& page) {
    if (!readPage(leafId, page) || !page.isLeaf) return false;
    const std::size_t pos = lowerBoundInLeaf(page, key);
    if (pos >= page.keys.size() || compareKeys(page.keys[pos], key) != 0) return false;

    page.keys.erase(page.keys.begin() + static_cast<std::ptrdiff_t>(pos));
    page.values.erase(page.values.begin() + static_cast<std::ptrdiff_t>(pos));
    return writePage(leafId, page);
}

void BPlusTree::refreshAncestorsAfterKeyChange(
    const std::vector<std::uint32_t>& path,
    std::uint32_t childId,
    const std::string& newMinimum,
    BPlusNode& page) {
    std::string minimum = newMinimum;
    std::uint32_t currentChildId = childId;

    for (std::size_t pos = path.size(); pos > 0; --pos) {
        const std::uint32_t parentId = path[pos - 1];
        if (!readPage(parentId, page) || page.isLeaf) return;

        std::size_t childIndex = 0;
        while (childIndex < page.children.size() &&
               page.children[childIndex] != currentChildId) {
            ++childIndex;
        }
        if (childIndex >= page.children.size()) return;

        if (childIndex > 0) {
            page.keys[childIndex - 1] = minimum;
            writePage(parentId, page);
            return;
        }

        // The changed child is the first child, so this internal node's
        // minimum changed too. Propagate the new minimum to its parent.
        currentChildId = parentId;
    }
}

bool BPlusTree::borrowFromRight(std::uint32_t nodeId,
                                std::uint32_t parentId,
                                size_t index,
                                BPlusNode& page) {
    if (!readPage(parentId, page) || page.isLeaf || index >= page.keys.size()) return false;
    if (index + 1 >= page.children.size()) return false;

    const std::uint32_t rightId = page.children[index + 1];
    const std::uint32_t leftId = nodeId;

    // Reuse the same BPlusNode object for parent, sibling and current pages.
    const std::string parentSeparator = page.keys[index];
    (void)parentSeparator;

    if (!readPage(rightId, page)) return false;
    if (page.keys.size() <= static_cast<std::size_t>((order - 1) / 2)) return false;

    const std::string borrowedKey = page.keys.front();
    const size_t borrowedValue = page.values.empty() ? 0 : page.values.front();
    const bool leaf = page.isLeaf;
    const std::uint32_t borrowedChild = (!leaf && !page.children.empty()) ? page.children.front() : 0;

    page.keys.erase(page.keys.begin());
    if (leaf) page.values.erase(page.values.begin());
    else page.children.erase(page.children.begin());
    // For an internal page, keys[i] describes the minimum of child i+1.
    // After moving the first child to the left sibling, the new minimum of
    // this right page is the key that used to separate its first two children.
    const std::string newParentSeparator = leaf
        ? (page.keys.empty() ? parentSeparator : page.keys.front())
        : borrowedKey;
    if (!writePage(rightId, page)) return false;

    if (!readPage(leftId, page)) return false;
    if (leaf) {
        page.keys.push_back(borrowedKey);
        page.values.push_back(borrowedValue);
    } else {
        page.keys.push_back(parentSeparator);
        page.children.push_back(borrowedChild);
    }
    if (!writePage(leftId, page)) return false;

    if (!readPage(parentId, page)) return false;
    page.keys[index] = newParentSeparator;
    return writePage(parentId, page);
}

bool BPlusTree::borrowFromLeft(std::uint32_t nodeId,
                               std::uint32_t parentId,
                               size_t index,
                               BPlusNode& page) {
    if (index == 0) return false;
    if (!readPage(parentId, page) || page.isLeaf) return false;
    const std::uint32_t leftId = page.children[index - 1];
    const std::string parentSeparator = page.keys[index - 1];

    if (!readPage(leftId, page)) return false;
    if (page.keys.size() <= static_cast<std::size_t>((order - 1) / 2)) return false;

    const bool leaf = page.isLeaf;
    std::string borrowedKey;
    size_t borrowedValue = 0;
    std::uint32_t borrowedChild = 0;
    if (leaf) {
        borrowedKey = page.keys.back();
        borrowedValue = page.values.back();
        page.keys.pop_back();
        page.values.pop_back();
    } else {
        borrowedKey = page.keys.back();
        borrowedChild = page.children.back();
        page.keys.pop_back();
        page.children.pop_back();
    }
    if (!writePage(leftId, page)) return false;

    if (!readPage(nodeId, page)) return false;
    if (leaf) {
        page.keys.insert(page.keys.begin(), borrowedKey);
        page.values.insert(page.values.begin(), borrowedValue);
    } else {
        page.keys.insert(page.keys.begin(), parentSeparator);
        page.children.insert(page.children.begin(), borrowedChild);
    }
    if (!writePage(nodeId, page)) return false;

    if (!readPage(parentId, page)) return false;
    if (leaf) page.keys[index - 1] = borrowedKey;
    else page.keys[index - 1] = borrowedKey;
    return writePage(parentId, page);
}

bool BPlusTree::mergeWithSibling(std::uint32_t nodeId,
                                 std::uint32_t parentId,
                                 size_t index,
                                 BPlusNode& page) {
    if (!readPage(parentId, page) || page.isLeaf || page.children.size() < 2) return false;
    if (index >= page.children.size()) return false;

    // Merge the underfull node with one adjacent sibling. When a left sibling
    // exists, merge into it; otherwise merge the current node with its right
    // sibling. The parent separator is moved into an internal-node merge.
    std::uint32_t leftId = 0;
    std::uint32_t rightId = 0;
    std::size_t separatorIndex = 0;

    if (index > 0) {
        leftId = page.children[index - 1];
        rightId = nodeId;
        separatorIndex = index - 1;
    } else if (index + 1 < page.children.size()) {
        leftId = nodeId;
        rightId = page.children[index + 1];
        separatorIndex = index;
    } else {
        return false;
    }

    const std::string separator = page.keys[separatorIndex];

    if (!readPage(rightId, page)) return false;
    const bool leaf = page.isLeaf;
    const std::uint32_t rightNext = page.nextPageId;
    const std::vector<std::string> rightKeys = page.keys;
    const std::vector<size_t> rightValues = page.values;
    const std::vector<std::uint32_t> rightChildren = page.children;

    if (!readPage(leftId, page)) return false;
    if (page.isLeaf != leaf) return false;

    if (leaf) {
        page.keys.insert(page.keys.end(), rightKeys.begin(), rightKeys.end());
        page.values.insert(page.values.end(), rightValues.begin(), rightValues.end());
        page.nextPageId = rightNext;
    } else {
        page.keys.push_back(separator);
        page.keys.insert(page.keys.end(), rightKeys.begin(), rightKeys.end());
        page.children.insert(page.children.end(), rightChildren.begin(), rightChildren.end());
    }
    if (!writePage(leftId, page)) return false;

    if (!readPage(parentId, page)) return false;
    page.keys.erase(page.keys.begin() + static_cast<std::ptrdiff_t>(separatorIndex));
    page.children.erase(page.children.begin() + static_cast<std::ptrdiff_t>(separatorIndex + 1));
    if (!writePage(parentId, page)) return false;

    return true;
}

void BPlusTree::fixUnderflow(std::uint32_t nodeId,
                             std::vector<std::uint32_t>& path,
                             BPlusNode& page) {
    const std::size_t minKeys = static_cast<std::size_t>((order - 1) / 2);

    while (nodeId != rootPageId) {
        if (!readPage(nodeId, page)) return;
        if (page.keys.size() >= minKeys) return;
        if (path.empty()) return;

        const std::uint32_t parentId = path.back();
        path.pop_back();
        if (!readPage(parentId, page) || page.isLeaf) return;

        std::size_t index = 0;
        while (index < page.children.size() && page.children[index] != nodeId) ++index;
        if (index >= page.children.size()) return;

        const bool hasRight = index + 1 < page.children.size();
        const bool hasLeft = index > 0;

        // Prefer borrowing before merge, following the reference design.
        if (hasRight) {
            if (borrowFromRight(nodeId, parentId, index, page)) return;
        }
        if (hasLeft) {
            if (borrowFromLeft(nodeId, parentId, index, page)) return;
        }

        if (!mergeWithSibling(nodeId, parentId, index, page)) return;
        nodeId = parentId;
    }
}

void BPlusTree::shrinkRoot(BPlusNode& page) {
    while (rootPageId != 0) {
        if (!readPage(rootPageId, page)) return;
        if (page.isLeaf) {
            if (page.keys.empty()) rootPageId = 0;
            break;
        }
        if (!page.keys.empty()) break;
        if (page.children.empty()) {
            rootPageId = 0;
            break;
        }
        rootPageId = page.children.front();
    }
    persistMetadata();
}

bool BPlusTree::remove(const std::string& key) {
    invalidateRootView();
    if (rootPageId == 0) return false;

    std::vector<std::uint32_t> path;
    std::uint32_t leafId = 0;
    if (!findLeafId(key, &path, leafId)) return false;

    BPlusNode page(true);
    if (!removeFromCurrentLeaf(leafId, key, page)) return false;

    if (leafId == rootPageId) {
        shrinkRoot(page);
        return true;
    }

    const std::size_t minKeys = static_cast<std::size_t>((order - 1) / 2);
    if (!page.keys.empty()) {
        refreshAncestorsAfterKeyChange(path, leafId, page.keys.front(), page);
    }
    if (page.keys.size() < minKeys) {
        fixUnderflow(leafId, path, page);
    }
    shrinkRoot(page);
    return true;
}

BPlusNode* BPlusTree::getRoot() const {
    rootView.reset(new BPlusNode(true));
    if (rootPageId == 0 || !readPage(rootPageId, *rootView)) {
        rootView.reset();
        return nullptr;
    }
    return rootView.get();
}

std::vector<std::pair<std::string, size_t>> BPlusTree::getAllKeyValues() const {
    invalidateRootView();
    std::vector<std::pair<std::string, size_t>> result;
    if (rootPageId == 0) return result;

    BPlusNode page(true);
    std::uint32_t currentId = rootPageId;
    while (true) {
        if (!readPage(currentId, page)) return result;
        if (page.isLeaf) break;
        if (page.children.empty()) return result;
        currentId = page.children.front();
    }

    while (currentId != 0) {
        if (!readPage(currentId, page)) break;
        for (std::size_t i = 0; i < page.keys.size(); ++i) {
            result.push_back(std::make_pair(page.keys[i], page.values[i]));
        }
        currentId = page.nextPageId;
    }
    return result;
}

void BPlusTree::shiftRowIdsAfterDeleted(const std::vector<size_t>& deletedRowIds) {
    invalidateRootView();
    if (deletedRowIds.empty() || rootPageId == 0) return;

    std::vector<size_t> sorted = deletedRowIds;
    std::sort(sorted.begin(), sorted.end());
    sorted.erase(std::unique(sorted.begin(), sorted.end()), sorted.end());

    BPlusNode page(true);
    std::uint32_t currentId = rootPageId;
    while (true) {
        if (!readPage(currentId, page)) return;
        if (page.isLeaf) break;
        if (page.children.empty()) return;
        currentId = page.children.front();
    }

    while (currentId != 0) {
        if (!readPage(currentId, page)) return;
        for (std::size_t i = 0; i < page.values.size(); ++i) {
            const size_t oldId = page.values[i];
            const std::size_t shift = static_cast<std::size_t>(
                std::lower_bound(sorted.begin(), sorted.end(), oldId) - sorted.begin());
            if (shift > 0) page.values[i] = oldId - shift;
        }
        if (!writePage(currentId, page)) return;
        currentId = page.nextPageId;
    }
    persistMetadata();
}

std::string BPlusTree::jsonEscape(const std::string& value) {
    std::string result;
    result.reserve(value.size() + 8);
    for (char ch : value) {
        switch (ch) {
            case '\\': result += "\\\\"; break;
            case '"': result += "\\\""; break;
            case '\n': result += "\\n"; break;
            case '\r': result += "\\r"; break;
            case '\t': result += "\\t"; break;
            default: result += ch; break;
        }
    }
    return result;
}

void BPlusTree::persistMetadata() const {
    if (metadataPath.empty()) return;
    std::ofstream out(metadataPath.c_str(), std::ios::trunc);
    if (!out.is_open()) return;

    out << "{\n";
    out << "  \"type\": \"bplus_tree_index\",\n";
    out << "  \"version\": 2,\n";
    out << "  \"column\": \"" << jsonEscape(metadataColumn) << "\",\n";
    out << "  \"keyType\": \"" << jsonEscape(keyType) << "\",\n";
    out << "  \"order\": " << order << ",\n";
    out << "  \"rootPageId\": " << rootPageId << ",\n";
    out << "  \"nextPageId\": " << nextPageId << ",\n";
    out << "  \"pageSize\": " << BPLUS_PAGE_SIZE << ",\n";
    out << "  \"storageFile\": \"" << jsonEscape(pagesPath) << "\"\n";
    out << "}\n";
}

bool BPlusTree::parseUnsignedField(const std::string& text,
                                   const std::string& field,
                                   std::uint32_t& out) {
    const std::string marker = "\"" + field + "\"";
    const std::size_t pos = text.find(marker);
    if (pos == std::string::npos) return false;
    const std::size_t colon = text.find(':', pos + marker.size());
    if (colon == std::string::npos) return false;
    std::size_t end = text.find_first_of(",}\n\r", colon + 1);
    if (end == std::string::npos) end = text.size();
    try {
        const unsigned long long value = std::stoull(trimCopy(text.substr(colon + 1, end - colon - 1)));
        if (value > std::numeric_limits<std::uint32_t>::max()) return false;
        out = static_cast<std::uint32_t>(value);
        return true;
    } catch (...) {
        return false;
    }
}

bool BPlusTree::parseIntField(const std::string& text,
                              const std::string& field,
                              int& out) {
    const std::string marker = "\"" + field + "\"";
    const std::size_t pos = text.find(marker);
    if (pos == std::string::npos) return false;
    const std::size_t colon = text.find(':', pos + marker.size());
    if (colon == std::string::npos) return false;
    std::size_t end = text.find_first_of(",}\n\r", colon + 1);
    if (end == std::string::npos) end = text.size();
    try {
        out = std::stoi(trimCopy(text.substr(colon + 1, end - colon - 1)));
        return true;
    } catch (...) {
        return false;
    }
}

bool BPlusTree::loadMetadata(const std::string& path, int& version) {
    std::ifstream in(path.c_str(), std::ios::binary);
    if (!in.is_open()) return false;
    std::stringstream ss;
    ss << in.rdbuf();
    const std::string text = ss.str();

    int v = 0;
    if (!parseIntField(text, "version", v)) return false;
    version = v;

    std::uint32_t loadedRoot = 0;
    std::uint32_t loadedNext = 1;
    if (!parseUnsignedField(text, "rootPageId", loadedRoot)) return false;
    if (!parseUnsignedField(text, "nextPageId", loadedNext)) return false;

    rootPageId = loadedRoot;
    nextPageId = std::max<std::uint32_t>(1, loadedNext);
    metadataPath = path;
    pagesPath = path + ".pages";

    const std::string keyMarker = "\"keyType\"";
    const std::size_t keyPos = text.find(keyMarker);
    if (keyPos != std::string::npos) {
        const std::size_t colon = text.find(':', keyPos + keyMarker.size());
        const std::size_t q1 = text.find('"', colon + 1);
        const std::size_t q2 = text.find('"', q1 + 1);
        if (q1 != std::string::npos && q2 != std::string::npos) keyType = text.substr(q1 + 1, q2 - q1 - 1);
    }
    parseIntField(text, "order", order);

    return true;
}

bool BPlusTree::readLegacyPage(const std::string& json,
                               std::size_t objectStart,
                               std::size_t objectEnd,
                               BPlusNode& page) const {
    const std::string object = json.substr(objectStart, objectEnd - objectStart + 1);

    std::uint32_t pageId = 0;
    if (!parseUnsignedField(object, "pageId", pageId)) return false;
    page.pageId = pageId;
    const std::size_t leafPos = object.find("\"isLeaf\"");
    const std::size_t colon = object.find(':', leafPos);
    page.isLeaf = leafPos != std::string::npos && colon != std::string::npos &&
                  object.find("true", colon) < object.find_first_of(",}", colon);

    page.keys.clear();
    page.values.clear();
    page.children.clear();
    page.nextPageId = 0;

    const std::size_t keysPos = object.find("\"keys\":[");
    if (keysPos == std::string::npos) return false;
    const std::size_t keysBegin = keysPos + 8;
    const std::size_t keysEnd = object.find(']', keysBegin);
    if (keysEnd == std::string::npos) return false;
    std::size_t q = keysBegin;
    while (q < keysEnd) {
        const std::size_t q1 = object.find('"', q);
        if (q1 == std::string::npos || q1 >= keysEnd) break;
        const std::size_t q2 = object.find('"', q1 + 1);
        if (q2 == std::string::npos || q2 > keysEnd) return false;
        page.keys.push_back(object.substr(q1 + 1, q2 - q1 - 1));
        q = q2 + 1;
    }

    if (page.isLeaf) {
        const std::size_t idsPos = object.find("\"rowIds\":[");
        if (idsPos != std::string::npos) {
            const std::size_t idsBegin = idsPos + 10;
            const std::size_t idsEnd = object.find(']', idsBegin);
            if (idsEnd == std::string::npos) return false;
            std::size_t p = idsBegin;
            while (p < idsEnd) {
                const std::size_t comma = object.find(',', p);
                const std::size_t end = comma == std::string::npos || comma > idsEnd ? idsEnd : comma;
                const std::string number = trimCopy(object.substr(p, end - p));
                if (!number.empty()) {
                    try { page.values.push_back(static_cast<size_t>(std::stoull(number))); }
                    catch (...) { return false; }
                }
                p = end + 1;
            }
        }
        std::uint32_t next = 0;
        if (parseUnsignedField(object, "nextLeafPageId", next)) {
            page.nextPageId = next;
        }
    } else {
        const std::size_t childrenPos = object.find("\"children\":[");
        if (childrenPos == std::string::npos) return false;
        const std::size_t begin = childrenPos + 12;
        const std::size_t end = object.find(']', begin);
        if (end == std::string::npos) return false;
        std::size_t p = begin;
        while (p < end) {
            const std::size_t comma = object.find(',', p);
            const std::size_t stop = comma == std::string::npos || comma > end ? end : comma;
            const std::string number = trimCopy(object.substr(p, stop - p));
            if (!number.empty()) {
                try { page.children.push_back(static_cast<std::uint32_t>(std::stoul(number))); }
                catch (...) { return false; }
            }
            p = stop + 1;
        }
    }
    return true;
}

bool BPlusTree::migrateLegacyJson(const std::string& path) {
    std::ifstream in(path.c_str(), std::ios::binary);
    if (!in.is_open()) return false;
    std::stringstream ss;
    ss << in.rdbuf();
    const std::string json = ss.str();

    std::uint32_t oldRoot = 0;
    if (!parseUnsignedField(json, "rootPageId", oldRoot)) return false;
    int oldOrder = order;
    parseIntField(json, "order", oldOrder);
    order = std::max(3, oldOrder);

    std::uint32_t maxPage = 0;
    ensurePageFile();
    BPlusNode page(true);

    std::size_t pos = json.find("{\"pageId\":");
    while (pos != std::string::npos) {
        const std::size_t end = json.find('}', pos);
        if (end == std::string::npos) return false;
        if (!readLegacyPage(json, pos, end, page)) return false;
        maxPage = std::max(maxPage, page.pageId);
        if (!writePage(page.pageId, page)) return false;
        pos = json.find("{\"pageId\":", end + 1);
    }

    rootPageId = oldRoot;
    nextPageId = maxPage + 1;
    metadataPath = path;
    pagesPath = path + ".pages";
    persistMetadata();
    return rootPageId != 0;
}

void BPlusTree::saveToJsonFile(const std::string& path, const std::string& column) {
    invalidateRootView();
    setStoragePath(path, column);
}

bool BPlusTree::loadFromJsonFile(const std::string& path) {
    invalidateRootView();
    int version = 0;
    if (loadMetadata(path, version)) {
        metadataColumn.clear();
        if (version >= 2) {
            return ensurePageFile();
        }
    }

    // Version 1 was the old implementation where all BPlusNode objects were
    // recreated in RAM. Convert it once into page storage, one node at a time.
    return migrateLegacyJson(path);
}

int BPlusTree::getMaxPageId() const {
    return nextPageId == 0 ? 0 : static_cast<int>(nextPageId - 1);
}
