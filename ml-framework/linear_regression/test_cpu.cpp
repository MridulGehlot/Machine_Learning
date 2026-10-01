#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <iomanip>

#ifdef _WIN32
#include <windows.h>
#endif

int main() {
    std::cout << "========================================\n";
    std::cout << "       SYSTEM PERFORMANCE CHECK\n";
    std::cout << "========================================\n\n";

    // -------------------------------
    // CPU
    // -------------------------------
    unsigned int cores = std::thread::hardware_concurrency();

    std::cout << "[CPU]\n";
    std::cout << "Logical CPU cores visible to program: "
              << cores << "\n";

#ifdef _WIN32
    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);

    std::cout << "Windows reported logical processors: "
              << sysInfo.dwNumberOfProcessors << "\n";
#endif

    // -------------------------------
    // RAM
    // -------------------------------
#ifdef _WIN32
    MEMORYSTATUSEX memInfo;
    memInfo.dwLength = sizeof(memInfo);

    if (GlobalMemoryStatusEx(&memInfo)) {
        double totalGB =
            static_cast<double>(memInfo.ullTotalPhys) /
            (1024.0 * 1024.0 * 1024.0);

        double availableGB =
            static_cast<double>(memInfo.ullAvailPhys) /
            (1024.0 * 1024.0 * 1024.0);

        double usedGB = totalGB - availableGB;

        std::cout << "\n[RAM]\n";
        std::cout << std::fixed << std::setprecision(2);

        std::cout << "Total RAM:     "
                  << totalGB << " GB\n";

        std::cout << "Available RAM: "
                  << availableGB << " GB\n";

        std::cout << "Used RAM:      "
                  << usedGB << " GB\n";

        std::cout << "RAM usage:     "
                  << memInfo.dwMemoryLoad << "%\n";
    }
#endif

    // -------------------------------
    // CPU stress / timing test
    // -------------------------------
    std::cout << "\n========================================\n";
    std::cout << "       SINGLE THREAD TEST\n";
    std::cout << "========================================\n";

    volatile double result = 0.0;

    const long long ITERATIONS = 500000000LL;

    auto start = std::chrono::high_resolution_clock::now();

    for (long long i = 0; i < ITERATIONS; ++i) {
        result += 1.0 / (i + 1.0);
    }

    auto end = std::chrono::high_resolution_clock::now();

    double seconds =
        std::chrono::duration<double>(end - start).count();

    std::cout << "Result: " << result << "\n";
    std::cout << "Time:   " << seconds << " seconds\n";

    // -------------------------------
    // Multi-thread test
    // -------------------------------
    std::cout << "\n========================================\n";
    std::cout << "       MULTI THREAD TEST\n";
    std::cout << "========================================\n";

    unsigned int threadCount =
        std::thread::hardware_concurrency();

    if (threadCount == 0)
        threadCount = 1;

    std::cout << "Using " << threadCount
              << " threads\n";

    auto worker = [](long long iterations) {
        volatile double local = 0.0;

        for (long long i = 0; i < iterations; ++i) {
            local += 1.0 / (i + 1.0);
        }
    };

    std::vector<std::thread> threads;

    long long perThread =
        ITERATIONS / threadCount;

    start = std::chrono::high_resolution_clock::now();

    for (unsigned int i = 0; i < threadCount; ++i) {
        threads.emplace_back(worker, perThread);
    }

    for (auto& t : threads) {
        t.join();
    }

    end = std::chrono::high_resolution_clock::now();

    seconds =
        std::chrono::duration<double>(end - start).count();

    std::cout << "Time: " << seconds << " seconds\n";

    std::cout << "\n========================================\n";
    std::cout << "              DONE\n";
    std::cout << "========================================\n";

    return 0;
}