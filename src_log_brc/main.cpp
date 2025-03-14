#include "log-brc.hpp"
#include <iostream>

int main()
{
    int domainSize = 8;
    LogBRC logBRC(domainSize);

    std::map<int, std::vector<int>> data = {
        {3, {1, 2}},
        {5, {3}},
        {7, {4, 5}}};

    logBRC.buildIndex(data);

    int queryStart = 5;
    int queryEnd = 7;
    std::vector<std::string> tokens = logBRC.trpdr(queryStart, queryEnd);
    std::set<int> result = logBRC.search(tokens);

    std::cout << "Query Range: [" << queryStart << ", " << queryEnd << "]" << std::endl;
    std::cout << "Result: ";
    for (int id : result)
    {
        std::cout << id << " ";
    }
    std::cout << std::endl;

    return 0;
}