#ifndef HASH_CORE_H
#define HASH_CORE_H

#include <string>
#include <vector>
using namespace std;

string cryptographic_hash(const string &input, int output_size_bits, int cost = 24);
string run_tests();

#endif // HASH_CORE_H