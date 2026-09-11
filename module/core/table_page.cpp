#include "table_page.h"

#include <algorithm>
#include <cstring>
#include <limits>

namespace {

void writeUint16(
    std::vector<char>& buffer,
    std::size_t offset,
    std::uint16_t value
) {
    buffer[offset] = static_cast<char>(value & 0xFF);
    buffer[offset + 1] =
        static_cast<char>((value >> 8) & 0xFF);
}

void writeUint32(
    std::vector<char>& buffer,
    std::size_t offset,
    std::uint32_t value
) {
    buffer[offset] =
        static_cast<char>(value & 0xFF);

    buffer[offset + 1] =
        static_cast<char>((value >> 8) & 0xFF);

    buffer[offset + 2] =
        static_cast<char>((value >> 16) & 0xFF);

    buffer[offset + 3] =
        static_cast<char>((value >> 24) & 0xFF);
}

std::uint16_t readUint16(
    const std::vector<char>& buffer,
    std::size_t offset
) {
    return static_cast<std::uint16_t>(
        static_cast<unsigned char>(buffer[offset]) |
        (static_cast<std::uint16_t>(
             static_cast<unsigned char>(buffer[offset + 1]))
         << 8)
    );
}

std::uint32_t readUint32(
    const std::vector<char>& buffer,
    std::size_t offset
) {
    return
        static_cast<std::uint32_t>(
            static_cast<unsigned char>(buffer[offset])
        ) |
        (static_cast<std::uint32_t>(
             static_cast<unsigned char>(buffer[offset + 1]))
         << 8) |
        (static_cast<std::uint32_t>(
             static_cast<unsigned char>(buffer[offset + 2]))
         << 16) |
        (static_cast<std::uint32_t>(
             static_cast<unsigned char>(buffer[offset + 3]))
         << 24);
}

}

TablePage::TablePage(std::uint32_t pageId)
    : pageId_(pageId) {
}

std::uint32_t TablePage::pageId() const {
    return pageId_;
}

std::size_t TablePage::slotCount() const {
    return slots_.size();
}

std::size_t TablePage::dataEnd() const {
    std::size_t end = TABLE_PAGE_SIZE;

    for (const auto& slot : slots_) {
        if (!slot.occupied) {
            continue;
        }

        if (slot.offset < end) {
            end = slot.offset;
        }
    }

    return end;
}

std::size_t TablePage::freeSpace() const {
    const std::size_t slotDirectoryEnd =
        HEADER_SIZE + slots_.size() * SLOT_SIZE;

    const std::size_t recordStart = dataEnd();

    if (recordStart < slotDirectoryEnd) {
        return 0;
    }

    return recordStart - slotDirectoryEnd;
}

bool TablePage::hasFreeSpace(std::size_t recordSize) const {
    if (recordSize >
        std::numeric_limits<std::uint16_t>::max()) {
        return false;
    }

    return freeSpace() >=
           recordSize + SLOT_SIZE;
}

RecordID TablePage::insertRecord(
    const std::string& record
) {
    if (!hasFreeSpace(record.size())) {
        return RecordID{};
    }

    std::uint32_t slotId =
        static_cast<std::uint32_t>(slots_.size());

    Slot slot;
    slot.occupied = 1;
    slot.length =
        static_cast<std::uint16_t>(record.size());

    std::size_t newOffset = dataEnd();

    if (newOffset < record.size()) {
        return RecordID{};
    }

    newOffset -= record.size();

    if (newOffset <
        HEADER_SIZE +
        (slots_.size() + 1) * SLOT_SIZE) {
        return RecordID{};
    }

    slot.offset =
        static_cast<std::uint16_t>(newOffset);

    slots_.push_back(slot);
    records_.push_back(record);

    return RecordID(pageId_, slotId);
}

bool TablePage::readRecord(
    std::uint32_t slotId,
    std::string& record
) const {
    if (slotId >= slots_.size()) {
        return false;
    }

    if (!slots_[slotId].occupied) {
        return false;
    }

    record = records_[slotId];
    return true;
}

bool TablePage::updateRecord(
    std::uint32_t slotId,
    const std::string& record
) {
    if (slotId >= slots_.size()) {
        return false;
    }

    if (!slots_[slotId].occupied) {
        return false;
    }

    if (record.size() <=
        slots_[slotId].length) {

        records_[slotId] = record;
        slots_[slotId].length =
            static_cast<std::uint16_t>(record.size());

        return true;
    }

    return false;
}

bool TablePage::deleteRecord(
    std::uint32_t slotId
) {
    if (slotId >= slots_.size()) {
        return false;
    }

    if (!slots_[slotId].occupied) {
        return false;
    }

    slots_[slotId].occupied = 0;
    records_[slotId].clear();
    slots_[slotId].length = 0;
    slots_[slotId].offset = 0;

    return true;
}

bool TablePage::isSlotOccupied(
    std::uint32_t slotId
) const {
    return slotId < slots_.size() &&
           slots_[slotId].occupied != 0;
}

std::vector<std::uint32_t>
TablePage::occupiedSlots() const {
    std::vector<std::uint32_t> result;

    for (std::uint32_t i = 0;
         i < slots_.size();
         ++i) {

        if (slots_[i].occupied) {
            result.push_back(i);
        }
    }

    return result;
}

std::vector<char> TablePage::serialize() const {
    std::vector<char> buffer(
        TABLE_PAGE_SIZE,
        0
    );

    writeUint32(
        buffer,
        0,
        TABLE_PAGE_MAGIC
    );

    writeUint16(
        buffer,
        4,
        TABLE_PAGE_VERSION
    );

    writeUint16(
        buffer,
        6,
        static_cast<std::uint16_t>(
            slots_.size()
        )
    );

    writeUint32(
        buffer,
        8,
        pageId_
    );

    std::uint16_t freeEnd =
        static_cast<std::uint16_t>(
            dataEnd()
        );

    writeUint16(
        buffer,
        12,
        freeEnd
    );

    std::uint16_t slotCount =
        static_cast<std::uint16_t>(
            slots_.size()
        );

    writeUint16(
        buffer,
        14,
        slotCount
    );

    for (std::size_t i = 0;
         i < slots_.size();
         ++i) {

        const std::size_t slotOffset =
            HEADER_SIZE + i * SLOT_SIZE;

        writeUint16(
            buffer,
            slotOffset,
            slots_[i].offset
        );

        writeUint16(
            buffer,
            slotOffset + 2,
            slots_[i].length
        );

        buffer[slotOffset + 4] =
            static_cast<char>(
                slots_[i].occupied
            );
    }

    for (std::size_t i = 0;
         i < slots_.size();
         ++i) {

        if (!slots_[i].occupied) {
            continue;
        }

        const std::string& record =
            records_[i];

        std::memcpy(
            buffer.data() + slots_[i].offset,
            record.data(),
            record.size()
        );
    }

    return buffer;
}

bool TablePage::deserialize(
    const std::vector<char>& data,
    std::uint32_t expectedPageId,
    TablePage& page
) {
    if (data.size() != TABLE_PAGE_SIZE) {
        return false;
    }

    if (readUint32(data, 0) !=
        TABLE_PAGE_MAGIC) {
        return false;
    }

    if (readUint16(data, 4) !=
        TABLE_PAGE_VERSION) {
        return false;
    }

    const std::uint16_t slotCount =
        readUint16(data, 6);

    const std::uint32_t pageId =
        readUint32(data, 8);

    if (pageId != expectedPageId) {
        return false;
    }

    if (slotCount >
        (TABLE_PAGE_SIZE - HEADER_SIZE) /
            SLOT_SIZE) {
        return false;
    }

    page = TablePage(pageId);

    page.slots_.resize(slotCount);
    page.records_.resize(slotCount);

    for (std::size_t i = 0;
         i < slotCount;
         ++i) {

        const std::size_t offset =
            HEADER_SIZE + i * SLOT_SIZE;

        Slot slot;

        slot.offset =
            readUint16(data, offset);

        slot.length =
            readUint16(data, offset + 2);

        slot.occupied =
            static_cast<std::uint8_t>(
                data[offset + 4]
            );

        if (slot.occupied) {
            const std::size_t end =
                static_cast<std::size_t>(
                    slot.offset
                ) + slot.length;

            if (end > TABLE_PAGE_SIZE) {
                return false;
            }

            if (slot.offset <
                HEADER_SIZE +
                slotCount * SLOT_SIZE) {
                return false;
            }

            page.records_[i].assign(
                data.data() + slot.offset,
                slot.length
            );
        }

        page.slots_[i] = slot;
    }

    return true;
}