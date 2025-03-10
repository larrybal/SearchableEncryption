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
std::pair<vector<unsigned char>, map<string, vector<unsigned char>>> Setup(
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

        auto keys = Encryption::deriveKey(K, vector<string>{w});

        vector<unsigned char> K1 = keys.first;
        vector<unsigned char> K2 = keys.second;

        int c = 0;

        for (const string &id : ids)
        {
            string lookupKey = toHex(Encryption::computePRF(K1, to_string(c)));
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

    return {K, ED};
}

std::pair<vector<unsigned char>, vector<unsigned char>> Client(const vector<unsigned char> K, const std::string &w)
{
    vector<unsigned char> K1 = Encryption::computePRF(K, "1" + w);
    vector<unsigned char> K2 = Encryption::computePRF(K, "2" + w);

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
        vector<unsigned char> tag = Encryption::computePRF(K1, std::to_string(c));
        std::string tag_s(tag.begin(), tag.end());
        auto it = ED.find(tag_s);

        if (it == ED.end())
        {
            break;
        }

        vector<unsigned char> d = it->second;

        std::string id = Encryption::decryptAES(K2, d);
        output.push_back(id);

        c++;
    }

    return output;
}

// Search function: retrieves document IDs for a given keyword
std::vector<std::string> Search(
    const map<string, vector<unsigned char>> &ED,
    const vector<unsigned char> &K,
    const string &w)
{
    auto keys = Client(K, w);
    return Server(ED, keys.first, keys.second);
}