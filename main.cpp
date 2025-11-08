#include <iostream>
#include "symmetric/symmetric.h"
#include "asymmetric/asymmetric.h"

using namespace std;

void symmetricMenu()
{
    int choice;
    cout << "\n--- Symmetric Ciphers ---\n";
    cout << "1. Caesar Cipher\n";
    cout << "2. Vigenere Cipher\n";
    cout << "3. Hill Cipher\n";
    cout << "4. One-Time Pad\n";
    cout << "Enter choice: ";
    cin >> choice;

    Cipher *cipher = nullptr;

    switch (choice)
    {
    case 1:
        cipher = new CaesarCipher();
        break;
    case 2:
        cipher = new VigenereCipher();
        break;
    case 3:
    {
        // Hardcoded example key for Hill cipher (3x3 invertible matrix mod 26)
        vector<vector<int>> key = {
            {6, 24, 1},
            {13, 16, 10},
            {20, 17, 15}};
        cipher = new HillCipher(key);
        break;
    }
    case 4:
        cipher = new OneTimePad();
        break;
    default:
        cout << "Invalid choice.\n";
        return;
    }

    int action;
    cout << "1. Encrypt\n2. Decrypt\nEnter action: ";
    cin >> action;

    if (action == 1)
    {
        cipher->encrypt();
    }
    else if (action == 2)
    {
        cipher->decrypt();
    }
    else
    {
        cout << "Invalid action.\n";
    }

    delete cipher;
}

void asymmetricMenu()
{
    static ElGamalEncryption elgamal;
    int choice;
    cout << "\n--- Asymmetric Ciphers ---\n";
    cout << "1. ElGamal Encryption\n";
    cout << "Enter choice: ";
    cin >> choice;

    if (choice == 1)
    {

        int action;
        cout << "1.Key Generation\n 2.Encrypt and Sign\n3. Decrypt ans Verify\nEnter action: ";
        cin >> action;
        if (action == 1)
        {
            elgamal.keygen();
        }
        else if (action == 2)
        {
            elgamal.encrypt();
        }
        else if (action == 3)
        {
            elgamal.decrypt();
        }
        else
        {
            cout << "Invalid action.\n";
        }
    }
    else
    {
        cout << "Invalid choice.\n";
    }
}

int main()
{
    while (true)
    {
        int choice;
        cout << "\n===== Cryptography Library =====\n";
        cout << "1. Symmetric Ciphers\n";
        cout << "2. Asymmetric Ciphers\n";
        cout << "0. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            symmetricMenu();
            break;
        case 2:
            asymmetricMenu();
            break;
        case 0:
            cout << "Exiting...\n";
            return 0;
        default:
            cout << "Invalid choice.\n";
        }
    }
}
