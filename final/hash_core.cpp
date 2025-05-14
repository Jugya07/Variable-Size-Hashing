// #include "hash_core.h"
// #include "seeds.h"
// #include <openssl/sha.h>
// #include <iomanip>
// #include <stdexcept>
// #include <chrono>

// #define getBit(n, p) (((n) >> (p)) & 1)

// using namespace std;

// // Convert bit array to hexadecimal string
// string bits_to_hex(const vector<uint8_t>& bits) {
//     string hex;
//     hex.reserve((bits.size() + 3) / 4);
//     for (size_t i = 0; i < bits.size(); i += 4) {
//         uint8_t nibble = 0;
//         for (size_t j = 0; j < 4 && i + j < bits.size(); ++j) {
//             nibble |= bits[i + j] << (3 - j);
//         }
//         char buf[2];
//         snprintf(buf, sizeof(buf), "%x", nibble);
//         hex += buf;
//     }
//     return hex;
// }

// // Compute SHA-256 hash of input + seed + input length
// vector<uint8_t> getHashValue(uint32_t seed, const string& input) {
//     string seeded_input = input + to_string(seed) + to_string(input.length());
//     unsigned char hash[SHA256_DIGEST_LENGTH];
//     SHA256((unsigned char*)seeded_input.c_str(), seeded_input.length(), hash);
//     vector<uint8_t> result(hash, hash + SHA256_DIGEST_LENGTH);
//     return result;
// }

// // Cryptographic hash function
// string cryptographic_hash(const string& input, int output_size_bits) {
//     if (output_size_bits <= 0) {
//         throw runtime_error("Output size must be positive");
//     }
//     if (output_size_bits > 1000000) {
//         throw runtime_error("Output size too large");
//     }
//     vector<uint8_t> finalhash(output_size_bits);
//     const uint32_t* seeds = get_seeds();
//     for (int i = 0; i < output_size_bits; i += 1) {
//         int seed_idx = i % 1024;
//         vector<uint8_t> hash = getHashValue(seeds[seed_idx], input);
//         for (int j = 0; j < 8 && i + j < output_size_bits; ++j) {
//             finalhash[i + j] = getBit(hash[0], 7 - j);
//         }
//     }
//     return bits_to_hex(finalhash);
// }

// // Run tests (avalanche and speed)
// string run_tests() {
//     string output;

//     // Avalanche test
//     string input = "test";
//     string input2 = "tesu";
//     int bits = 64;
//     string h1 = cryptographic_hash(input, bits);
//     string h2 = cryptographic_hash(input2, bits);
//     int diff = 0;
//     for (size_t i = 0; i < h1.size() && i < h2.size(); ++i) {
//         if (h1[i] != h2[i]) diff += 4; // Approx 4 bits per hex char
//     }
//     output += "Avalanche Test:\nInput 1: " + input + "\nHash 1: " + h1 +
//               "\nInput 2: " + input2 + "\nHash 2: " + h2 +
//               "\nBit differences: " + to_string(diff) + " (~" +
//               to_string((double)diff / bits * 100) + "%)\n\n";

//     // Speed test
//     auto start = chrono::high_resolution_clock::now();
//     int trials = 1000;
//     for (int i = 0; i < trials; ++i) {
//         cryptographic_hash(input + to_string(i), bits);
//     }
//     auto end = chrono::high_resolution_clock::now();
//     auto duration = chrono::duration_cast<chrono::milliseconds>(end - start).count();
//     output += "Speed Test:\nTime for " + to_string(trials) +
//               " hashes (64 bits): " + to_string(duration) + " ms\n";

//     return output;
// }

#include "hash_core.h"
#include "seeds.h"
#include <openssl/sha.h>
#include <iomanip>
#include <stdexcept>
#include <chrono>
#include <vector>

#define getBit(n, p) (((n) >> (p)) & 1)

using namespace std;

// Memory block size (1 MB)
const size_t MEMORY_SIZE = 1024 * 1024; // 1 MB in bytes

// Convert bit array to hexadecimal string
string bits_to_hex(const vector<uint8_t> &bits)
{
    string hex;
    hex.reserve((bits.size() + 3) / 4);
    for (size_t i = 0; i < bits.size(); i += 4)
    {
        uint8_t nibble = 0;
        for (size_t j = 0; j < 4 && i + j < bits.size(); ++j)
        {
            nibble |= bits[i + j] << (3 - j);
        }
        char buf[2];
        snprintf(buf, sizeof(buf), "%x", nibble);
        hex += buf;
    }
    return hex;
}

// Compute SHA-256 hash of input + seed + input length
vector<uint8_t> getHashValue(uint32_t seed, const string &input)
{
    string seeded_input = input + to_string(seed) + to_string(input.length());
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256((unsigned char *)seeded_input.c_str(), seeded_input.length(), hash);
    vector<uint8_t> result(hash, hash + SHA256_DIGEST_LENGTH);
    return result;
}

// Fill memory array with pseudo-random data
void fill_memory(vector<uint8_t> &memory, const string &input, const uint32_t *seeds)
{
    for (size_t i = 0; i < MEMORY_SIZE; i += SHA256_DIGEST_LENGTH)
    {
        string seed_input = input + to_string(seeds[i % 1024]) + to_string(i);
        unsigned char hash[SHA256_DIGEST_LENGTH];
        SHA256((unsigned char *)seed_input.c_str(), seed_input.length(), hash);
        for (size_t j = 0; j < SHA256_DIGEST_LENGTH && i + j < MEMORY_SIZE; ++j)
        {
            memory[i + j] = hash[j];
        }
    }
}

// Cryptographic hash function with memory-hardness
string cryptographic_hash(const string &input, int output_size_bits)
{
    if (output_size_bits <= 0)
    {
        throw runtime_error("Output size must be positive");
    }
    if (output_size_bits > 1000000)
    {
        throw runtime_error("Output size too large");
    }

    // Initialize memory array (1 MB)
    vector<uint8_t> memory(MEMORY_SIZE);
    const uint32_t *seeds = get_seeds();
    fill_memory(memory, input, seeds);

    // Generate hash
    vector<uint8_t> finalhash(output_size_bits);
    for (int i = 0; i < output_size_bits; i += 8)
    {
        int seed_idx = (i / 8) % 1024;
        vector<uint8_t> hash = getHashValue(seeds[seed_idx], input);

        // Use hash to index memory (memory-hard step)
        uint32_t index = (hash[0] << 24) | (hash[1] << 16) | (hash[2] << 8) | hash[3];
        index %= MEMORY_SIZE; // Ensure index is within bounds
        uint8_t memory_byte = memory[index];

        // Mix memory byte into hash
        hash[0] ^= memory_byte;

        for (int j = 0; j < 8 && i + j < output_size_bits; ++j)
        {
            finalhash[i + j] = getBit(hash[0], 7 - j);
        }
    }
    return bits_to_hex(finalhash);
}

// Run tests (avalanche and speed)
string run_tests()
{
    string output;

    // Avalanche test
    string input = "test";
    string input2 = "tesu";
    int bits = 64;
    string h1 = cryptographic_hash(input, bits);
    string h2 = cryptographic_hash(input2, bits);
    int diff = 0;
    for (size_t i = 0; i < h1.size() && i < h2.size(); ++i)
    {
        if (h1[i] != h2[i])
            diff += 4; // Approx 4 bits per hex char
    }
    output += "Avalanche Test:\nInput 1: " + input + "\nHash 1: " + h1 +
              "\nInput 2: " + input2 + "\nHash 2: " + h2 +
              "\nBit differences: " + to_string(diff) + " (~" +
              to_string((double)diff / bits * 100) + "%)\n\n";

    // Speed test
    auto start = chrono::high_resolution_clock::now();
    int trials = 1000;
    for (int i = 0; i < trials; ++i)
    {
        cryptographic_hash(input + to_string(i), bits);
    }
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start).count();
    output += "Speed Test:\nTime for " + to_string(trials) +
              " hashes (64 bits): " + to_string(duration) + " ms\n";

    return output;
}