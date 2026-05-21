#pragma once

#include <string>

struct Value {
    enum Type {
        INT,
        STRING,
        NULL_VALUE
    };

    Type type = NULL_VALUE;
    int intValue = 0;
    std::string stringValue;
    bool isNull = true;
};