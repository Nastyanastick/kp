#pragma once
#include <string>
#include <vector>

std::string trim(const std::string& s);
std::string toUpper(const std::string& s);
std::vector<std::string> splitCommands(const std::string& text);
bool isValidName(const std::string& name);
void processCommand(const std::string& command);
void runInteractive();
void runBatch(const std::string& filename);
std::string executeSQLForServer(const std::string& sql);