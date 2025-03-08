#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <sys/resource.h>  // For getrusage (Linux/macOS)
#include <mach/task.h>
#include <mach/mach_init.h>
#include "sha256.h"

// Your previous code for sha256, murmur2, etc. goes here...
int calculateIterations(int size)
{
    return (size / 256.0 + 0.99999999);
}

unsigned int murmur2(const void *key, int len, unsigned int seed)
{
    const unsigned int m = 0x5bd1e995;
    const int r = 24;
    unsigned int h = seed ^ len;
    const unsigned char *data = (const unsigned char *)key;

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
        seed = murmur2(input, strlen(input), seed);
        j = seed % (SHA256_BLOCK_SIZE * 2);  // Select from the 64 hex characters
        des[i++] = src[j];  // Choose a hex character
    }
    des[i] = '\0';  // Null-terminate the result
    strcpy(input, des);  // Update input with the new 16-character string
    return seed;
}

void variableSizeHash(const char *input, int size, double *time_taken_ms)
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

    // Start the time measurement
    clock_t start = clock();

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

    // End the time measurement
    clock_t end = clock();
    *time_taken_ms = ((double)(end - start)) / CLOCKS_PER_SEC * 1000;
}

// Function to get current memory usage (macOS-based, ru_maxrss is in bytes)
// long getMemoryUsage()
// {
//     struct rusage usage;
//     getrusage(RUSAGE_SELF, &usage);
//     return usage.ru_maxrss;  // Return memory usage in bytes (as per macOS)
// }


long getMemoryUsage() {
    task_basic_info_data_t info;
    mach_msg_type_number_t info_count = TASK_BASIC_INFO_COUNT;
    
    if (task_info(mach_task_self(), TASK_BASIC_INFO, (task_info_t)&info, &info_count) == KERN_SUCCESS) {
        printf("Memory usage: %lu KB\n", info.resident_size / 1024);
        return info.resident_size /1024;
    } else {
        return -1;
    }
}


int main()
{
    const char input[] = "Jugya";
    int sizes[] = {128, 256, 384, 512, 640, 768, 896, 1024, 1280, 1536};  // Test sizes
    int num_sizes = sizeof(sizes) / sizeof(sizes[0]);

    // Print table header
    printf("\n---------------------------------------------------------------------\n");
    printf("| %-15s | %-15s | %-20s |\n", "Input Size (bits)", "Time (ms)", "CPU Time (us)");
    printf("---------------------------------------------------------------------\n");

    for (int i = 0; i < num_sizes; i++)
    {
        int size = sizes[i];
        double time_taken_ms;
        
        // Call the hashing function
        variableSizeHash(input, size, &time_taken_ms);

        // Get CPU time used (for macOS/Linux, measured by getrusage)
        struct rusage usage;
        getrusage(RUSAGE_SELF, &usage);
        long cpu_time_us = (usage.ru_utime.tv_sec * 1000000) + usage.ru_utime.tv_usec;  // CPU time in microseconds

        // Print the result in a table format
        printf("| %-15d | %-15.3f | %-20ld |\n", size, time_taken_ms, cpu_time_us);
    }

    printf("---------------------------------------------------------------------\n");

    return 0;
}
