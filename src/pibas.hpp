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

    static std::tuple<std::vector<std::string>, int, double> Search(
        const std::map<std::string, std::vector<unsigned char>> &ED,
        const std::vector<unsigned char> &masterKey,
        const std::string &w);
};

#endif // PIBAS_HPP