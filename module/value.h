#pragma once
#include <string>

struct Value {
    enum Type { INT, STRING, NULL_VALUE };
    Type type;

    int intValue;
    std::string stringValue;

    Value() : type(NULL_VALUE), intValue(0), stringValue("") {}

    Value(int v) : type(INT), intValue(v), stringValue("") {}

    Value(const std::string& s) : type(STRING), intValue(0), stringValue(s) {}
};
