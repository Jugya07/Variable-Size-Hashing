#include "hash_core.h"
#include <iostream>
#include <string>
#include <stdexcept>
#include <chrono>

using namespace std;

int main()
{
    while (true)
    {
        cout << "\nHash Function CLI\n";
        cout << "1. Compute Hash\n";
        cout << "2. Run Tests\n";
        cout << "3. Exit\n";
        cout << "Enter choice: ";
        int choice;
        cin >> choice;

        switch (choice)
        {
        case 1:
        {
            cout << "Enter input string: ";
            string input;
            cin.ignore();
            getline(cin, input);
            cout << "Enter output size (bits, multiple of 4): ";
            int size;
            cin >> size;
            cout << "Enter cost (1–100000): ";
            int cost;
            cin >> cost;
            try
            {
                auto start = chrono::high_resolution_clock::now();
                string hash = cryptographic_hash(input, size, cost);
                auto end = chrono::high_resolution_clock::now();
                auto duration = chrono::duration_cast<chrono::milliseconds>(end - start).count();
                cout << "Hash: " << hash << endl;
                cout << "Time: " << duration << " ms" << endl;
            }
            catch (const exception &e)
            {
                cout << "Error: " << e.what() << endl;
            }
            break;
        }
        case 2:
        {
            cout << run_tests() << endl;
            break;
        }
        case 3:
            cout << "Exiting..." << endl;
            return 0;
        default:
            cout << "Invalid choice!" << endl;
        }
    }
    return 0;
}