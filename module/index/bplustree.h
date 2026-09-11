#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "../core/record_id.h"

// Размер одной страницы B+ дерева.
static const std::size_t BPLUS_PAGE_SIZE = 4096;

// Временное представление одной страницы B+ дерева.
// Само дерево в RAM целиком не хранится.
struct BPlusNode {
    bool isLeaf;

    std::vector<std::string> keys;

    // Для листа:
    // key[i] -> values[i]
    // values[i] = физический адрес записи таблицы.
    std::vector<RecordID> values;

    // Для внутреннего узла:
    // page ids дочерних страниц.
    std::vector<std::uint32_t> children;

    // Следующий лист.
    std::uint32_t nextPageId;

    // ID этой страницы.
    std::uint32_t pageId;

    explicit BPlusNode(bool leaf = true);
};

class BPlusTree {
private:
    std::uint32_t rootPageId;
    std::uint32_t nextPageId;

    int order;
    std::string keyType;

    // .idx       — метаданные дерева.
    // .idx.pages — страницы B+ дерева.
    std::string metadataPath;
    std::string pagesPath;
    std::string metadataColumn;

    // Только временное представление одной страницы.
    mutable std::unique_ptr<BPlusNode> rootView;

    void invalidateRootView() const {
        rootView.reset();
    }

    int compareKeys(
        const std::string& a,
        const std::string& b
    ) const;

    bool ensurePageFile() const;

    bool readPage(
        std::uint32_t pageId,
        BPlusNode& node
    ) const;

    bool writePage(
        std::uint32_t pageId,
        const BPlusNode& node
    ) const;

    std::uint32_t allocatePage();

    void persistMetadata() const;

    bool loadMetadata(
        const std::string& path,
        int& version
    );

    static bool parseUnsignedField(
        const std::string& text,
        const std::string& field,
        std::uint32_t& out
    );

    static bool parseIntField(
        const std::string& text,
        const std::string& field,
        int& out
    );

    static std::string jsonEscape(
        const std::string& value
    );

    bool findLeafId(
        const std::string& key,
        std::vector<std::uint32_t>* path,
        std::uint32_t& leafId
    ) const;

    std::size_t lowerBoundInLeaf(
        const BPlusNode& leaf,
        const std::string& key
    ) const;

    std::size_t childIndexInInternal(
        const BPlusNode& node,
        const std::string& key
    ) const;

    bool splitLeafAndInsertIntoParent(
        std::uint32_t leafId,
        std::vector<std::uint32_t>& path,
        BPlusNode& page
    );

    bool splitInternalAndInsertIntoParent(
        std::uint32_t nodeId,
        std::vector<std::uint32_t>& path,
        BPlusNode& page
    );

    bool insertSeparatorIntoParent(
        std::uint32_t leftId,
        const std::string& separator,
        std::uint32_t rightId,
        std::vector<std::uint32_t>& path,
        BPlusNode& page
    );

    void growRoot(
        std::uint32_t leftId,
        const std::string& separator,
        std::uint32_t rightId,
        BPlusNode& page
    );

    bool removeFromCurrentLeaf(
        std::uint32_t leafId,
        const std::string& key,
        const RecordID& recordId,
        BPlusNode& page
    );

    void refreshAncestorsAfterKeyChange(
        const std::vector<std::uint32_t>& path,
        std::uint32_t childId,
        const std::string& newMinimum,
        BPlusNode& page
    );

    bool borrowFromRight(
        std::uint32_t nodeId,
        std::uint32_t parentId,
        std::size_t index,
        BPlusNode& page
    );

    bool borrowFromLeft(
        std::uint32_t nodeId,
        std::uint32_t parentId,
        std::size_t index,
        BPlusNode& page
    );

    bool mergeWithSibling(
        std::uint32_t nodeId,
        std::uint32_t parentId,
        std::size_t index,
        BPlusNode& page
    );

    void fixUnderflow(
        std::uint32_t nodeId,
        std::vector<std::uint32_t>& path,
        BPlusNode& page
    );

    void shrinkRoot(BPlusNode& page);

public:
    BPlusTree(
        int order,
        const std::string& keyType
    );

    ~BPlusTree();

    void setStoragePath(
        const std::string& path,
        const std::string& column
    );

    bool insert(
        const std::string& key,
        const RecordID& recordId
    );

    bool remove(
        const std::string& key,
        const RecordID& recordId
    );

    bool search(
        const std::string& key,
        RecordID& recordId
    ) const;

    std::vector<RecordID> searchAll(
        const std::string& key
    ) const;

    std::vector<RecordID> rangeSearch(
        const std::string& left,
        const std::string& right
    ) const;

    BPlusNode* getRoot() const;

    std::vector<std::pair<std::string, RecordID>>
    getAllKeyValues() const;

    void saveToJsonFile(
        const std::string& path,
        const std::string& column
    );

    bool loadFromJsonFile(
        const std::string& path
    );

    int getMaxPageId() const;
};