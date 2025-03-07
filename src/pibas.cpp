#include <iostream>
#include <list>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "pibas.hpp"
#include "encryption.hpp"
#include <unistd.h>

#define KEYLEN 32
using namespace std;

// Converts toHex for lookupKey use
string toHex(const vector<unsigned char>& data) {
    stringstream ss;
    ss << hex << setfill('0');
    for (unsigned char c : data)
        ss << setw(2) << static_cast<int>(c);
    return ss.str();
}

// Setup function: creates an encrypted index
std::unordered_map<std::string, std::string> Setup(
    const std::unordered_map<std::string, std::vector<std::string>> &D)
{
    std::string k = generateKey(KEYLEN);

    // List L to store pairs
    std::vector<std::pair<std::string, std::string> L;

    // For each keyword w in D
    for (const auto &entry : D)
    {
        const std::string &w = entry.first;
        const vector<string> &ids = entry.second;

        auto keys = deriveKey(K, vector<string>{w});

        vector<unsigned char> K1 = keys.first;
        vector<unsigned char> K2 = keys.second;

        int c = 0;

        for (const string &id : ids) 
        {
            string lookupKey = toHex(computePRF(K1, to_string(c)));
            vector<unsigned char> d = encryptAES(K2, id);
            L.push_back({lookupKey, d});
            c++;
        }

    }

    sort(L.begin(), L.end(), [](const auto &a, const auto &b) {
        return a.first < b.first;
    });

    unordered_map<string, vector<unsigned char>> ED;
    for (const auto &p : L) {
        ED[p.first] = p.second;
    }
    
    return {K, ED};

}

std::pair<std::string, std::string> Client(const std::string &K, const std::string &w)
{
    std::string K1 = computePRF(K, "1" + w);
    std::string K2 = computePRF(K, "2" + w);

    return {K1, K2};
}

std::vector<std::string> Server(const std::unordered_map<std::string, std::string> &ED,
    const std::string &K1,
    const std::string &K2) 

{
    std::vector<std::string> output;
    int c = 0;

    while (1) {
        std::string tag = computePRF(K1, std::to_string(c));
        auto it = ED.find(tag);

        if (it == ED.end()) 
        {
            break;
        }

        std::string d = it->second;

        std::string id = decryptAES(K2, d);
        output.push_back(id);

        c++;
    }

    return output;
}

// Search function: retrieves document IDs for a given keyword
std::vector<std::string> Search(
    const std::unordered_map<std::string, std::string> &ED,
    const std::string &K,
    const std::string &w)
{
    auto keys = Client(K, w);
    return Server(ED, keys.first, keys.second);
}