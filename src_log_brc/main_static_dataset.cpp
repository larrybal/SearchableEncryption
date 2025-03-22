#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <chrono>
#include <fstream>
#include "log_brc.hpp"

using namespace std;
using namespace chrono;

double measure_time(function<void()> func)
{
    auto start = high_resolution_clock::now();
    func();
    auto stop = high_resolution_clock::now();
    return duration<double, milli>(stop - start).count();
}

int main()
{
    ofstream file("performance_results_static_dataset.csv");
    file << "DatasetSize,RangeSize,SetupTime,SearchTime\n";

    const int max_n = 300;
    map<int, vector<int>> dataset;

    cout << "Generating dataset with n = " << max_n << " elements...\n";
    for (int i = 1; i <= max_n; i++)
        dataset[i] = {i}; // Simplified dataset: each key maps to a value with the same key

    int domainSize = 1000; // Should be greater than the maximum value in the dataset
    LogBRC logBRC(domainSize);

    // Build index
    cout << "Building index...\n";
    double setup_time = measure_time([&]()
                                     { logBRC.buildIndex(dataset); });

    cout << "Setup Time: " << setup_time << " ms\n";

    cout << "Performing range queries...\n";
    for (int r = 1; r <= max_n; r += 5)
    {
        cout << "Querying range: [1, " << r << "]\n";

        int total_accessed_records = 0;
        double total_search_time = measure_time([&]()
                                                {
                                                    int query_start = 1;
                                                    int query_end = r;
                                                    // Generate a query range from 1 to i
                                                    vector<string> tokens = logBRC.trpdr(query_start, query_end);
                                                    set<int> result = logBRC.search(tokens);
                                                    total_accessed_records = result.size(); // Count the number of records accessed
                                                });

        cout << "Search Time for range " << r << ": " << total_search_time << " ms\n";
        file << max_n << "," << r << "," << setup_time << "," << total_search_time << "\n";
    }

    file.close();
    return 0;
}
