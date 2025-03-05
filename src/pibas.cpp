#include <iostream>
#include <list>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "pibas.hpp"
#include "encryption.hpp"
#include <unistd.h>

#define KEYLEN 100
using namespace std;

// Setup function: creates an encrypted index
std::unordered_map<std::string, std::string> Setup(
    const std::unordered_map<std::string, std::vector<std::string>> &D)
{

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