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
#include <vector>
#include <algorithm>

#include "pibas.hpp"
#include "encryption.hpp"

#define KEYLEN 32
using namespace std;

// Helper: Print a pair containing the master key and encrypted database.
void printPair(const std::pair<std::vector<unsigned char>, std::map<std::string, std::vector<unsigned char>>> &p)
{
    // Print the master key.
    std::cout << "Master Key: ";
    for (const auto &byte : p.first)
    {
        std::cout << byte << " ";
    }
    std::cout << std::endl;

    // Print the encrypted database map.
    std::cout << "Encrypted Database (ED):" << std::endl;
    for (const auto &entry : p.second)
    {
        std::cout << "Lookup Tag: " << entry.first << " => Encrypted ID: ";
        for (const auto &byte : entry.second)
        {
            std::cout << byte << " ";
        }
        std::cout << std::endl;
    }
}

// Helper: Convert vector<unsigned char> to a hex string.
std::string toHex(const std::vector<unsigned char> &data)
{
    stringstream ss;
    ss << hex << setfill('0');
    for (unsigned char c : data)
        ss << setw(2) << static_cast<int>(c);
    return ss.str();
}

/*
 * Setup:
 * 1. Generate a master key.
 * 2. For each keyword w in the dataset D, derive keys as:
 *       K1 = F(masterKey, "1" + w)
 *       K2 = F(masterKey, "2" + w)
 * 3. For each id in id(w):
 *       Compute lookup tag ℓ = F(K1, c)
 *       Encrypt the id using K2.
 * 4. Add (ℓ, encrypted id) to list L (sorted lexicographically).
 * 5. Create the encrypted database ED from L.
 */
std::pair<std::vector<unsigned char>, std::map<std::string, std::vector<unsigned char>>> Pibas::Setup(
    const std::map<std::string, std::vector<std::string>> &D)
{
    // Generate the master key.
    std::vector<unsigned char> masterKey = Encryption::generateKey(KEYLEN);

    // List to store pairs (lookup tag, encrypted id).
    std::vector<std::pair<std::string, std::vector<unsigned char>>> L;

    // For each keyword w in the dataset.
    for (const auto &entry : D)
    {
        const std::string &w = entry.first;
        const std::vector<std::string> &ids = entry.second;

        // Derive keys using the PiBas method:
        // K1 = F(masterKey, "1" + w)
        // K2 = F(masterKey, "2" + w)
        std::vector<unsigned char> K1 = Encryption::computePRF(masterKey, "1" + w);
        std::vector<unsigned char> K2 = Encryption::computePRF(masterKey, "2" + w);

        int c = 0;
        for (const std::string &id : ids)
        {
            // Compute lookup tag ℓ = F(K1, c)
            std::string lookupTag = toHex(Encryption::computePRF(K1, std::to_string(c)));
            std::cout << "id: " << id << " lookup tag: " << lookupTag << std::endl;

            // Encrypt the document id using K2.
            std::vector<unsigned char> encryptedId = Encryption::encryptAES(K2, id);
            L.push_back({lookupTag, encryptedId});
            c++;
        }
    }

    // Sort L lexicographically by the lookup tag.
    sort(L.begin(), L.end(), [](const auto &a, const auto &b)
         { return a.first < b.first; });

    // Create the encrypted database ED.
    std::map<std::string, std::vector<unsigned char>> ED;
    for (const auto &p : L)
    {
        ED[p.first] = p.second;
    }
    // printPair({masterKey, ED});
    return {masterKey, ED};
}

// Helper: Print the two vectors (K1 and K2).
void printPair2(const std::pair<std::vector<unsigned char>, std::vector<unsigned char>> &p)
{
    std::cout << "K1: ";
    for (const auto &byte : p.first)
    {
        std::cout << static_cast<int>(byte) << " ";
    }
    std::cout << std::endl;

    std::cout << "K2: ";
    for (const auto &byte : p.second)
    {
        std::cout << static_cast<int>(byte) << " ";
    }
    std::cout << std::endl;
}

/*
 * Client (Search) Function:
 * 1. On input (masterKey, w), derive:
 *       K1 = F(masterKey, "1" + w)
 *       K2 = F(masterKey, "2" + w)
 * 2. Return (K1, K2) to be used by the server.
 */
std::pair<std::vector<unsigned char>, std::vector<unsigned char>> Client(const std::vector<unsigned char> masterKey, const std::string &w)
{
    std::vector<unsigned char> K1 = Encryption::computePRF(masterKey, "1" + w);
    std::vector<unsigned char> K2 = Encryption::computePRF(masterKey, "2" + w);
    // printPair2({K1, K2});
    return {K1, K2};
}

/*
 * Server (Search) Function:
 * 1. For each counter c = 0, 1, 2, ...:
 *       Compute tag = F(K1, c)
 *       If tag is found in ED, decrypt the corresponding encrypted id using K2.
 *       Stop when no entry is found.
 */
std::vector<std::string> Server(const std::map<std::string, std::vector<unsigned char>> &ED,
                                const std::vector<unsigned char> &K1,
                                const std::vector<unsigned char> &K2)
{
    std::vector<std::string> output;
    int c = 0;

    while (true)
    {
        std::string tag = toHex(Encryption::computePRF(K1, std::to_string(c)));
        std::cout << "Computed tag: " << tag << std::endl;
        auto it = ED.find(tag);

        if (it == ED.end())
        {
            std::cout << "No entry found for counter c = " << c << ". Ending search." << std::endl;
            break;
        }

        std::vector<unsigned char> encryptedId = it->second;
        std::string id = Encryption::decryptAES(K2, encryptedId);
        std::cout << "Decrypted id: " << id << std::endl;
        output.push_back(id);
        c++;
    }
    return output;
}

// Helper: Print the search parameters.
void printSearchResult(const std::map<std::string, std::vector<unsigned char>> &ED,
                       const std::vector<unsigned char> &masterKey,
                       const std::string &w)
{
    std::cout << "Encrypted Database (ED):" << std::endl;
    for (const auto &pair : ED)
    {
        std::cout << "Lookup Tag: " << pair.first << " -> Encrypted Value: ";
        for (const auto &value : pair.second)
        {
            std::cout << value << " ";
        }
        std::cout << std::endl;
    }

    std::cout << "Master Key:" << std::endl;
    for (const auto &byte : masterKey)
    {
        std::cout << byte << " ";
    }
    std::cout << std::endl;

    std::cout << "Search Keyword: " << w << std::endl;
}

/*
 * Search:
 * Given ED and masterKey, perform the search for keyword w.
 */
std::vector<std::string> Pibas::Search(
    const std::map<std::string, std::vector<unsigned char>> &ED,
    const std::vector<unsigned char> &masterKey,
    const std::string &w)
{
    // printSearchResult(ED, masterKey, w);
    auto keys = Client(masterKey, w);
    std::cout << "Starting server lookup..." << std::endl;
    std::vector<std::string> result = Server(ED, keys.first, keys.second);
    std::cout << "Search complete." << std::endl;
    return result;
}
