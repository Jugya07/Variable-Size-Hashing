#include <stdio.h>
#include <string.h>
#include <time.h>
#include "sha256.h"

#define M 0x5bd1e995
#define N 0x71b18589

int calculateIterations(int size)
{
    return (size / 256.0 + 0.99999999);
}

unsigned int prng(const char str[], int len, unsigned int seed1) 
{
    unsigned int seed2 = 0x91259ab, seed3 = 0, x = 0, y = 1, z = 0;

    // Loop through the string byte-by-byte (avoid casting to unsigned int)
    for (int i = 0; i < len; i++) 
    {
        unsigned int key = (unsigned int)str[i];  // Take each character as key

        seed3 += (seed1 ^ seed2);
        seed3 ^= key;

        x = x ^ M;
        x += (seed3 << 16) + (seed3 << 8) + seed3;

        y += (x >> 13) + (x >> 15) + x;
        y = y * N;

        z ^= (x + y);

        seed3 = z;
        seed1 = seed2;
        seed2 = seed3;
    }

    return seed3;
}

// Convert hash (binary) to hexadecimal string
void hash_to_hex(const unsigned char *hash, char *hex)
{
    for (int i = 0; i < SHA256_BLOCK_SIZE; i++)
    {
        sprintf(hex + (i * 2), "%02x", hash[i]);  // Each byte becomes 2 hex characters
    }
    hex[SHA256_BLOCK_SIZE * 2] = '\0';  // Null-terminate the hex string
}

// Select 16 hex characters from the hash
unsigned int select_16_hex_chars(char input[], char des[], char src[], unsigned int seed)
{
    int i = 0, j = 0;
    while (i < 16)
    {
        seed = prng(input, strlen(input), seed);
        j = seed % (SHA256_BLOCK_SIZE * 2);  // Select from the 64 hex characters
        des[i++] = src[j];  // Choose a hex character
    }
    des[i] = '\0';  // Null-terminate the result
    strcpy(input, des);  // Update input with the new 16-character string
    return seed;
}

// New function to compute the variable size hash
void variableSizeHash(const char *input, int size, char *final_hash)
{
    SHA256_CTX ctx;
    BYTE hash[SHA256_BLOCK_SIZE];  // Binary hash output
    char hex_hash[SHA256_BLOCK_SIZE * 2 + 1];  // Hexadecimal representation of the hash
    char tmp[1024];  // Buffer for initial concatenation and hex string of 16 chars
    char buff[1024];  // Buffer for selected 16 hex chars

    sprintf(tmp, "%s+%d", input, size);  // Initial input

    int k = calculateIterations(size);  // Number of iterations
    BYTE hash_store[k][SHA256_BLOCK_SIZE];  // To store the hashes

    unsigned int seed = 0xab915;

    for (int i = 0; i < k; i++)
    {
        // Initialize SHA-256 context inside the loop for each iteration
        sha256_init(&ctx);

        // Update the hash with the current input
        sha256_update(&ctx, tmp, strlen(tmp));

        // Finalize the hash computation
        sha256_final(&ctx, hash);

        // Convert the binary hash to a hexadecimal string
        hash_to_hex(hash, hex_hash);

        // Select 16 hex characters from the hexadecimal hash for the next iteration
        seed = select_16_hex_chars(tmp, buff, hex_hash, seed);

        // Store the binary hash in hash_store
        memcpy(hash_store[i], hash, SHA256_BLOCK_SIZE);

        // Copy buff (16 hex chars) to tmp for the next iteration
        strcpy(tmp, buff);
    }

    // Now extract 'size / 4' characters from hash_store
    int total_chars_needed = size / 4;  // The number of hex chars we need
    int collected_chars = 0;

    // Traverse through hash_store to collect characters
    for (int i = 0; i < k && collected_chars < total_chars_needed; i++)
    {
        char hex[SHA256_BLOCK_SIZE * 2 + 1];
        hash_to_hex(hash_store[i], hex);  // Convert stored binary hash to hex

        // Determine how many characters to copy from this hash
        int chars_to_copy = SHA256_BLOCK_SIZE * 2;
        if (collected_chars + chars_to_copy > total_chars_needed)
        {
            chars_to_copy = total_chars_needed - collected_chars;
        }

        // Copy the necessary number of hex characters
        strncpy(final_hash + collected_chars, hex, chars_to_copy);
        collected_chars += chars_to_copy;
    }

    // Null-terminate the final hash
    final_hash[total_chars_needed] = '\0';
}

int main()
{
    const char input[] = "Jugya";
    int size = 720;  // Desired hash size in bits (multiple of 4)

    // Buffer to store the final hash output
    char final_hash[size / 4 + 1];

    // Start the time measurement
    clock_t start = clock();

    // Call the variableSizeHash function
    variableSizeHash(input, size, final_hash);

    // End the time measurement
    clock_t end = clock();

    // Calculate the time taken in milliseconds
    double time_taken_ms = ((double)(end - start)) / CLOCKS_PER_SEC * 1000;
    printf("Time taken by the algorithm: %.3f milliseconds\n", time_taken_ms);

    // Print the final hash
    printf("Final Hash of size %d bits: %s\n", size, final_hash);

    return 0;
}
