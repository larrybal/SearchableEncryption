#include <iostream>
#include "encryption.hpp"

std::vector<unsigned char> generateKey(size_t length) {
    std::vector<unsigned char> key(length);
    if (RAND_bytes(key.data(), length) != 1) {
        std::cerr << "Error generating random bytes" << std::endl;
    }

    return key;
}

std::vector<unsigned char> deriveKey(const std::vector<unsigned char>& masterKey, const std::string& w) {
    // Implementation goes here
    
}

std::vector<unsigned char> computePRF(const std::vector<unsigned char>& key, const std::string& data) {
    // Implementation goes here

}

std::vector<unsigned char> encryptAES(const std::vector<unsigned char>& key, const std::string& plaintext) {
    // Implementation goes here
}

std::string decryptAES(const std::vector<unsigned char>& key, const std::vector<unsigned char>& ciphertext) {
    // Implementation goes here
}