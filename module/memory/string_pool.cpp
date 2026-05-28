#include "string_pool.h"
#include <fstream>
#include <iostream>

size_t StringPool::intern(const std::string& s) {
    auto it = strToId.find(s);
    if (it != strToId.end()) {
        return it->second;
    }
    size_t id = idToStr.size();
    strToId[s] = id;
    idToStr.push_back(s);
    return id;
}

const std::string& StringPool::resolve(size_t id) const {
    static const std::string empty = "";
    if (id < idToStr.size()) {
        return idToStr[id];
    }
    return empty;
}

size_t StringPool::size() const {
    return idToStr.size();
}

void StringPool::clear() {
    strToId.clear();
    idToStr.clear();
}

void StringPool::save(const std::string& path) const {
    std::ofstream out(path);
    if (!out) return;
    
    for (size_t i = 0; i < idToStr.size(); i++) {
        out << i << "|" << idToStr[i] << "\n";
    }
}

void StringPool::load(const std::string& path) {
    std::ifstream in(path);
    if (!in) return;
    
    clear();
    
    std::string line;
    while (std::getline(in, line)) {
        size_t pos = line.find('|');
        if (pos == std::string::npos) continue;
        
        size_t id = std::stoull(line.substr(0, pos));
        std::string str = line.substr(pos + 1);
        
        if (id >= idToStr.size()) {
            idToStr.resize(id + 1);
        }
        idToStr[id] = str;
        strToId[str] = id;
    }
}