#include "database.h"
#include "../parser/parser.h"

#include <iostream>
#include <direct.h>
#include <io.h>
#include <cstdlib>
#include <sstream>

const std::string DATA_DIR = "data";
std::string currentDatabase;

static bool pathExists(const std::string& path) {
    return _access(path.c_str(), 0) == 0;
}

static bool createDirectory(const std::string& path) {
    return _mkdir(path.c_str()) == 0;
}

static void removeAll(const std::string& path) {
    std::string command = "rmdir /s /q \"" + path + "\"";
    system(command.c_str());
}

void ensureDataDirectory() {
    if (!pathExists(DATA_DIR)) {
        createDirectory(DATA_DIR);
    }
}

void createDatabase(const std::string& dbName) {
    if (!isValidName(dbName)) {
        std::cout << "Error: invalid database name\n";
        return;
    }

    ensureDataDirectory();

    std::string dbPath = DATA_DIR + "/" + dbName;

    if (pathExists(dbPath)) {
        std::cout << "Error: database already exists\n";
        return;
    }

    createDirectory(dbPath);
    std::cout << "Database '" << dbName << "' created\n";
}

void dropDatabase(const std::string& dbName) {
    if (!isValidName(dbName)) {
        std::cout << "Error: invalid database name\n";
        return;
    }

    std::string dbPath = DATA_DIR + "/" + dbName;

    if (!pathExists(dbPath)) {
        std::cout << "Error: database does not exist\n";
        return;
    }

    removeAll(dbPath);

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

    std::string dbPath = DATA_DIR + "/" + dbName;

    if (!pathExists(dbPath)) {
        std::cout << "Error: database does not exist\n";
        return;
    }

    currentDatabase = dbName;
    std::cout << "Using database '" << dbName << "'\n";
}
