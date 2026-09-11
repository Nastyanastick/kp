#pragma once        // одна физическая страница таблицы

#include <cstdint>
#include <string>
#include <vector>

#include "record_id.h"

static constexpr std::uint32_t TABLE_PAGE_SIZE = 4096;
static constexpr std::uint32_t TABLE_PAGE_MAGIC = 0x54424C50;
static constexpr std::uint16_t TABLE_PAGE_VERSION = 1;

class TablePage {
public:
    struct Slot {
        std::uint16_t offset = 0;
        std::uint16_t length = 0;
        std::uint8_t occupied = 0;
    };

private:
    std::uint32_t pageId_;
    std::vector<Slot> slots_;
    std::vector<std::string> records_;

    static constexpr std::size_t HEADER_SIZE = 16;
    static constexpr std::size_t SLOT_SIZE = 6;

public:
    explicit TablePage(std::uint32_t pageId = 0);

    std::uint32_t pageId() const;

    std::size_t slotCount() const;

    bool hasFreeSpace(std::size_t recordSize) const;

    RecordID insertRecord(const std::string& record);

    bool readRecord(
        std::uint32_t slotId,
        std::string& record
    ) const;

    bool updateRecord(
        std::uint32_t slotId,
        const std::string& record
    );

    bool deleteRecord(std::uint32_t slotId);

    bool isSlotOccupied(std::uint32_t slotId) const;

    std::vector<std::uint32_t> occupiedSlots() const;

    std::vector<char> serialize() const;

    static bool deserialize(
        const std::vector<char>& data,
        std::uint32_t expectedPageId,
        TablePage& page
    );

private:
    std::size_t freeSpace() const;

    std::size_t dataEnd() const;

    void rebuildOffsets();
};