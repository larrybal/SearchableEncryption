
#include <iostream>
#include <list>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sstream>
#include <unistd.h>
#include <iomanip>
#include <map>

#include "pibas.hpp"
#include "encryption.hpp"

#define KEYLEN 32
using namespace std;

void printPair(const std::pair<std::vector<unsigned char>, std::map<std::string, std::vector<unsigned char>>> &p)
{
    // Printing the vector<unsigned char> (first element of the pair)
    std::cout << "Vector (first element): ";
    for (const auto &byte : p.first)
    {
        std::cout << byte << " "; // Print each byte in the vector
    }
    std::cout << std::endl;

    // Printing the map<string, vector<unsigned char>> (second element of the pair)
    std::cout << "Map (second element):" << std::endl;
    for (const auto &entry : p.second)
    {
        std::cout << "Key: " << entry.first << " => Values: ";
        for (const auto &byte : entry.second)
        {
            std::cout << byte << " "; // Print each byte in the vector for each map entry
        }
        std::cout << std::endl;
    }
}
// Converts toHex for lookupKey use
std::string toHex(const vector<unsigned char> &data)
{
    stringstream ss;
    ss << hex << setfill('0');
    for (unsigned char c : data)
        ss << setw(2) << static_cast<int>(c);
    return ss.str();
}

// Setup function: creates an encrypted index
std::pair<vector<unsigned char>, map<string, vector<unsigned char>>> Pibas::Setup(
    const std::map<std::string, std::vector<std::string>> &D)
{
    std::vector<unsigned char> K = Encryption::generateKey(KEYLEN);

    // List L to store pairs
    std::vector<std::pair<std::string, vector<unsigned char>>> L;

    // For each keyword w in D
    for (const auto &entry : D)
    {
        const std::string &w = entry.first;
        const vector<string> &ids = entry.second;

        auto keys = Encryption::deriveKey(K, w);

        vector<unsigned char> K1 = keys.first;
        vector<unsigned char> K2 = keys.second;

        int c = 0;

        for (const string &id : ids)
        {
            string lookupKey = toHex(Encryption::computePRF(K1, to_string(c)));
            cout << "id: " << id << " lookup: " << lookupKey << endl;
            vector<unsigned char> d = Encryption::encryptAES(K2, id);
            L.push_back({lookupKey, d});
            c++;
        }
    }

    sort(L.begin(), L.end(), [](const auto &a, const auto &b)
         { return a.first < b.first; });

    map<string, vector<unsigned char>> ED;
    for (const auto &p : L)
    {
        ED[p.first] = p.second;
    }
    printPair({K, ED});
    return {K, ED};
}
void printPair2(const std::pair<std::vector<unsigned char>, std::vector<unsigned char>> &p)
{
    // Print the first vector (first element of the pair)
    std::cout << "First vector (pair.first):" << std::endl;
    for (const auto &byte : p.first)
    {
        std::cout << static_cast<int>(byte) << " "; // Cast to int for printing readable values
    }
    std::cout << std::endl;

    // Print the second vector (second element of the pair)
    std::cout << "Second vector (pair.second):" << std::endl;
    for (const auto &byte : p.second)
    {
        std::cout << static_cast<int>(byte) << " "; // Cast to int for printing readable values
    }
    std::cout << std::endl;
}
std::pair<vector<unsigned char>, vector<unsigned char>> Client(const vector<unsigned char> K, const std::string &w)
{
    vector<unsigned char> K1 = Encryption::computePRF(K, "1" + w);
    vector<unsigned char> K2 = Encryption::computePRF(K, "2" + w);
    printPair2({K1, K2});
    return {K1, K2};
}

std::vector<std::string> Server(const map<string, vector<unsigned char>> &ED,
                                const vector<unsigned char> &K1,
                                const vector<unsigned char> &K2)

{
    std::vector<std::string> output;
    int c = 0;

    while (1)
    {
        string tag = toHex(Encryption::computePRF(K1, std::to_string(c)));
        // std::string tag_s(tag.begin(), tag.end());
        cout << "tag s" << tag << endl;
        auto it = ED.find(tag);

        if (it == ED.end())
        {
            cout << "breaks after c: " << to_string(c) << endl;
            break;
        }

        vector<unsigned char> d = it->second;

        std::string id = Encryption::decryptAES(K2, d);
        std::cout << "id: " << id << std::endl;

        output.push_back(id);

        c++;
        std::cout << "output" << std::endl;
    }
    for (const std::string &str : output)
    {
        std::cout << str << std::endl; // Printing each string followed by a newline
    }
    return output;
}
void printSearchResult(const std::map<std::string, std::vector<unsigned char>> &ED,
                       const std::vector<unsigned char> &K,
                       const std::string &w)
{
    // Printing the map<string, vector<unsigned char>>
    std::cout << "Map (ED):" << std::endl;
    for (const auto &pair : ED)
    {
        std::cout << "Key: " << pair.first << std::endl;
        std::cout << "Values: ";
        for (const auto &value : pair.second)
        {
            std::cout << value << " ";
        }
        std::cout << std::endl;
    }

    // Printing the vector<unsigned char> (K)
    std::cout << "Vector (K):" << std::endl;
    for (const auto &byte : K)
    {
        std::cout << byte << std::endl;
    }

    // Printing the string (w)
    std::cout << "String (w): " << w << std::endl;
}

// Search function: retrieves document IDs for a given keyword
std::vector<std::string> Pibas::Search(
    const map<string, vector<unsigned char>> &ED,
    const vector<unsigned char> &K,
    const string &w)
{
    printSearchResult(ED, K, w);
    auto keys = Client(K, w);
    cout << "before server" << endl;
    vector<string> result = Server(ED, keys.first, keys.second);
    cout << "after server" << endl;
    return result;
}
