#include "encryption.hpp"
#include "pibas.hpp"

#include <iostream>
#include <string>
#include <map>

int main()
{
    // 1. Create a synthetic dataset (map of Age to tuple IDs)
    // Key, Value
    std::map<std::string, vector<std::string>> dataset;
    dataset["27"] = {"emp1", "emp2"};
    dataset["28"] = {"emp3"};
    dataset["29"] = {"emp4", "emp5", "emp6"};
    dataset["30"] = {"emp7"};
    dataset["31"] = {"emp8", "emp9"};

    // Master key
    const vector<unsigned char> masterKey(
        reinterpret_cast<const unsigned char *>("masterkey"),
        reinterpret_cast<const unsigned char *>("masterkey") + strlen("masterkey"));

    // 2. Run Setup and generate the encrypted index ED
    pair<vector<unsigned char>, std::map<std::string, std::vector<unsigned char>>> K_ED = Pibas::Setup(dataset);
    vector<unsigned char> K = K_ED.first;
    map<std::string, std::vector<unsigned char>> ED = K_ED.second;

    // 3. For a given age (point query), generate search token, run Search, and decrypt results
    std::string searchAge = "27";
    std::vector<std::string> searchResults = Pibas::Search(ED, masterKey, searchAge);

    // Printing dataset contents
    std::cout << "Dataset Contents:" << std::endl;

    // 4. Compare decrypted results with expected tuple IDs
    std::cout << "Point Query for age " << searchAge << ":\n";
    std::cout << "Expected: ";
    for (const auto &tid : dataset[searchAge])
    {
        std::cout << tid << " ";
    }
    std::cout << "\nGot: ";
    for (const auto &tid : searchResults)
    {
        std::cout << tid << " ";
    }
    std::cout << "\n\n";

    // // 5. For a range query (ages 27 to 31), repeat Search for each age and aggregate results
    // std::vector<std::string> aggregatedResults;
    // std::cout << "Range Query for ages 27 to 31:\n";
    // for (int age = 27; age <= 31; age++) {
    //     std::string ageStr = std::to_string(age);
    //     std::vector<std::string> res = Pibas::Search(ED, masterKey, ageStr);
    //     aggregatedResults.insert(aggregatedResults.end(), res.begin(), res.end());
    //     std::cout << "Age " << ageStr << " results: ";
    //     for (const auto &tid : res) {
    //         std::cout << tid << " ";
    //     }
    //     std::cout << "\n";
    // }

    // // 6. Verify that the aggregated results match the expected tuple IDs from the dataset for the range
    // std::vector<std::string> expectedAggregated;
    // for (int age = 27; age <= 31; age++) {
    //     std::string ageStr = std::to_string(age);
    //     expectedAggregated.insert(expectedAggregated.end(), dataset[ageStr].begin(), dataset[ageStr].end());
    // }

    // // Sort both vectors for comparison (if order is not significant)
    // std::sort(aggregatedResults.begin(), aggregatedResults.end());
    // std::sort(expectedAggregated.begin(), expectedAggregated.end());

    // bool match = (aggregatedResults == expectedAggregated);

    // std::cout << "\nAggregated Expected Results: ";
    // for (const auto &tid : expectedAggregated) {
    //     std::cout << tid << " ";
    // }
    // std::cout << "\nAggregated Got Results: ";
    // for (const auto &tid : aggregatedResults) {
    //     std::cout << tid << " ";
    // }
    // std::cout << "\n";

    // if (match) {
    //     std::cout << "\nRange query test passed.\n";
    // } else {
    //     std::cout << "\nRange query test FAILED.\n";
    // }

    return 0;
}