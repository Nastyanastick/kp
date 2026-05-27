#pragma once

#include <string>

std::string currentTime();

void writeAccessLog(
    int clientId,
    int handlerId,
    const std::string& request,
    const std::string& status,
    const std::string& startTime,
    const std::string& endTime
);