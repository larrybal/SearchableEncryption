#ifndef LOGSRC_HPP
#define LOGSRC_HPP

#include <vector>
#include <memory>

/**
 * A record in your dataset:
 *  - 'tid' stores the tuple ID.
 *  - 'value' stores the attributer.
 */
struct Record
{
    std::vector<unsigned char> tid;
    std::vector<unsigned char> value;
};

/**
 * A tree node in the Logarithmic-SRC (wavelet/segment) structure.
 *
 *  - Each node covers an integer range [low, high] in the domain of 'value'.
 *  - 'encryptedTIDs' is a collection of AES-encrypted TIDs 
 *    for records whose 'value' ∈ [low, high].
 *  - leftChild and rightChild partition the domain further if (low < high).
 */
struct TreeNode
{
    int low;  
    int high;

    std::vector<std::vector<unsigned char>> encryptedTIDs;

    std::unique_ptr<TreeNode> leftChild;
    std::unique_ptr<TreeNode> rightChild;

    TreeNode(int l, int h)
        : low(l), high(h),
          leftChild(nullptr), rightChild(nullptr) {}
};

/**
 * The top-level structure holding the root of the Logarithmic-SRC tree.
 */
struct LogSRCIndex
{
    std::unique_ptr<TreeNode> root;
};

/**
 * Build ("Setup") the Logarithmic-SRC index.
 *
 * @param dataset     A vector of (tid, value) records. Both 'tid' and 'value' are byte vectors.
 * @param masterKey   A byte vector (e.g., 32 bytes for AES-256). Used to derive node-specific keys.
 * @param domainLow   Integer specifying the minimum attribute value in the domain.
 * @param domainHigh  Integer specifying the maximum attribute value in the domain.
 *
 * @return A LogSRCIndex containing a pointer to the fully built tree (root node).
 *
 */
LogSRCIndex SetupLogSRC(const std::vector<Record> &dataset,
                        const std::vector<unsigned char> &masterKey,
                        int domainLow,
                        int domainHigh);

/**
 * Range-query ("Search") in the Logarithmic-SRC tree.
 *
 * @param index       The LogSRCIndex from SetupLogSRC().
 * @param masterKey   The master key used in setup.
 * @param L           The lower bound of the query range (integer).
 * @param H           The upper bound of the query range (integer).
 *
 * @return A vector of decrypted TIDs (as integers or strings—depending on implementation).
 *         Typically, you'll parse them to int if your TIDs are stored that way.
 */
std::vector<int> SearchLogSRC(const LogSRCIndex &index,
                              const std::vector<unsigned char> &masterKey,
                              int L,
                              int H);

#endif // LOGSRC_HPP
