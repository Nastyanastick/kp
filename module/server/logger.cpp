#include "logger.h"

#include <ctime>
#include <fstream>

std::string currentTime() {
    std::time_t now = std::time(nullptr);
    char buffer[64];

    std::strftime(
        buffer,
        sizeof(buffer),
        "%Y-%m-%d %H:%M:%S",
        std::localtime(&now)
    );

    return buffer;
}

void writeAccessLog(
    int clientId,
    int handlerId,
    const std::string& request,
    const std::string& status,
    const std::string& startTime,
    const std::string& endTime
) {
    std::ofstream logFile("access.log", std::ios::app);

    logFile << "client_id=" << clientId
            << " | handler_id=" << handlerId
            << " | start=" << startTime
            << " | end=" << endTime
            << " | status=" << status
            << " | request=" << request
            << "\n";
}