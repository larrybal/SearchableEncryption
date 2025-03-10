#ifndef PIBAS_HPP
#define PIBAS_HPP

#include <map>
#include <vector>
#include <string>

class Pibas {
public:
    static std::map<std::string, std::vector<std::string>> Setup(
        const std::map<std::string, std::vector<std::string>>& D);

    static std::vector<std::string> Search(
        const std::map<std::string, std::vector<std::string>>& ED,
        const std::string& K,
        const std::string& w);
};

#endif // PIBAS_HPP