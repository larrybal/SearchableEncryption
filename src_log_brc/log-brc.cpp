#include "log-brc.hpp"
#include <algorithm>
#include <random>

void SSE::buildIndex(const std::map<std::string, std::vector<int>> &data)
{
    index = data;
}

std::vector<int> SSE::search(const std::string &token)
{
    if (index.find(token) != index.end())
    {
        return index[token];
    }
    else
    {
        return {};
    }
}

Node::Node(int start, int end, const std::string &keyword) : start(start), end(end), keyword(keyword), left(nullptr), right(nullptr) {}

LogBRC::LogBRC(int domainSize) : domainSize(domainSize), root(nullptr) {}

Node *LogBRC::buildTree(int start, int end, int level)
{
    if (start == end)
    {
        return new Node(start, end, "N" + std::to_string(level) + "," + std::to_string(start));
    }

    int mid = start + (end - start) / 2;
    Node *node = new Node(start, end, "N" + std::to_string(level) + "," + std::to_string(start) + "," + std::to_string(end));
    node->left = buildTree(start, mid, level + 1);
    node->right = buildTree(mid + 1, end, level + 1);
    return node;
}

std::vector<std::string> LogBRC::findNodes(Node *node, int value)
{
    std::vector<std::string> result;
    if (node == nullptr)
    {
        return result;
    }
    if (node->start == node->end && node->start == value)
    {
        result.push_back(node->keyword);
        return result;
    }
    if (value >= node->start && value <= node->end)
    {
        result.push_back(node->keyword);
        std::vector<std::string> leftResult = findNodes(node->left, value);
        result.insert(result.end(), leftResult.begin(), leftResult.end());
        std::vector<std::string> rightResult = findNodes(node->right, value);
        result.insert(result.end(), rightResult.begin(), rightResult.end());
    }
    return result;
}

std::vector<std::string> LogBRC::findRangeCover(Node *node, int start, int end)
{
    std::vector<std::string> result;
    if (node == nullptr || end < node->start || start > node->end)
    {
        return result;
    }
    if (start <= node->start && end >= node->end)
    {
        result.push_back(node->keyword);
        return result;
    }
    std::vector<std::string> leftResult = findRangeCover(node->left, start, end);
    result.insert(result.end(), leftResult.begin(), leftResult.end());
    std::vector<std::string> rightResult = findRangeCover(node->right, start, end);
    result.insert(result.end(), rightResult.begin(), rightResult.end());
    return result;
}

void LogBRC::buildIndex(const std::map<int, std::vector<int>> &data)
{
    root = buildTree(0, domainSize - 1, 0);
    std::map<std::string, std::vector<int>> replicatedData;

    for (const auto &pair : data)
    {
        int value = pair.first;
        const std::vector<int> &ids = pair.second;
        std::vector<std::string> keywords = findNodes(root, value);

        for (const std::string &keyword : keywords)
        {
            replicatedData[keyword].insert(replicatedData[keyword].end(), ids.begin(), ids.end());
        }
    }

    sse.buildIndex(replicatedData);
}

std::vector<std::string> LogBRC::trpdr(int start, int end)
{
    std::vector<std::string> keywords = findRangeCover(root, start, end);
    std::vector<std::string> tokens = keywords;

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(tokens.begin(), tokens.end(), g);

    return tokens;
}

std::set<int> LogBRC::search(const std::vector<std::string> &tokens)
{
    std::set<int> result;
    for (const std::string &token : tokens)
    {
        std::vector<int> ids = sse.search(token);
        result.insert(ids.begin(), ids.end());
    }
    return result;
}
