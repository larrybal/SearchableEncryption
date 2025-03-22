#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <chrono>
#include <fstream>
#include <sys/resource.h> // For getrusage on Unix
#include "log_brc.hpp"

using namespace std;
using namespace chrono;

// Function to measure memory usage in KB
long getMemoryUsage()
{
    struct rusage usage;
    getrusage(RUSAGE_SELF, &usage);
    return usage.ru_maxrss; // Memory usage in KB
}

// Function to measure execution time
double measure_time(function<void()> func)
{
    auto start = high_resolution_clock::now();
    func();
    auto stop = high_resolution_clock::now();
    return duration<double, milli>(stop - start).count();
}

int main()
{
    ofstream file("performance_results_static_range.csv");
    file << "DatasetSize,RangeSize,SetupTime,SearchTime,MemoryUsageKB\n"; // Added MemoryUsageKB

    const int max_n = 10000;     // Largest dataset size
    const int min_n = 10;        // Smallest dataset size
    const int query_length = 10; // Fixed query length

    map<int, vector<int>> dataset;

    for (int n = min_n; n <= max_n; n += 400)
    {
        // Generate dataset for current n
        cout << "Generating dataset with n = " << n << " elements...\n";
        dataset.clear();
        for (int i = 1; i <= n; i++)
            dataset[i] = {i}; // Simplified dataset

        int domainSize = 20000;

        auto start_setup = std::chrono::high_resolution_clock::now();
        long memory_before = getMemoryUsage();

        LogBRC logBRC(domainSize);

        long memory_after = getMemoryUsage();
        long memory_used = memory_after - memory_before;

        auto end_setup = std::chrono::high_resolution_clock::now();
        double setup_time = std::chrono::duration<double>(end_setup - start_setup).count();

        // Measure memory before building the index
        // memory_before = getMemoryUsage();

        // Measure setup time
        cout << "Building index...\n";
        setup_time += measure_time([&]()
                                   { logBRC.buildIndex(dataset); });
        // Measure memory after building the index
        // memory_after = getMemoryUsage();
        // memory_used += (memory_after - memory_before);

        cout << "Setup Time: " << setup_time << " ms, Memory Usage: " << memory_used << " KB\n";

        // Perform range query
        cout << "Performing range queries...\n";
        int total_accessed_records = 0;
        double total_search_time = measure_time([&]()
                                                {
                                                    int query_start = 1;
                                                    int query_end = query_start + query_length - 1;
                                                    vector<string> tokens = logBRC.trpdr(query_start, query_end);
                                                    set<int> result = logBRC.search(tokens);
                                                    total_accessed_records = result.size(); });

        cout << "Search Time for range [1, 10]: " << total_search_time << " ms\n";
        file << n << "," << query_length << "," << setup_time << "," << total_search_time << "," << memory_used << "\n";
    }

    file.close();
    return 0;
}
