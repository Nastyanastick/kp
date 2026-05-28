#pragma once

#include <atomic>
#include <chrono>
#include <mutex>
#include <thread>
#include <vector>

struct RequestMetric {
    std::chrono::steady_clock::time_point timestamp;
    long long durationMs;
    bool isError;
};

class Telemetry {
private:
    std::vector<RequestMetric> requests;
    std::mutex mtx;

    std::atomic<bool> running;
    std::thread printerThread;

    void cleanupOld();
    void printLoop();

public:
    Telemetry();
    ~Telemetry();

    void recordRequest(long long durationMs, bool isError);

    void start();
    void stop();
};