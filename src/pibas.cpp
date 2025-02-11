#include <iostream>
#include <list>
#include "pibas.hpp"
#include "encryption.hpp"
#define KEYLEN 100
using namespace std;

// Setup function: creates an encrypted index
std::unordered_map<std::string, std::string> Setup(
    const std::unordered_map<std::string, std::vector<std::string>> &D)
{

    std::unordered_map<std::string, std::string> ED;
    // Implementation: Encrypt document IDs and store in ED
    vector<unsigned char> key;
    key = Encryption::generateKey(KEYLEN);
    list<int> *my_list = new list<int>;

    for ()

        return ED;
}

// Search function: retrieves document IDs for a given keyword
std::vector<std::string> Search(
    const std::unordered_map<std::string, std::string> &ED,
    const std::string &K,
    const std::string &w)
{

    std::vector<std::string> result;
    // Implementation: Decrypt ED[w] using K and return document IDs
    return result;
}