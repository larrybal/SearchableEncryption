#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <chrono>
#include <fstream>
#include "pibas.hpp"

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
    ofstream file("performance_results_static_range.csv");
    file << "DatasetSize,RangeSize,SetupTime,SearchTime,AccessedRecords\n";

    // Set maximum database size n = 2^20
    const int max_n = 10000; // 2^20
    map<string, vector<string>> dataset;

    // Generate datasets with varying sizes
    for (int n = 10; n <= max_n; n += 400)
    {
        cout << "Generating dataset with n = " << n << " elements...\n";

        // Fill the dataset with n elements
        dataset.clear();
        for (int i = 1; i <= n; i++)
            dataset[to_string(i)] = {"Value_" + to_string(i)};

        vector<unsigned char> K;
        map<string, vector<unsigned char>> ED;

        // Encrypt the dataset
        cout << "Encrypting dataset...\n";
        double setup_time = measure_time([&]()
                                         { 
            auto setup_result = Pibas::Setup(dataset);
            K = setup_result.first;
            ED = setup_result.second; });

        cout << "Setup Time: " << setup_time << " ms\n";

        // Perform static range queries of size 10
        cout << "Performing static range queries of size 10...\n";
        const int range_size = 10;

        int total_accessed_records = 0;
        double total_search_time = measure_time([&]()
                                                {
            for (int i = 1; i <= n - range_size + 1; i++) {
                string query = to_string(i);
                auto [results, accessed_records, search_time] = Pibas::Search(ED, K, query);
                total_accessed_records += accessed_records;
            } });

        cout << "Search Time for dataset size " << n << ": " << total_search_time << " ms\n";
        file << n << "," << range_size << "," << setup_time << "," << total_search_time << "," << total_accessed_records << "\n";
    }

    file.close();
    return 0;
}
