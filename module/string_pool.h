#pragma once

#include <string>
#include <unordered_map>
#include <vector>

class StringPool {
private:
    std::unordered_map<std::string, size_t> strToId;
    std::vector<std::string> idToStr;

public:
    size_t intern(const std::string& s);
    const std::string& resolve(size_t id) const;
    size_t size() const;
    void clear();
    void save(const std::string& path) const;
    void load(const std::string& path);
};