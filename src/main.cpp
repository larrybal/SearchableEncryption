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

    return 0;
}