#include "encryption.hpp"
#include "pibas.hpp"

#include <iostream>
#include <unordered_map>
#include <string>

int main() {
    // 1. Create a synthetic dataset (map of Age to tuple IDs)
    // Key, Value
    std::unordered_map<std::string, std::string> dataset;
    dataset["27"] = {"emp1", "emp2"};
    dataset["28"] = {"emp3"};
    dataset["29"] = {"emp4", "emp5", "emp6"};
    dataset["30"] = {"emp7"};
    dataset["31"] = {"emp8", "emp9"};

    // Master key
    std::string masterKey = "masterkey";

    // 2. Run Setup and generate the encrypted index ED
    std::map<std::string, std::string> ED = Setup(dataset, masterKey);

    // 3. For a given age (point query), generate search token, run Search, and decrypt results
    std::string searchAge = "27"; 
    std::vector<std::string> searchResults = Search(ED, masterKey, searchAge);

    // Printing dataset contents 
    std::cout << "Dataset Contents:" << std::endl;

    return 0;
}