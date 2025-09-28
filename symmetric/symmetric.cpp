#include "symmetric.h"
#include <fstream>
#include <random>
#include <iostream>
using namespace std;
using namespace NTL;
/* --- CaesarCipher --- */
void CaesarCipher::encrypt()
{
    int shift;
    cout << "Enter shift: ";
    cin >> shift;
    shift = shift % 26;

    string message;
    cout << "Enter Your Message: ";
    cin.ignore(); // clear buffer
    getline(cin, message);

    for (int i = 0; i < message.size(); i++)
    {
        if (isupper(message[i]))
        {
            message[i] = 'A' + (message[i] - 'A' + shift) % 26;
        }
        else if (islower(message[i]))
        {
            message[i] = 'a' + (message[i] - 'a' + shift) % 26;
        }
        else
        {
            // leave unchanged
            message[i] = message[i];
        }
    }

    cout << "Encrypted Message: " << message << endl;
}

void CaesarCipher::decrypt()
{
    int shift;
    cout << "Enter shift: ";
    cin >> shift;
    shift = shift % 26;

    string message;
    cout << "Enter Your Encrypted Message: ";
    cin.ignore(); // clear buffer
    getline(cin, message);

    for (int i = 0; i < message.size(); i++)
    {
        if (isupper(message[i]))
        {
            message[i] = 'A' + (message[i] - 'A' - shift + 26) % 26;
        }
        else if (islower(message[i]))
        {
            message[i] = 'a' + (message[i] - 'a' - shift + 26) % 26;
        }
        else
        {
            // leave unchanged
            message[i] = message[i];
        }
    }

    cout << "Decrypted Message: " << message << endl;
}

char VigenereCipher::encryptChar(char ch, char keyChar)
{
    int key_shift = toupper(keyChar) - 'A';

    if (isupper(ch))
    {
        return char(((ch - 'A' + key_shift) % 26) + 'A');
    }
    else if (islower(ch))
    {
        return char(((ch - 'a' + key_shift) % 26) + 'a');
    }
    else
    {
        return ch; // non-alphabet characters unchanged
    }
}

void VigenereCipher::encrypt()
{
    string text, key;
    cout << "Enter the text: ";
    if (cin.peek() == '\n')
        cin.ignore();
    getline(cin, text);

    cout << "Enter the key: ";
    getline(cin, key);

    int n = key.length();
    if (n == 0)
    {
        cout << "Key cannot be empty!" << endl;
        return;
    }

    // Validate and normalize key
    for (int i = 0; i < n; i++)
    {
        if (!isalpha(key[i]))
        {
            cout << "Key must contain only letters!" << endl;
            return;
        }
        key[i] = toupper(key[i]);
    }

    string cipher = "";
    int keyIndex = 0;

    for (int i = 0; i < (int)text.length(); i++)
    {
        if (isalpha(text[i]))
        {
            cipher += encryptChar(text[i], key[keyIndex % n]);
            keyIndex++;
        }
        else
        {
            cipher += text[i];
        }
    }

    cout << "Encrypted Message: " << cipher << endl;
}

void VigenereCipher::decrypt()
{
    string text, key;
    cout << "Enter the text: ";
    if (cin.peek() == '\n')
        cin.ignore();
    getline(cin, text);

    cout << "Enter the key: ";
    getline(cin, key);

    int n = key.length();
    if (n == 0)
    {
        cout << "Key cannot be empty!" << endl;
        return;
    }

    for (int i = 0; i < n; i++)
    {
        if (!isalpha(key[i]))
        {
            cout << "Key must contain only letters!" << endl;
            return;
        }
        key[i] = toupper(key[i]);
    }

    string plain = "";
    int keyIndex = 0;

    for (int i = 0; i < (int)text.length(); i++)
    {
        if (isupper(text[i]))
        {
            int shift = key[keyIndex % n] - 'A';
            plain += char(((text[i] - 'A' - shift + 26) % 26) + 'A');
            keyIndex++;
        }
        else if (islower(text[i]))
        {
            int shift = key[keyIndex % n] - 'A';
            plain += char(((text[i] - 'a' - shift + 26) % 26) + 'a');
            keyIndex++;
        }
        else
        {
            plain += text[i];
        }
    }

    cout << "Decrypted Message: " << plain << endl;
}
bool HillCipher::isInvertible(const vector<vector<int>> &key)
{
    long size = key.size();
    ZZ_p::init(ZZ(26)); // Set modulus to 26

    mat_ZZ_p mat;
    mat.SetDims(size, size);

    for (long i = 0; i < size; i++)
        for (long j = 0; j < size; j++)
            mat[i][j] = key[i][j];

    ZZ_p detMod = determinant(mat);
    ZZ det = rep(detMod);
    return GCD(conv<ZZ>(det), ZZ(26)) == 1;
}
HillCipher::HillCipher(const vector<vector<int>> &key)
{
    if (!HillCipher::isInvertible(key))
    {
        cout << "Error: Hill cipher key is not invertible modulo 26.\n";
        return;
    }
    initKey(key);
    cout << "HillCipher initialized with key:" << endl;
    for (auto &row : keyMatrix)
    {
        for (auto v : row)
            cout << v << " ";
        cout << endl;
    }
}
/* --- HillCipher --- */
void HillCipher::encrypt()
{
    cout << "Enter plaintext: ";
    string text;
    getline(cin >> ws, text);

    string cleaned;
    for (char c : text)
        if (isalpha(c))
            cleaned += toupper(c);
    while (cleaned.size() % blockSize != 0)
        cleaned += 'X';

    long nBlocks = cleaned.size() / blockSize;
    mat_ZZ_p msg;
    msg.SetDims(nBlocks, blockSize);

    for (long i = 0; i < nBlocks; i++)
        for (long j = 0; j < blockSize; j++)
            msg[i][j] = cleaned[i * blockSize + j] - 'A';

    mat_ZZ_p cipher = msg * keyNTL;

    cout << "Encrypted Text: ";
    for (long i = 0; i < cipher.NumRows(); i++)
        for (long j = 0; j < blockSize; j++)
            cout << char(conv<long>(cipher[i][j]) + 'A');
    cout << endl;
}

void HillCipher::decrypt()
{
    cout << "Enter ciphertext: ";
    string text;
    getline(cin >> ws, text);

    string cleaned;
    for (char c : text)
        if (isalpha(c))
            cleaned += toupper(c);
    while (cleaned.size() % blockSize != 0)
        cleaned += 'X';

    long nBlocks = cleaned.size() / blockSize;
    mat_ZZ_p cipher;
    cipher.SetDims(nBlocks, blockSize);

    for (long i = 0; i < nBlocks; i++)
        for (long j = 0; j < blockSize; j++)
            cipher[i][j] = cleaned[i * blockSize + j] - 'A';

    mat_ZZ_p keyInv;
    inv(keyInv, keyNTL);

    mat_ZZ_p plain = cipher * keyInv;

    cout << "Decrypted Text: ";
    for (long i = 0; i < plain.NumRows(); i++)
        for (long j = 0; j < blockSize; j++)
            cout << char(conv<long>(plain[i][j]) + 'A');
    cout << endl;
}
//------OneTimePad------------------
OneTimePad::OneTimePad(const string &k) : key(k) {}

string OneTimePad::generateKey(int length)
{
    string k;
    for (int i = 0; i < length; ++i)
    {
        char c = 'A' + rand() % 26; // Random uppercase letter
        k += c;
    }
    return k;
}

void OneTimePad::encrypt()
{
    string message;
    cout << "Enter Your Message: ";
    if (cin.peek() == '\n')
        cin.ignore();
    getline(cin, message);

    string cleaned;
    for (char c : message)
    {
        if (isalpha(c))
            cleaned += toupper(c);
    }

    if (cleaned.empty())
    {
        cout << "Message contains no alphabetic characters!" << endl;
        return;
    }

    key = generateKey(cleaned.size());

    // Save key to file
    ofstream keyFile("otp_key.txt");
    if (!keyFile)
    {
        cerr << "Failed to write key to file!" << endl;
        return;
    }
    keyFile << key;
    keyFile.close();

    string cipher;
    for (size_t i = 0; i < cleaned.size(); i++)
    {
        char encChar = 'A' + (cleaned[i] - 'A' + key[i] - 'A') % 26;
        cipher += encChar;
    }

    cout << "Encrypted Message: " << cipher << endl;
}

void OneTimePad::decrypt()
{
    // Load key from file
    ifstream keyFile("otp_key.txt");
    if (!keyFile)
    {
        throw runtime_error("Key file 'otp_key.txt' not found!");
    }

    getline(keyFile, key);
    keyFile.close();

    string cipher;
    cout << "Enter Your Encrypted Message: ";
    if (cin.peek() == '\n')
        cin.ignore();
    getline(cin, cipher);

    string cleaned;
    for (char c : cipher)
    {
        if (isalpha(c))
            cleaned += toupper(c);
    }

    if (cleaned.empty())
    {
        cout << "Ciphertext contains no alphabetic characters!" << endl;
        return;
    }

    if (key.size() < cleaned.size())
    {
        throw runtime_error("Stored key is shorter than the ciphertext!");
    }

    string plain;
    for (size_t i = 0; i < cleaned.size(); i++)
    {
        char decChar = 'A' + (cleaned[i] - 'A' - (key[i] - 'A') + 26) % 26;
        plain += decChar;
    }

    cout << "Decrypted Message: " << plain << endl;
    if (remove("otp_key.txt") != 0)
    {
        cerr << "Warning: Failed to delete key file!" << endl;
    }
}