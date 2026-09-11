#pragma once

#include <cstdint>
#include <limits>

struct RecordID {
    std::uint32_t pageId = INVALID_PAGE_ID;
    std::uint32_t slotId = INVALID_SLOT_ID;

    static constexpr std::uint32_t INVALID_PAGE_ID =
        std::numeric_limits<std::uint32_t>::max();

    static constexpr std::uint32_t INVALID_SLOT_ID =
        std::numeric_limits<std::uint32_t>::max();

    RecordID() = default;

    RecordID(std::uint32_t page, std::uint32_t slot)
        : pageId(page), slotId(slot) {}

    bool isValid() const {
        return pageId != INVALID_PAGE_ID &&
               slotId != INVALID_SLOT_ID;
    }

    bool operator==(const RecordID& other) const {
        return pageId == other.pageId &&
               slotId == other.slotId;
    }

    bool operator!=(const RecordID& other) const {
        return !(*this == other);
    }
};