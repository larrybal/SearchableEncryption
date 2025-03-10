#include <openssl/evp.h>
#include <openssl/rand.h>
#include <vector>
#include <iostream>
#include <iomanip>
#include <cstring>

#include "encryption.hpp"

using namespace std;

vector<unsigned char> generateKey(size_t length)
{
    vector<unsigned char> key(length);

    // Needs valid key length for AES
    if (length != 16 && length != 24 && length != 32)
    {
        cerr << "Invalid AES key length. Must be 16, 24, or 32 bytes." << endl;
        throw invalid_argument("Unsupported AES key length.");
    }

    if (RAND_bytes(key.data(), length) != 1)
    {
        cerr << "Error generating random bytes" << endl;
        throw runtime_error("Failed to generate a secure random key.");
    }

    return key;
}

pair<vector<unsigned char>, vector<unsigned char>> deriveKey(const vector<unsigned char> &masterKey, const string &w)
{
    vector<unsigned char> prf_output = Encryption::computePRF(masterKey, w);

    // Ensure the PRF output is at least two parts
    size_t half_size = prf_output.size() / 2;

    // Correctly split PRF output
    vector<unsigned char> K1(prf_output.begin(), prf_output.begin() + half_size);
    vector<unsigned char> K2(prf_output.begin() + half_size, prf_output.end());

    return {K1, K2}; // Return K1 and K2 as a pair
}

vector<unsigned char> computePRF(const vector<unsigned char> &key, const string &data)
{
    vector<unsigned char> result(EVP_MAX_MD_SIZE); // Max possible hash size
    unsigned int result_len = 0;

    // Compute HMAC-SHA256
    HMAC(EVP_sha256(), key.data(), key.size(),
         reinterpret_cast<const unsigned char *>(data.c_str()), data.length(),
         result.data(), &result_len);

    // Resize to hash length
    result.resize(result_len);
    return result;
}

vector<unsigned char> encryptAES(const vector<unsigned char> &key, const string &plaintext)
{
    // Implementation goes here
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx)
    {
        cerr << "Failed to create EVP_CIPHER_CTX" << endl;
        return {};
    }

    vector<unsigned char> iv(AES_BLOCK_SIZE);
    RAND_bytes(iv.data(), AES_BLOCK_SIZE);

    // Initializing AES-256-CBC encryption
    if (EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, key.data(), iv.data()) != 1)
    {
        cerr << "Encryption initialization failed" << endl;
        EVP_CIPHER_CTX_free(ctx);
        return {};
    }

    vector<unsigned char> ciphertext(plaintext.size() + AES_BLOCK_SIZE);
    int len = 0, ciphertext_len = 0;

    // Encrypt the plaintext
    if (EVP_EncryptUpdate(ctx, ciphertext.data(), &len,
                          reinterpret_cast<const unsigned char *>(plaintext.c_str()), plaintext.size()) != 1)
    {
        cerr << "Encryption update failed" << endl;
        EVP_CIPHER_CTX_free(ctx);
        return {};
    }
    ciphertext_len += len;

    // Finalize encryption
    if (EVP_EncryptFinal_ex(ctx, ciphertext.data() + len, &len) != 1)
    {
        cerr << "Encryption finalization failed" << endl;
        EVP_CIPHER_CTX_free(ctx);
        return {};
    }
    ciphertext_len += len;

    EVP_CIPHER_CTX_free(ctx);

    ciphertext.insert(ciphertext.begin(), iv.begin(), iv.end());
    ciphertext.resize(ciphertext_len + AES_BLOCK_SIZE);
    return ciphertext;
}

string decryptAES(const vector<unsigned char> &key, const vector<unsigned char> &ciphertext)
{
    if (ciphertext.size() < AES_BLOCK_SIZE)
    {
        cerr << "Ciphertext too short to contain IV" << endl;
        return {};
    }

    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx)
    {
        cerr << "Failed to create EVP_CIPHER_CTX" << endl;
        return {};
    }

    vector<unsigned char> iv(ciphertext.begin(), ciphertext.begin() + AES_BLOCK_SIZE);
    vector<unsigned char> actual_ciphertext(ciphertext.begin() + AES_BLOCK_SIZE, ciphertext.end());

    // Initialize AES-256-CBC decryption with the extracted IV
    if (EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, key.data(), iv.data()) != 1)
    {
        cerr << "Decryption initialization failed" << endl;
        EVP_CIPHER_CTX_free(ctx);
        return {};
    }

    vector<unsigned char> plaintext(actual_ciphertext.size());
    int len = 0, plaintext_len = 0;

    // Decrypt the ciphertext
    if (EVP_DecryptUpdate(ctx, plaintext.data(), &len, actual_ciphertext.data(), actual_ciphertext.size()) != 1)
    {
        cerr << "Decryption update failed" << endl;
        EVP_CIPHER_CTX_free(ctx);
        return {};
    }
    plaintext_len += len;

    // Finalize decryption (remove PKCS7 padding)
    if (EVP_DecryptFinal_ex(ctx, plaintext.data() + len, &len) != 1)
    {
        cerr << "Decryption finalization failed (wrong key or corrupted data)" << endl;
        EVP_CIPHER_CTX_free(ctx);
        return {};
    }
    plaintext_len += len;

    EVP_CIPHER_CTX_free(ctx);
    plaintext.resize(plaintext_len);
    return string(plaintext.begin(), plaintext.end());
}
