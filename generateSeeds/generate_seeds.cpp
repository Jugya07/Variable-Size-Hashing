#include <iostream>
#include <random>
#include <iomanip>
#include <set>

using namespace std;

// Function to generate 1024 unique random uint32_t seeds
void generate_seeds()
{
    mt19937 rng(42); // Fixed seed for reproducibility
    uniform_int_distribution<uint32_t> dist(0, UINT32_MAX);
    set<uint32_t> unique_seeds;

    // Generate unique seeds
    while (unique_seeds.size() < 1024)
    {
        unique_seeds.insert(dist(rng));
    }

    // Output as C++ array initializer
    cout << "const uint32_t seeds[1024] = {\n    ";
    int count = 0;
    for (auto seed : unique_seeds)
    {
        cout << "0x" << hex << setfill('0') << setw(8) << seed;
        if (count < 1023)
        {
            cout << ",";
        }
        if (++count % 8 == 0 && count < 1024)
        {
            cout << "\n    ";
        }
        else
        {
            cout << " ";
        }
    }
    cout << "\n};" << endl;
}

int main()
{
    generate_seeds();
    return 0;
}