#ifndef ENCRYPTION_HPP
#define ENCRYPTION_HPP

#include <openssl/rand.h> // Secure random number generation
#include <openssl/evp.h>  // Encryption functions (AES, HKDF)
#include <openssl/hmac.h> // HMAC for PRF
#include <openssl/aes.h>  // Low-level AES operations
#include <vector>
#include <string>

class Encryption
{
public:
    // Generate a random master key
    static std::vector<unsigned char> generateKey(size_t length);

    // Derive subkeys K1 and K2 from the master key
    static std::pair<std::vector<string>, std::vector<string>>
    deriveKey(const std::vector<unsigned char> &masterKey, std::vector<std::string> w);

    // Compute PRF
    static std::vector<unsigned char> computePRF(const std::vector<unsigned char> &key, const std::string &data);

    // Encrypt using AES-GCM
    static std::vector<unsigned char> encryptAES(const std::vector<unsigned char> &key, const std::string &plaintext);

    // Decrypt using AES-GCM
    static std::string decryptAES(const std::vector<unsigned char> &key, const std::vector<unsigned char> &ciphertext);
};

#endif // ENCRYPTION_HPP
