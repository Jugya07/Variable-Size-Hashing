#include <iostream>
#include <cmath>
#include <fstream>

bool isPrime(int num)
{
    if (num < 2)
        return false;
    if (num == 2)
        return true;
    if (num % 2 == 0)
        return false;

    int limit = std::sqrt(num);
    for (int i = 3; i <= limit; i += 2)
    {
        if (num % i == 0)
            return false;
    }
    return true;
}

int main()
{
    const int PRIME_COUNT = 500;
    int primes[PRIME_COUNT];
    int count = 0;
    int num = 2;

    while (count < PRIME_COUNT)
    {
        if (isPrime(num))
        {
            primes[count] = num;
            count++;
        }
        num++;
    }

    // Write the primes to a text file
    std::ofstream outfile("primes.txt");
    if (!outfile)
    {
        std::cerr << "Error opening file!" << std::endl;
        return 1;
    }

    outfile << "First 500 Prime Numbers:\n";
    for (int i = 0; i < PRIME_COUNT; i++)
    {
        outfile << primes[i];
        if ((i + 1) % 10 == 0)
            outfile << ",\n";
        else
            outfile << ", ";
    }

    outfile.close();
    std::cout << "Primes written to primes.txt successfully." << std::endl;

    return 0;
}
