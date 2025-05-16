#include "hash_core.h"
#include "seeds.h"
#include "primes.h"
#include <string>
#include <vector>
#include <cmath>
#include <chrono>

#define getBit(n, p) (((n) >> (p)) & 1)

using namespace std;

// MurmurHash2
unsigned int murmur2(const string &key, unsigned int seed)
{
    const unsigned int m = 0x5bd1e995;
    const int r = 24;
    unsigned int h = seed ^ key.length();
    const unsigned char *data = (const unsigned char *)key.c_str();
    int len = key.length();

    while (len >= 4)
    {
        unsigned int k = *(unsigned int *)data;
        k *= m;
        k ^= k >> r;
        k *= m;
        h *= m;
        h ^= k;
        data += 4;
        len -= 4;
    }

    switch (len)
    {
    case 3:
        h ^= data[2] << 16;
    case 2:
        h ^= data[1] << 8;
    case 1:
        h ^= data[0];
        h *= m;
    }

    h ^= h >> 13;
    h *= m;
    h ^= h >> 15;
    return h;
}

// Convert bits to hexadecimal
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

// Fill matrix with bits
void fill_matrix(int m, int n, vector<vector<uint8_t>> &mat, const string &str)
{
    int k = 0;
    int total_cells = m * n;
    for (int i = 0; i < m; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            unsigned int seed;
            if (k < 1024 && total_cells <= 1024)
            {
                seed = get_seed(k); // Use fixed seeds for small matrices
            }
            else
            {
                // Generate dynamic seed using murmur2
                string seed_input = str + to_string(k);
                seed = murmur2(seed_input, 0x5bd1e995);
            }
            unsigned int value = murmur2(str, seed);
            mat[i][j] = getBit(value, value % 32);
            k++;
        }
    }
}

// Select bits from matrix
void select_bits(int m, int n, const vector<vector<uint8_t>> &mat, vector<uint8_t> &bits, int size, const string &str)
{
    unsigned int value = 397997;
    for (int i = 0; i < size; ++i)
    {
        value = murmur2(str, get_seed(i) + i + 1);
        int p = value % m;
        int q = value % n;
        bits[i] = mat[p][q];
    }
}

// Cryptographic hash function
string cryptographic_hash(const string &input, int output_size_bits, int cost)
{
    if (output_size_bits <= 0 || output_size_bits % 4 != 0)
    {
        throw runtime_error("Output size must be positive and a multiple of 4");
    }
    if (output_size_bits > 1000000)
    {
        throw runtime_error("Output size too large");
    }
    if (cost < 1)
    {
        throw runtime_error("Cost must be positive");
    }

    string mystr = input + to_string(output_size_bits);
    int s = sqrt(cost);
    int m = get_prime(s + 1);
    int n = get_prime(s + 4);

    vector<vector<uint8_t>> mat(m, vector<uint8_t>(n));
    fill_matrix(m, n, mat, mystr);

    vector<uint8_t> bits(output_size_bits);
    select_bits(m, n, mat, bits, output_size_bits, mystr);

    return bits_to_hex(bits);
}

// Run tests (avalanche and speed)
string run_tests()
{
    string output;

    // Avalanche test
    string input = "test";
    string input2 = "tesu";
    int bits = 64;
    int cost = 24;
    string h1 = cryptographic_hash(input, bits, cost);
    string h2 = cryptographic_hash(input2, bits, cost);
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
        cryptographic_hash(input + to_string(i), bits, cost);
    }
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start).count();
    output += "Speed Test:\nTime for " + to_string(trials) +
              " hashes (64 bits, cost=24): " + to_string(duration) + " ms\n";

    // High-cost test
    cost = 1000;
    start = chrono::high_resolution_clock::now();
    for (int i = 0; i < trials; ++i)
    {
        cryptographic_hash(input + to_string(i), bits, cost);
    }
    end = chrono::high_resolution_clock::now();
    duration = chrono::duration_cast<chrono::milliseconds>(end - start).count();
    output += "Speed Test (High Cost):\nTime for " + to_string(trials) +
              " hashes (64 bits, cost=1000): " + to_string(duration) + " ms\n";

    return output;
}