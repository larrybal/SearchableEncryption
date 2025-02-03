#ifndef PIBAS_HPP
#define PIBAS_HPP

#include <unordered_map>
#include <vector>
#include <string>

std::unordered_map<std::string, std::string> Setup(
    const std::unordered_map<std::string, std::vector<std::string>>& D);

std::vector<std::string> Search(
    const std::unordered_map<std::string, std::string>& ED,
    const std::string& K,
    const std::string& w);

#endif // PIBAS_HPP