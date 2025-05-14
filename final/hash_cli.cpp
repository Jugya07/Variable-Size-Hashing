#include "hash_core.h"
#include <iostream>
#include <stdexcept>

using namespace std;

int main()
{
    while (true)
    {
        cout << "\nCustom Hashing Algorithm\n";
        cout << "1. Compute Hash\n2. Run Tests\n3. Exit\n";
        cout << "Choose option: ";

        int choice;
        cin >> choice;
        cin.ignore(); // Clear newline

        if (choice == 1)
        {
            string input;
            cout << "Enter the string to hash: ";
            getline(cin, input);

            int output_size_bits;
            cout << "Enter the output size in bits: ";
            cin >> output_size_bits;
            cin.ignore();

            try
            {
                string hash = cryptographic_hash(input, output_size_bits);
                cout << "Hash: " << hash << endl;
            }
            catch (const exception &e)
            {
                cout << "Error: " << e.what() << endl;
            }
        }
        else if (choice == 2)
        {
            cout << run_tests();
        }
        else if (choice == 3)
        {
            cout << "Exiting...\n";
            break;
        }
        else
        {
            cout << "Invalid option. Try again.\n";
        }
    }

    return 0;
}