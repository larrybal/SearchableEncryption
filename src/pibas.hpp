#ifndef PIBAS_HPP
#define PIBAS_HPP

#include <map>
#include <vector>
#include <string>
using namespace std;
class Pibas
{
public:
    static std::pair<vector<unsigned char>, map<string, vector<unsigned char>>> Setup(
        const std::map<std::string, std::vector<std::string>> &D);

    static std::vector<std::string> Search(
        const std::map<std::string, std::vector<unsigned char>> &ED,
        const vector<unsigned char> &K,
        const std::string &w);
};

#endif // PIBAS_HPP