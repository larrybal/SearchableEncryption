// main.cpp
#include "log-brc.hpp"
#include <iostream>
#include <fstream> // Include fstream for file output
#include <random>
#include <chrono>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <sys/resource.h> // For memory usage

// Function to get memory usage (in bytes)
long long getMemoryUsage()
{
    struct rusage usage;
    getrusage(RUSAGE_SELF, &usage);
    return usage.ru_maxrss * 1024; // Convert from kilobytes to bytes
}

int main()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, 100000); // Adjust range as needed

    std::vector<int> datasetSizes = {100, 500, 1000, 5000, 10000, 20000};
    int numDatasets = 500;

    // Open the output file
    std::ofstream outputFile("results.csv");
    if (!outputFile.is_open())
    {
        std::cerr << "Error: Could not open output file." << std::endl;
        return 1;
    }

    // Print header to the output file (CSV format)
    outputFile << "DatasetSize,Accuracy,QueryTime,MemoryUsage" << std::endl;

    for (int datasetSize : datasetSizes)
    {
        double averageAccuracy = 0.0;
        double averageTime = 0.0;
        long long averageMemory = 0;

        for (int i = 0; i < numDatasets; ++i)
        {
            std::map<int, std::vector<int>> data;
            std::set<int> allIds;

            // Generate random dataset
            for (int j = 0; j < datasetSize; ++j)
            {
                int value = dist(gen);
                int id = j;
                data[value].push_back(id);
                allIds.insert(id);
            }

            int domainSize = 100001; // Should be greater than the max value generated.
            LogBRC logBRC(domainSize);
            logBRC.buildIndex(data);

            // Generate random query
            int queryStart = dist(gen) % (domainSize - 1);
            int queryEnd = queryStart + (dist(gen) % (domainSize - queryStart));

            // Perform query and measure time
            auto startTime = std::chrono::high_resolution_clock::now();
            long long memoryBefore = getMemoryUsage(); // Memory before query
            std::vector<std::string> tokens = logBRC.trpdr(queryStart, queryEnd);
            std::set<int> result = logBRC.search(tokens);
            long long memoryAfter = getMemoryUsage(); // Memory after query
            auto endTime = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> duration = endTime - startTime;
            averageTime += duration.count();
            averageMemory += (memoryAfter - memoryBefore); // Measure query memory usage

            // Calculate accuracy
            std::set<int> expectedResult;
            for (const auto &pair : data)
            {
                if (pair.first >= queryStart && pair.first <= queryEnd)
                {
                    expectedResult.insert(pair.second.begin(), pair.second.end());
                }
            }

            std::set<int> intersection;
            std::set_intersection(result.begin(), result.end(), expectedResult.begin(), expectedResult.end(), std::inserter(intersection, intersection.begin()));

            double accuracy = (double)intersection.size() / expectedResult.size();
            averageAccuracy += accuracy;
        }

        averageAccuracy /= numDatasets;
        averageTime /= numDatasets;
        averageMemory /= numDatasets;

        // Print data in CSV format to the output file
        outputFile << datasetSize << "," << averageAccuracy << "," << averageTime << "," << averageMemory << std::endl;
    }

    outputFile.close(); // Close the output file
    return 0;
}