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
    ofstream file("performance_results.csv");
    file << "DatasetSize,RangeSize,SetupTime,SearchTime,AccessedRecords\n";

    const int max_n = 1 << 20; // 2^20
    map<string, vector<string>> dataset;

    cout << "Generating dataset with n = " << max_n << " elements...\n";
    for (int i = 1; i <= max_n; i++)
        dataset[to_string(i)] = {"Value_" + to_string(i)};

    vector<unsigned char> K;
    map<string, vector<unsigned char>> ED;

    cout << "Encrypting dataset...\n";
    double setup_time = measure_time([&]()
                                     { 
        auto setup_result = Pibas::Setup(dataset);
        K = setup_result.first;
        ED = setup_result.second; });

    cout << "Setup Time: " << setup_time << " ms\n";

    cout << "Performing range queries...\n";
    for (int r = 1; r <= max_n; r *= 2)
    {
        cout << "Querying range: [1, " << r << "]\n";

        int total_accessed_records = 0;
        double total_search_time = measure_time([&]()
                                                {
            for (int i = 1; i <= r; i++) {
                string query = to_string(i);
                auto [results, accessed_records, search_time] = Pibas::Search(ED, K, query);
                total_accessed_records += accessed_records;
            } });

        cout << "Search Time for range " << r << ": " << total_search_time << " ms\n";
        file << max_n << "," << r << "," << setup_time << "," << total_search_time << "," << total_accessed_records << "\n";
    }

    file.close();
    return 0;
}
