#include "database.h"
#include "parser.h"

#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

const std::string DATA_DIR = "data";
std::string currentDatabase;

void ensureDataDirectory() {
    if (!fs::exists(DATA_DIR)) {
        fs::create_directory(DATA_DIR);
    }
}

void createDatabase(const std::string& dbName) {
    if (!isValidName(dbName)) {
        std::cout << "Error: invalid database name\n";
        return;
    }

    ensureDataDirectory();

    fs::path dbPath = fs::path(DATA_DIR) / dbName;

    if (fs::exists(dbPath)) {
        std::cout << "Error: database already exists\n";
        return;
    }

    fs::create_directory(dbPath);
    std::cout << "Database '" << dbName << "' created\n";
}

void dropDatabase(const std::string& dbName) {
    if (!isValidName(dbName)) {
        std::cout << "Error: invalid database name\n";
        return;
    }

    fs::path dbPath = fs::path(DATA_DIR) / dbName;

    if (!fs::exists(dbPath)) {
        std::cout << "Error: database does not exist\n";
        return;
    }

    fs::remove_all(dbPath);

    if (currentDatabase == dbName) {
        currentDatabase.clear();
    }

    std::cout << "Database '" << dbName << "' dropped\n";
}

void useDatabase(const std::string& dbName) {
    if (!isValidName(dbName)) {
        std::cout << "Error: invalid database name\n";
        return;
    }

    fs::path dbPath = fs::path(DATA_DIR) / dbName;

    if (!fs::exists(dbPath)) {
        std::cout << "Error: database does not exist\n";
        return;
    }

    currentDatabase = dbName;
    std::cout << "Using database '" << dbName << "'\n";
}