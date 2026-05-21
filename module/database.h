#pragma once

#include <string>

extern const std::string DATA_DIR;
extern std::string currentDatabase;

struct Database {
    std::string name;
};

void ensureDataDirectory();
void createDatabase(const std::string& dbName);
void dropDatabase(const std::string& dbName);
void useDatabase(const std::string& dbName);