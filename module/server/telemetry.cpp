#include "telemetry.h"

#include <algorithm>
#include <iostream>

Telemetry::Telemetry() : running(false) {}

Telemetry::~Telemetry() {
    stop();
}

void Telemetry::recordRequest(long long durationMs, bool isError) {
    std::lock_guard<std::mutex> lock(mtx);

    requests.push_back({
        std::chrono::steady_clock::now(),
        durationMs,
        isError
    });

    cleanupOld();
}

void Telemetry::cleanupOld() {
    auto now = std::chrono::steady_clock::now();
    auto tenMinutesAgo = now - std::chrono::minutes(10);

    requests.erase(
        std::remove_if(
            requests.begin(),
            requests.end(),
            [tenMinutesAgo](const RequestMetric& metric) {
                return metric.timestamp < tenMinutesAgo;
            }
        ),
        requests.end()
    );
}

void Telemetry::start() {
    if (running) {
        return;
    }

    running = true;
    printerThread = std::thread(&Telemetry::printLoop, this);
}

void Telemetry::stop() {
    running = false;

    if (printerThread.joinable()) {
        printerThread.join();
    }
}

void Telemetry::printLoop() {
    while (running) {
        std::this_thread::sleep_for(std::chrono::seconds(5));

        std::lock_guard<std::mutex> lock(mtx);
        cleanupOld();

        auto now = std::chrono::steady_clock::now();

        int currentRequests = 0;
        int requestsLast10Min = 0;
        int maxRps = 0;

        long long durationSum10Sec = 0;
        int durationCount10Sec = 0;

        int errorsLastMinute = 0;
        int requestsLastMinute = 0;

        std::vector<int> rpsBuckets(600, 0);

        for (const auto& metric : requests) {
            auto ageSec = std::chrono::duration_cast<std::chrono::seconds>(
                now - metric.timestamp
            ).count();

            if (ageSec < 1) {
                currentRequests++;
            }

            if (ageSec < 600) {
                requestsLast10Min++;

                int bucketIndex = static_cast<int>(ageSec);
                if (bucketIndex >= 0 && bucketIndex < 600) {
                    rpsBuckets[bucketIndex]++;
                }
            }

            if (ageSec < 10) {
                durationSum10Sec += metric.durationMs;
                durationCount10Sec++;
            }

            if (ageSec < 60) {
                requestsLastMinute++;

                if (metric.isError) {
                    errorsLastMinute++;
                }
            }
        }

        for (int value : rpsBuckets) {
            if (value > maxRps) {
                maxRps = value;
            }
        }

        double avgRps10Min = requestsLast10Min / 600.0;

        double avgDuration10Sec = 0.0;
        if (durationCount10Sec > 0) {
            avgDuration10Sec =
                static_cast<double>(durationSum10Sec) / durationCount10Sec;
        }

        double errorRate = 0.0;
        if (requestsLastMinute > 0) {
            errorRate =
                static_cast<double>(errorsLastMinute) / requestsLastMinute * 100.0;
        }

        std::cout << "\n===== TELEMETRY =====\n";
        std::cout << "Current RPS: " << currentRequests << "\n";
        std::cout << "Average RPS last 10 min: " << avgRps10Min << "\n";
        std::cout << "Max RPS last 10 min: " << maxRps << "\n";
        std::cout << "Average processing time last 10 sec: " << avgDuration10Sec << " ms\n";
        std::cout << "Errors last minute: " << errorsLastMinute << "\n";
        std::cout << "Error Rate last minute: " << errorRate << "%\n";
        std::cout << "=====================\n";
    }
}