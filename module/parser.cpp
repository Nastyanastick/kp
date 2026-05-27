#include "parser.h"
#include "database.h"
#include "table.h"
#include "sql_parser.h"
#include "sql_ast.h"
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

    std::vector<sql::Command> cmds;
    if (!parse_sql(cleaned, cmds)) {
        std::cout << "Error: parse failed\n";
        return;
    }

    for (auto &c : cmds) {
        switch (c.type) {
            case sql::CmdType::CREATE_DATABASE:
                createDatabase(c.dbName);
                break;
            case sql::CmdType::DROP_DATABASE:
                dropDatabase(c.dbName);
                break;
            case sql::CmdType::USE_DATABASE:
                useDatabase(c.dbName);
                break;
            case sql::CmdType::CREATE_TABLE:
                createTableFromAST(c.createTable);
                break;
            case sql::CmdType::DROP_TABLE:
                dropTable(c.dropTableName);
                break;
            case sql::CmdType::INSERT:
                insertFromAST(c.insert);
                break;
            case sql::CmdType::SELECT:
                selectFromAST(c.select);
                break;
            case sql::CmdType::UPDATE:
                updateFromAST(c.update);
                break;
            case sql::CmdType::DELETE:
                deleteFromAST(c.del);
                break;
        }
    }
}

void runInteractive() {
    std::cout << "Interactive mode. Enter commands ending with ';'\n";
    std::cout << "Type EXIT; to quit\n";

    std::string line;
    std::string buffer;

    while (true) {
        std::cout << "> ";

        std::getline(std::cin, line);
        if (!std::cin) {
            break;
        }

        if (line == "EXIT;" || line == "exit;") {
            break;
        }

        buffer += line + '\n';

        if (buffer.find(';') != std::string::npos) {
            auto commands = splitCommands(buffer);

            for (const auto& cmd : commands) {
                processCommand(cmd);
            }

            buffer.clear();
        }
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

std::string executeSQLForServer(const std::string& sql) {
    std::ostringstream output;

    std::streambuf* oldCout = std::cout.rdbuf(output.rdbuf());
    std::streambuf* oldCerr = std::cerr.rdbuf(output.rdbuf());

    try {
        auto commands = splitCommands(sql);

        for (const auto& cmd : commands) {
            processCommand(cmd);
        }
    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << "\n";
    } catch (...) {
        std::cout << "Error: unknown error\n";
    }

    std::cout.rdbuf(oldCout);
    std::cerr.rdbuf(oldCerr);

    std::string result = output.str();

    if (result.empty()) {
        result = "OK\n";
    }

    return result;
}