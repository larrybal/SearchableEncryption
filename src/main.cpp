#include "encryption.hpp"
#include "pibas.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <random>
#include <chrono>
#include <algorithm>
#include <set>
#include <sys/resource.h>

using namespace std;

// Function to get current memory usage in MB
double get_memory_usage()
{
    struct rusage usage;
    getrusage(RUSAGE_SELF, &usage);
    return usage.ru_maxrss / 1024.0; // Convert KB to MB
}

// Function to compute accuracy
double compute_accuracy(const vector<string> &expected, const vector<string> &got)
{
    set<string> expectedSet(expected.begin(), expected.end());
    set<string> gotSet(got.begin(), got.end());

    // Compute intersection (correct matches)
    int correctMatches = 0;
    for (const auto &tid : gotSet)
    {
        if (expectedSet.count(tid))
        {
            correctMatches++;
        }
    }

    // Avoid division by zero
    if (expectedSet.empty())
        return gotSet.empty() ? 1.0 : 0.0;
    return static_cast<double>(correctMatches) / expectedSet.size();
}

void run_experiment(int dataset_size, ofstream &outfile, int num_trials)
{
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> ageDist(18, 65); // Age range

    double totalSetupTime = 0, totalSearchTime = 0, totalRangeSearchTime = 0;
    double totalPointAccuracy = 0, totalRangeAccuracy = 0;
    double totalMemoryUsageSetup = 0, totalMemoryUsageSearch = 0, totalMemoryUsageRange = 0;

    for (int trial = 0; trial < num_trials; trial++)
    {
        // 1. Create a random dataset
        map<string, vector<string>> dataset;
        for (int i = 1; i <= dataset_size; i++)
        {
            string age = to_string(ageDist(gen));
            dataset[age].push_back("emp" + to_string(i));
        }

        // 2. Encrypt dataset
        double memBeforeSetup = get_memory_usage();
        auto startSetup = chrono::high_resolution_clock::now();
        pair<vector<unsigned char>, map<string, vector<unsigned char>>> K_ED = Pibas::Setup(dataset);
        auto endSetup = chrono::high_resolution_clock::now();
        double memAfterSetup = get_memory_usage();
        totalSetupTime += chrono::duration<double>(endSetup - startSetup).count();
        totalMemoryUsageSetup += (memAfterSetup - memBeforeSetup);

        vector<unsigned char> K = K_ED.first;
        map<string, vector<unsigned char>> ED = K_ED.second;

        // 3. Perform a point query
        string searchAge = to_string(ageDist(gen));
        double memBeforeSearch = get_memory_usage();
        auto startSearch = chrono::high_resolution_clock::now();
        vector<string> searchResults = Pibas::Search(ED, K, searchAge);
        auto endSearch = chrono::high_resolution_clock::now();
        double memAfterSearch = get_memory_usage();
        totalSearchTime += chrono::duration<double>(endSearch - startSearch).count();
        totalMemoryUsageSearch += (memAfterSearch - memBeforeSearch);

        // Compute point query accuracy
        totalPointAccuracy += compute_accuracy(dataset[searchAge], searchResults);

        // 4. Perform a range query (5 ages)
        int rangeStart = ageDist(gen);
        int rangeEnd = min(rangeStart + 5, 65);
        vector<string> aggregatedResults;
        vector<string> expectedAggregated;

        double memBeforeRange = get_memory_usage();
        auto startRangeSearch = chrono::high_resolution_clock::now();
        for (int age = rangeStart; age <= rangeEnd; age++)
        {
            string ageStr = to_string(age);
            vector<string> res = Pibas::Search(ED, K, ageStr);
            aggregatedResults.insert(aggregatedResults.end(), res.begin(), res.end());

            // Collect expected results
            expectedAggregated.insert(expectedAggregated.end(), dataset[ageStr].begin(), dataset[ageStr].end());
        }
        auto endRangeSearch = chrono::high_resolution_clock::now();
        double memAfterRange = get_memory_usage();
        totalRangeSearchTime += chrono::duration<double>(endRangeSearch - startRangeSearch).count();
        totalMemoryUsageRange += (memAfterRange - memBeforeRange);

        // Compute range query accuracy
        totalRangeAccuracy += compute_accuracy(expectedAggregated, aggregatedResults);
    }

    // Compute averages
    double avgSetupTime = totalSetupTime / num_trials;
    double avgSearchTime = totalSearchTime / num_trials;
    double avgRangeSearchTime = totalRangeSearchTime / num_trials;
    double avgPointAccuracy = totalPointAccuracy / num_trials;
    double avgRangeAccuracy = totalRangeAccuracy / num_trials;
    double avgMemorySetup = totalMemoryUsageSetup / num_trials;
    double avgMemorySearch = totalMemoryUsageSearch / num_trials;
    double avgMemoryRange = totalMemoryUsageRange / num_trials;

    // Save results
    outfile << dataset_size << "," << avgSetupTime << "," << avgSearchTime << ","
            << avgRangeSearchTime << "," << avgPointAccuracy << "," << avgRangeAccuracy
            << "," << avgMemorySetup << "," << avgMemorySearch << "," << avgMemoryRange << "\n";

    cout << "Dataset: " << dataset_size
         << " | Setup: " << avgSetupTime << "s | Memory: " << avgMemorySetup << "MB"
         << " | Point Query: " << avgSearchTime << "s | Memory: " << avgMemorySearch << "MB"
         << " | Range Query: " << avgRangeSearchTime << "s | Memory: " << avgMemoryRange << "MB"
         << " | Point Accuracy: " << avgPointAccuracy
         << " | Range Accuracy: " << avgRangeAccuracy << "\n";
}

int main()
{
    vector<int> dataset_sizes = {100, 500, 1000, 5000, 10000, 20000};
    int num_trials = 5000;

    ofstream outfile("performance_results.csv");
    outfile << "DatasetSize,AvgSetupTime,AvgPointQueryTime,AvgRangeQueryTime,AvgPointAccuracy,AvgRangeAccuracy,AvgMemorySetup,AvgMemorySearch,AvgMemoryRange\n";

    for (int size : dataset_sizes)
    {
        run_experiment(size, outfile, num_trials);
    }

    outfile.close();
    cout << "Performance and resource utilization data saved to performance_results.csv\n";
    return 0;
}
