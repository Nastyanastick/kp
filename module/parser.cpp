#include "parser.h"
#include "database.h"
#include "table.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cctype>


std::string trim(const std::string& s) {
    size_t start = 0;
    while (start < s.size() && std::isspace(static_cast<unsigned char>(s[start]))) {
        start++;
    }

    size_t end = s.size();
    while (end > start && std::isspace(static_cast<unsigned char>(s[end - 1]))) {
        end--;
    }

    return s.substr(start, end - start);
}

std::string toUpper(const std::string& s) {
    std::string result = s;
    for (char& ch : result) {
        ch = static_cast<char>(std::toupper(static_cast<unsigned char>(ch)));
    }
    return result;
}

bool isValidKeywordCase(const std::string& word) {
    if (word.empty()) {
        return false;
    }

    bool hasLower = false;
    bool hasUpper = false;

    for (char ch : word) {
        if (std::isalpha(static_cast<unsigned char>(ch))) {
            if (std::islower(static_cast<unsigned char>(ch))) {
                hasLower = true;
            }
            if (std::isupper(static_cast<unsigned char>(ch))) {
                hasUpper = true;
            }
        }
    }

    return !(hasLower && hasUpper);
}

bool checkKeywordCases(const std::string& command) {
    std::istringstream iss(command);
    std::string word;

    while (iss >> word) {
        while (!word.empty() && (word.back() == '(' || word.back() == ',' || word.back() == ';')) {
            word.pop_back();
        }

        std::string upper = toUpper(word);

        if (upper == "CREATE" || upper == "DATABASE" || upper == "DROP" ||
            upper == "USE" || upper == "TABLE" || upper == "INSERT" ||
            upper == "INTO" || upper == "VALUE" || upper == "VALUES" ||
            upper == "SELECT" || upper == "FROM" || upper == "WHERE" ||
            upper == "UPDATE" || upper == "SET" || upper == "DELETE" ||
            upper == "AS" || upper == "BETWEEN" || upper == "AND" ||
            upper == "LIKE" || upper == "NOT_NULL" || upper == "INDEXED") {

            if (!isValidKeywordCase(word)) {
                return false;
            }
        }
    }

    return true;
}



bool isValidName(const std::string& name) {
    if (name.empty()) {
        return false;
    }

    if (std::isdigit(static_cast<unsigned char>(name[0]))) {
        return false;
    }

    for (char ch : name) {
        if (!(std::isalnum(static_cast<unsigned char>(ch)) || ch == '_')) {
            return false;
        }
    }

    return true;
}

std::vector<std::string> splitCommands(const std::string& text) {
    std::vector<std::string> commands;
    std::string current;

    for (char ch : text) {
        current += ch;
        if (ch == ';') {
            commands.push_back(current);
            current.clear();
        }
    }

    return commands;
}

void processCommand(const std::string& command) {
    std::string cleaned = trim(command);

    if (cleaned.empty()) {
        return;
    }

    if (cleaned.back() == ';') {
        cleaned.pop_back();
        cleaned = trim(cleaned);
    }

    if (!checkKeywordCases(cleaned)) {
        std::cout << "Error: invalid keyword case\n";
        return;
    }

    std::string upper = toUpper(cleaned);

    if (upper.rfind("CREATE DATABASE ", 0) == 0) {
        std::string dbName = trim(cleaned.substr(16));
        createDatabase(dbName);
    } else if (upper.rfind("DROP DATABASE ", 0) == 0) {
        std::string dbName = trim(cleaned.substr(14));
        dropDatabase(dbName);
    } else if (upper.rfind("USE ", 0) == 0) {
        std::string dbName = trim(cleaned.substr(4));
        useDatabase(dbName);
    } else if (upper.rfind("CREATE TABLE ", 0) == 0) {
        std::string body = trim(cleaned.substr(13));
        createTable(body);
    } else if (upper.rfind("DROP TABLE ", 0) == 0) {
        std::string tableName = trim(cleaned.substr(11));
        dropTable(tableName);
    } else if (upper.rfind("INSERT INTO ", 0) == 0) {
        std::string body = trim(cleaned.substr(12));
        insertInto(body);
    } else if (upper.rfind("SELECT ", 0) == 0) {
        std::string body = trim(cleaned.substr(7));
        selectFrom(body);
    } else if (upper.rfind("UPDATE ", 0) == 0) {
        std::string body = trim(cleaned.substr(7));
        updateRows(body);
    } else if (upper.rfind("DELETE FROM ", 0) == 0) {
        std::string body = trim(cleaned.substr(12));
        deleteFrom(body);
    } else {
        std::cout << "Error: unknown command\n";
    }
}

void runInteractive() {
    std::cout << "Interactive mode. Enter commands ending with ';'\n";

    std::string line;
    std::string buffer;

    while (true) {
        std::getline(std::cin, line);
        if (!std::cin) {
            break;
        }

        buffer += line + '\n';

        auto commands = splitCommands(buffer);

        size_t processedLength = 0;
        for (const auto& cmd : commands) {
            processCommand(cmd);
            processedLength += cmd.size();
        }

        buffer = buffer.substr(processedLength);
    }
}

void runBatch(const std::string& filename) {
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Error: cannot open file " << filename << std::endl;
        return;
    }

    std::stringstream ss;
    ss << file.rdbuf();
    std::string text = ss.str();

    auto commands = splitCommands(text);
    for (const auto& cmd : commands) {
        processCommand(cmd);
    }
}
