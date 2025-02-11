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

vector<unsigned char> deriveKey(const vector<unsigned char>& masterKey, const string& w) {
    // Step 1: Compute PRF(masterKey, w)
    vector<unsigned char> prf_output = computePRF(masterKey, w);

    // Step 2: Split into K1 and K2
    size_t half_size = prf_output.size() / 2;
    vector<unsigned char> K1(prf_output.begin(), prf_output.begin() + half_size);
    vector<unsigned char> K2(prf_output.begin() + half_size, prf_output.end());

    // Step 3: Concatenate K1 and K2 into final key
    vector<unsigned char> derivedKey;
    derivedKey.insert(derivedKey.end(), K1.begin(), K1.end());
    derivedKey.insert(derivedKey.end(), K2.begin(), K2.end());

    return derivedKey;
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
