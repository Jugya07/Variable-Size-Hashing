#include <iostream>
#include <vector>
#include <string>
#include <cstdint>
#include <cstdio>
#include "MurmurHash3.h"
#include "seeds.h"
#define getBit(n, p) (((n) >> (p)) & 1)

using namespace std;

// Function to convert bit array to hexadecimal string
string bits_to_hex(const vector<uint8_t> &bits)
{
    string hex;
    for (size_t i = 0; i < bits.size(); i += 4)
    {
        uint8_t nibble = 0;
        for (size_t j = 0; j < 4 && i + j < bits.size(); ++j)
        {
            nibble |= (bits[i + j] & 1) << (3 - j);
        }
        char buf[2];
        snprintf(buf, sizeof(buf), "%x", nibble);
        hex += buf;
    }
    return hex;
}

uint64_t getHashValue(uint32_t seed, const string &input)
{
    uint64_t hash[2]; // 128-bit hash (two 64-bit values)
    MurmurHash3_x64_128(input.c_str(), input.length(), seed, hash);
    return hash[0];
}

// Cryptographic hash function
string cryptographic_hash(const string &input, int output_size_bits)
{
    // Create finalhash array for bits
    vector<uint8_t> finalhash(output_size_bits);

    // Get the fixed seeds
    const uint32_t *seeds = get_seeds();

    // Compute Murmur3 128-bit hashes and extract 32nd bit
    for (int i = 0; i < output_size_bits; ++i)
    {
        uint32_t seed = seeds[i % 1024];

        uint64_t target_word = getHashValue(seed, input);

        finalhash[i] = getBit(target_word, 63 - 31); // Extract bit 31 of hash[0]
    }

    // Convert finalhash to hexadecimal
    return bits_to_hex(finalhash);
}

int main()
{
    // Get user input
    string input;
    cout << "Enter the string to hash: ";
    getline(cin, input);

    int output_size_bits;
    cout << "Enter the output size in bits: ";
    cin >> output_size_bits;

    // Validate input
    if (output_size_bits <= 0)
    {
        cout << "Output size must be positive." << endl;
        return 1;
    }

    // Compute and display hash
    string hash = cryptographic_hash(input, output_size_bits);
    cout << "Hash: " << hash << endl;

    return 0;
}