#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <utility>
#include <vector>

// Disk page size. Every B+ tree node is stored in exactly one page.
static const std::size_t BPLUS_PAGE_SIZE = 4096;

// A node is a transient in-memory representation of ONE disk page.
// It must never contain pointers to other BPlusNode objects.
struct BPlusNode {
    bool isLeaf;
    std::vector<std::string> keys;
    std::vector<size_t> values;       // leaf: row ids
    std::vector<std::uint32_t> children; // internal: page ids
    std::uint32_t nextPageId;
    std::uint32_t pageId;

    explicit BPlusNode(bool leaf = true);
};

class BPlusTree {
private:
    // Only page identifiers and small metadata live persistently in the tree object.
    std::uint32_t rootPageId;
    std::uint32_t nextPageId;
    int order;
    std::string keyType;

    // .idx is metadata; .idx.pages contains fixed-size B+ tree pages.
    std::string metadataPath;
    std::string pagesPath;
    std::string metadataColumn;

    // getRoot() exposes one temporary page only. Normal tree operations do not use it.
    mutable std::unique_ptr<BPlusNode> rootView;

    void invalidateRootView() const { rootView.reset(); }

    int compareKeys(const std::string& a, const std::string& b) const;

    bool ensurePageFile() const;
    bool readPage(std::uint32_t pageId, BPlusNode& node) const;
    bool writePage(std::uint32_t pageId, const BPlusNode& node) const;
    std::uint32_t allocatePage();
    void persistMetadata() const;
    bool loadMetadata(const std::string& path, int& version);
    bool migrateLegacyJson(const std::string& path);

    static bool parseUnsignedField(const std::string& text,
                                   const std::string& field,
                                   std::uint32_t& out);
    static bool parseIntField(const std::string& text,
                              const std::string& field,
                              int& out);
    static std::string jsonEscape(const std::string& value);

    bool findLeafId(const std::string& key,
                    std::vector<std::uint32_t>* path,
                    std::uint32_t& leafId) const;

    size_t lowerBoundInLeaf(const BPlusNode& leaf,
                            const std::string& key) const;
    size_t childIndexInInternal(const BPlusNode& node,
                                const std::string& key) const;

    // Splits a full page. Only one BPlusNode object is used; the other page is
    // produced directly from temporary vectors and immediately written to disk.
    bool splitLeafAndInsertIntoParent(std::uint32_t leafId,
                                      std::vector<std::uint32_t>& path,
                                      BPlusNode& page);
    bool splitInternalAndInsertIntoParent(std::uint32_t nodeId,
                                          std::vector<std::uint32_t>& path,
                                          BPlusNode& page);
    bool insertSeparatorIntoParent(std::uint32_t leftId,
                                   const std::string& separator,
                                   std::uint32_t rightId,
                                   std::vector<std::uint32_t>& path,
                                   BPlusNode& page);
    void growRoot(std::uint32_t leftId,
                  const std::string& separator,
                  std::uint32_t rightId,
                  BPlusNode& page);

    bool removeFromCurrentLeaf(std::uint32_t leafId,
                               const std::string& key,
                               BPlusNode& page);
    void refreshAncestorsAfterKeyChange(
        const std::vector<std::uint32_t>& path,
        std::uint32_t childId,
        const std::string& newMinimum,
        BPlusNode& page);
    bool borrowFromRight(std::uint32_t nodeId,
                         std::uint32_t parentId,
                         size_t index,
                         BPlusNode& page);
    bool borrowFromLeft(std::uint32_t nodeId,
                        std::uint32_t parentId,
                        size_t index,
                        BPlusNode& page);
    bool mergeWithSibling(std::uint32_t nodeId,
                          std::uint32_t parentId,
                          size_t index,
                          BPlusNode& page);
    void fixUnderflow(std::uint32_t nodeId,
                      std::vector<std::uint32_t>& path,
                      BPlusNode& page);
    void shrinkRoot(BPlusNode& page);

    bool readLegacyPage(const std::string& json,
                        std::size_t objectStart,
                        std::size_t objectEnd,
                        BPlusNode& page) const;

public:
    BPlusTree(int order, const std::string& keyType);
    ~BPlusTree();

    // Associates the tree with its on-disk page store. Called by IndexManager
    // immediately after an index is created/loaded.
    void setStoragePath(const std::string& path, const std::string& column);

    bool insert(const std::string& key, size_t value);
    bool remove(const std::string& key);
    bool search(const std::string& key, size_t& value) const;
    std::vector<size_t> searchAll(const std::string& key) const;
    std::vector<size_t> rangeSearch(const std::string& left, const std::string& right) const;

    // Returns one temporary root page. The pointer is invalidated by the next tree operation.
    BPlusNode* getRoot() const;

    std::vector<std::pair<std::string, size_t>> getAllKeyValues() const;

    void shiftRowIdsAfterDeleted(const std::vector<size_t>& deletedRowIds);
    void saveToJsonFile(const std::string& path, const std::string& column);
    bool loadFromJsonFile(const std::string& path);
    int getMaxPageId() const;
};
