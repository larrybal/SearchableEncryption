#include <iostream>
#include <vector>
#include <string>
#include <openssl/rand.h>

#include "encryption.hpp"

using namespace std;

vector<unsigned char> generateKey(size_t length) {
    vector<unsigned char> key(length);
    if (RAND_bytes(key.data(), length) != 1) {
        cerr << "Error generating random bytes" << endl;
        throw runtime_error("Failed to generate a secure random key.");
    }

    return key;
}

pair<vector<unsigned char>, vector<unsigned char>> deriveKey(const vector<unsigned char>& masterKey, const string& w) {
    vector<unsigned char> prf_output = computePRF(masterKey, w);

    // Ensure the PRF output is at least two parts
    size_t half_size = prf_output.size() / 2;

    // Correctly split PRF output
    vector<unsigned char> K1(prf_output.begin(), prf_output.begin() + half_size);
    vector<unsigned char> K2(prf_output.begin() + half_size, prf_output.end());

    return {K1, K2};  // Return K1 and K2 as a pair
}

vector<unsigned char> computePRF(const vector<unsigned char>& key, const string& data) {
    vector<unsigned char> result(EVP_MAX_MD_SIZE);  // Max possible hash size
    unsigned int result_len = 0;

    // Compute HMAC-SHA256
    HMAC(EVP_sha256(), key.data(), key.size(),
         reinterpret_cast<const unsigned char*>(data.c_str()), data.length(),
         result.data(), &result_len);

    // Resize to hash length
    result.resize(result_len);
    return result;
}

vector<unsigned char> encryptAES(const vector<unsigned char>& key, const string& plaintext) {
    // Implementation goes here

}

string decryptAES(const vector<unsigned char>& key, const vector<unsigned char>& ciphertext) {
    // Implementation goes here

}
