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
 * Helper: Manually expand the PRF output.
 * This function calls computePRF twice with diversifiers "0" and "1",
 * concatenates the two outputs, and returns the result.
 */
std::vector<unsigned char> computePRFExpanded(const std::vector<unsigned char> &masterKey, const std::string &w)
{
    // Call computePRF with two different diversifiers
    std::vector<unsigned char> part1 = Encryption::computePRF(masterKey, w + "0");
    std::vector<unsigned char> part2 = Encryption::computePRF(masterKey, w + "1");

    std::vector<unsigned char> combined;
    combined.insert(combined.end(), part1.begin(), part1.end());
    combined.insert(combined.end(), part2.begin(), part2.end());
    return combined;
}

/*
 * PiBas Setup (with manual PRF expansion):
 * 1. Generate a master key K.
 * 2. For each keyword w in the dataset D:
 *       a. Compute combined = computePRFExpanded(K, w) yielding 2*KEYLEN bytes.
 *       b. Split combined into:
 *             K1 = first KEYLEN bytes,
 *             K2 = last KEYLEN bytes.
 *       c. For each id in id(w):
 *              i.   ℓ = F(K1, c)   [c is a counter]
 *              ii.  d = Enc(K2, id)
 *              iii. Add (ℓ, d) to list L.
 * 3. Sort L lexicographically by ℓ and create the encrypted database ED.
 */
std::pair<std::vector<unsigned char>, std::map<std::string, std::vector<unsigned char>>> Pibas::Setup(
    const std::map<std::string, std::vector<std::string>> &D)
{
    // Generate the master key.
    std::vector<unsigned char> masterKey = Encryption::generateKey(KEYLEN);

    // List to store pairs (lookup tag, encrypted id).
    std::vector<std::pair<std::string, std::vector<unsigned char>>> L;

    // For each keyword in the dataset.
    for (const auto &entry : D)
    {
        const std::string &w = entry.first;
        const std::vector<std::string> &ids = entry.second;

        // Manually expand the PRF: combined will be 2*KEYLEN bytes.
        std::vector<unsigned char> combined = computePRFExpanded(masterKey, w);
        // Split into K1 and K2.
        std::vector<unsigned char> K1(combined.begin(), combined.begin() + KEYLEN);
        std::vector<unsigned char> K2(combined.begin() + KEYLEN, combined.end());

        int c = 0;
        for (const std::string &id : ids)
        {
            // Compute lookup tag ℓ = F(K1, c)
            std::string lookupTag = toHex(Encryption::computePRF(K1, std::to_string(c)));
            // std::cout << "id: " << id << " lookup tag: " << lookupTag << std::endl;

            // Encrypt the document id using K2.
            std::vector<unsigned char> encryptedId = Encryption::encryptAES(K2, id);
            L.push_back({lookupTag, encryptedId});
            c++;
        }
    }

    // Sort L lexicographically by lookup tag.
    sort(L.begin(), L.end(), [](const auto &a, const auto &b)
         { return a.first < b.first; });

    // Create the encrypted database ED.
    std::map<std::string, std::vector<unsigned char>> ED;
    for (const auto &p : L)
    {
        ED[p.first] = p.second;
    }
    return {masterKey, ED};
}

/*
 * Client (Search) Function:
 * Given (masterKey, w), derive:
 *   a. Compute combined = computePRFExpanded(masterKey, w)
 *   b. Split combined into:
 *          K1 = first KEYLEN bytes,
 *          K2 = last KEYLEN bytes.
 * Return (K1, K2) for use by the server.
 */
std::pair<std::vector<unsigned char>, std::vector<unsigned char>> Client(
    const std::vector<unsigned char> &masterKey, const std::string &w)
{
    std::vector<unsigned char> combined = computePRFExpanded(masterKey, w);
    std::vector<unsigned char> K1(combined.begin(), combined.begin() + KEYLEN);
    std::vector<unsigned char> K2(combined.begin() + KEYLEN, combined.end());
    return {K1, K2};
}

/*
 * Server (Search) Function:
 * For each counter c = 0, 1, 2, ...:
 *   a. Compute tag = F(K1, c)
 *   b. If tag is found in ED, decrypt the corresponding encrypted id using K2.
 *   c. Stop when no entry is found.
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
        // std::cout << "Computed tag: " << tag << std::endl;
        auto it = ED.find(tag);

        if (it == ED.end())
        {
            // std::cout << "No entry found for counter c = " << c << ". Ending search." << std::endl;
            break;
        }

        std::vector<unsigned char> encryptedId = it->second;
        std::string id = Encryption::decryptAES(K2, encryptedId);
        // std::cout << "Decrypted id: " << id << std::endl;
        output.push_back(id);
        c++;
    }
    return output;
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
    auto keys = Client(masterKey, w);
    // std::cout << "Starting server lookup..." << std::endl;
    std::vector<std::string> result = Server(ED, keys.first, keys.second);
    // std::cout << "Search complete." << std::endl;
    return result;
}
