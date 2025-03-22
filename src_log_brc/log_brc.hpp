#ifndef LOG_BRC_HPP
#define LOG_BRC_HPP

#include <iostream>
#include <vector>
#include <map>
#include <set>

class SSE
{
public:
    std::map<std::string, std::vector<int>> index;

    void buildIndex(const std::map<std::string, std::vector<int>> &data);
    std::vector<int> search(const std::string &token);
};

struct Node
{
    int start;
    int end;
    std::string keyword;
    Node *left;
    Node *right;

    Node(int start, int end, const std::string &keyword);
};

class LogBRC
{
public:
    int domainSize;
    Node *root;
    SSE sse;

    LogBRC(int domainSize);

    Node *buildTree(int start, int end, int level);
    std::vector<std::string> findNodes(Node *node, int value);
    std::vector<std::string> findRangeCover(Node *node, int start, int end);
    void buildIndex(const std::map<int, std::vector<int>> &data);
    std::vector<std::string> trpdr(int start, int end);
    std::set<int> search(const std::vector<std::string> &tokens);
};

#endif // LOG_BRC_HPP
