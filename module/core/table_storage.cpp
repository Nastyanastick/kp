#include "table_storage.h"

#include <fstream>

TableStorage::TableStorage(
    const std::string& tablePath
) {
    filePath = tablePath + "/pages.dat";
}

bool TableStorage::exists() const {
    std::ifstream in(
        filePath,
        std::ios::binary
    );

    return in.good();
}

bool TableStorage::create() {
    std::ofstream out(
        filePath,
        std::ios::binary |
        std::ios::trunc
    );

    return out.good();
}

std::uint32_t
TableStorage::calculatePageCount() const {
    std::ifstream in(
        filePath,
        std::ios::binary |
        std::ios::ate
    );

    if (!in) {
        return 0;
    }

    const std::streamoff size =
        in.tellg();

    if (size <= 0) {
        return 0;
    }

    if (size %
        static_cast<std::streamoff>(
            TABLE_PAGE_SIZE
        ) != 0) {
        return 0;
    }

    return static_cast<std::uint32_t>(
        size /
        static_cast<std::streamoff>(
            TABLE_PAGE_SIZE
        )
    );
}

std::uint32_t TableStorage::pageCount() const {
    return calculatePageCount();
}

bool TableStorage::readPage(
    std::uint32_t pageId,
    TablePage& page
) const {
    std::ifstream in(
        filePath,
        std::ios::binary
    );

    if (!in) {
        return false;
    }

    const std::streamoff offset =
        static_cast<std::streamoff>(
            pageId
        ) * TABLE_PAGE_SIZE;

    in.seekg(offset);

    if (!in) {
        return false;
    }

    std::vector<char> buffer(
        TABLE_PAGE_SIZE
    );

    in.read(
        buffer.data(),
        TABLE_PAGE_SIZE
    );

    if (in.gcount() !=
        static_cast<std::streamsize>(
            TABLE_PAGE_SIZE
        )) {
        return false;
    }

    return TablePage::deserialize(
        buffer,
        pageId,
        page
    );
}

bool TableStorage::writePage(
    const TablePage& page
) {
    std::fstream file(
        filePath,
        std::ios::binary |
        std::ios::in |
        std::ios::out
    );

    if (!file) {
        return false;
    }

    const std::streamoff offset =
        static_cast<std::streamoff>(
            page.pageId()
        ) * TABLE_PAGE_SIZE;

    file.seekp(offset);

    if (!file) {
        return false;
    }

    const std::vector<char> data =
        page.serialize();

    file.write(
        data.data(),
        static_cast<std::streamsize>(
            data.size()
        )
    );

    file.flush();

    return file.good();
}

bool TableStorage::appendPage(
    const TablePage& page
) {
    std::ofstream out(
        filePath,
        std::ios::binary |
        std::ios::app
    );

    if (!out) {
        return false;
    }

    const std::vector<char> data =
        page.serialize();

    out.write(
        data.data(),
        static_cast<std::streamsize>(
            data.size()
        )
    );

    return out.good();
}

RecordID TableStorage::insertRecord(
    const std::string& record
) {
    const std::uint32_t pages =
        calculatePageCount();

    for (std::uint32_t pageId = 0;
         pageId < pages;
         ++pageId) {

        TablePage page;

        if (!readPage(pageId, page)) {
            return RecordID{};
        }

        if (!page.hasFreeSpace(
                record.size())) {
            continue;
        }

        RecordID rid =
            page.insertRecord(record);

        if (!rid.isValid()) {
            continue;
        }

        if (!writePage(page)) {
            return RecordID{};
        }

        return rid;
    }

    TablePage newPage(pages);

    RecordID rid =
        newPage.insertRecord(record);

    if (!rid.isValid()) {
        return RecordID{};
    }

    if (!appendPage(newPage)) {
        return RecordID{};
    }

    return rid;
}

bool TableStorage::readRecord(
    const RecordID& rid,
    std::string& record
) const {
    if (!rid.isValid()) {
        return false;
    }

    TablePage page;

    if (!readPage(rid.pageId, page)) {
        return false;
    }

    return page.readRecord(
        rid.slotId,
        record
    );
}

bool TableStorage::updateRecord(
    const RecordID& rid,
    const std::string& record
) {
    if (!rid.isValid()) {
        return false;
    }

    TablePage page;

    if (!readPage(rid.pageId, page)) {
        return false;
    }

    if (!page.updateRecord(
            rid.slotId,
            record)) {
        return false;
    }

    return writePage(page);
}

bool TableStorage::deleteRecord(
    const RecordID& rid
) {
    if (!rid.isValid()) {
        return false;
    }

    TablePage page;

    if (!readPage(rid.pageId, page)) {
        return false;
    }

    if (!page.deleteRecord(
            rid.slotId)) {
        return false;
    }

    return writePage(page);
}

std::vector<RecordID>
TableStorage::getAllRecordIds() const {
    std::vector<RecordID> result;

    const std::uint32_t pages =
        calculatePageCount();

    for (std::uint32_t pageId = 0;
         pageId < pages;
         ++pageId) {

        TablePage page;

        if (!readPage(pageId, page)) {
            continue;
        }

        for (std::uint32_t slotId :
             page.occupiedSlots()) {

            result.emplace_back(
                pageId,
                slotId
            );
        }
    }

    return result;
}