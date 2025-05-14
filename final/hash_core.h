#ifndef HASH_CORE_H
#define HASH_CORE_H

#include <string>
#include <vector>
#include <cstdint>

std::string cryptographic_hash(const std::string &input, int output_size_bits);
std::string run_tests();

#endif // HASH_CORE_H