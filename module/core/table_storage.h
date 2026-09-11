#pragma once                // файл всей таблицы, состоящей из страниц

#include <cstdint>
#include <string>
#include <vector>

#include "record_id.h"
#include "table_page.h"

class TableStorage {
private:
    std::string filePath;

public:
    explicit TableStorage(
        const std::string& tablePath
    );

    bool create();

    bool exists() const;

    std::uint32_t pageCount() const;

    RecordID insertRecord(
        const std::string& record
    );

    bool readRecord(
        const RecordID& rid,
        std::string& record
    ) const;

    bool updateRecord(
        const RecordID& rid,
        const std::string& record
    );

    bool deleteRecord(
        const RecordID& rid
    );

    std::vector<RecordID> getAllRecordIds() const;

private:
    bool readPage(
        std::uint32_t pageId,
        TablePage& page
    ) const;

    bool writePage(
        const TablePage& page
    );

    bool appendPage(
        const TablePage& page
    );

    std::uint32_t calculatePageCount() const;
};