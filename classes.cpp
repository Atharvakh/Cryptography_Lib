#include <vector>
#include "header.h"
#include <limits>
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

// --- VigenereCipher ---
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

void HillCipher::encrypt()
{
    ZZ_p::init(ZZ(26));
    const long blockSize = 3;

    cout << "Enter text: ";
    string text;
    if (cin.peek() == '\n')
        cin.ignore();
    getline(cin, text);

    string cleaned;
    for (char c : text)
    {
        if (isalpha(c))
            cleaned += toupper(c);
    }
    while (cleaned.size() % blockSize != 0)
        cleaned += 'X';

    long nBlocks = cleaned.size() / blockSize;
    mat_ZZ_p msg;
    msg.SetDims(nBlocks, blockSize);

    for (long i = 0; i < nBlocks; i++)
    {
        for (long j = 0; j < blockSize; j++)
        {
            msg[i][j] = cleaned[i * blockSize + j] - 'A';
        }
    }

    cout << "Enter key (" << blockSize << "x" << blockSize << " matrix):" << endl;
    mat_ZZ_p key;
    key.SetDims(blockSize, blockSize);
    for (long i = 0; i < blockSize; i++)
    {
        for (long j = 0; j < blockSize; j++)
        {
            long val;
            cin >> val;
            key[i][j] = val;
        }
    }

    mat_ZZ_p cipher = msg * key;

    cout << "Encrypted Text: ";
    for (long i = 0; i < cipher.NumRows(); i++)
    {
        for (long j = 0; j < blockSize; j++)
        {
            cout << char(conv<long>(cipher[i][j]) + 'A');
        }
    }
    cout << endl;
}

void HillCipher::decrypt()
{
    ZZ_p::init(ZZ(26));
    const long blockSize = 3;

    cout << "Enter ciphertext: ";
    string text;
    if (cin.peek() == '\n')
        cin.ignore();
    getline(cin, text);

    string cleaned;
    for (char c : text)
    {
        if (isalpha(c))
            cleaned += toupper(c);
    }
    while (cleaned.size() % blockSize != 0)
        cleaned += 'X';

    long nBlocks = cleaned.size() / blockSize;
    mat_ZZ_p cipher;
    cipher.SetDims(nBlocks, blockSize);

    for (long i = 0; i < nBlocks; i++)
    {
        for (long j = 0; j < blockSize; j++)
        {
            cipher[i][j] = cleaned[i * blockSize + j] - 'A';
        }
    }

    cout << "Enter key (" << blockSize << "x" << blockSize << " matrix):" << endl;
    mat_ZZ_p key;
    key.SetDims(blockSize, blockSize);
    for (long i = 0; i < blockSize; i++)
    {
        for (long j = 0; j < blockSize; j++)
        {
            long val;
            cin >> val;
            key[i][j] = val;
        }
    }

    mat_ZZ_p keyInv;
    try
    {
        inv(keyInv, key); // just compute inverse
    }
    catch (...)
    {
        throw runtime_error("Key matrix is not invertible modulo 26!");
    }

    mat_ZZ_p plain = cipher * keyInv;

    cout << "Decrypted Text: ";
    for (long i = 0; i < plain.NumRows(); i++)
    {
        for (long j = 0; j < blockSize; j++)
        {
            cout << char(conv<long>(plain[i][j]) + 'A');
        }
    }
    cout << endl;
}

void Encryption ::generate_key()
{
    // cout << "Enter prime number p: ";
    // cin >> p;
    G = p - 1;
    // cout << "Enter primitive root g: ";
    // cin >> g;

    cout << "Enter private key x (1 < x < p-1): ";
    cin >> x;

    h = mod_pow(g, x, p);
    cout << "Public key (p, g, G, h): (" << p << ", " << g << ", " << G << ", " << h << ")" << endl;
}

vector<ZZ> Encryption ::encrypt()
{
    vector<ZZ> cipher(2);
    cout << "Enter message as integer: ";
    cin >> m;

    cout << "Enter random integer y (1 < y < p-1, gcd(y, p-1) != 1): ";
    cin >> y;
    ZZ p_minus_1 = p - 1;
    if (y <= 1 || y >= p_minus_1 || GCD(y, p_minus_1) != 1) // write loop for y
    {
        cout << "Invalid y!" << endl;
        return cipher;
    }

    ZZ c1 = mod_pow(g, y, p);
    ZZ s = mod_pow(h, y, p);
    ZZ c2 = (m * s) % p;
    cipher[0] = c1;
    cipher[1] = c2;
    cout << "Ciphertext: (" << c1 << ", " << c2 << ")" << endl;
    return cipher;
}
ZZ Decryption ::decrypt(ZZ &x, vector<ZZ> &cipher, const ZZ &p)
{
    ZZ c1 = cipher[0];
    ZZ c2 = cipher[1];
    ZZ s = mod_pow(c1, x, p);
    ZZ s_inv;
    try
    {
        InvMod(s_inv, s, p);
        // calculates the modular multiplicative inverse of s with respect
        // to the modulus p and stores the result in the variable s_inv
    }
    catch (...)
    {
        cout << "Error: Inverse does not exist!" << endl;
        return ZZ(-1);
    }
    ZZ m = (c2 * s_inv) % p;
    cout << "Decrypted message: " << m << endl;
    return m;
}

// vector<ZZ> ElGamalEncryption::encryptMessage(const ZZ &m, const ZZ &y)
// {
//     vector<ZZ> cipher(2);
//     ZZ c1 = PowerMod(g, y, p);
//     ZZ s = PowerMod(h, y, p);
//     ZZ c2 = (m * s) % p;
//     cipher[0] = c1;
//     cipher[1] = c2;
//     return cipher;
// }

// ZZ ElGamalEncryption::decryptMessage(const vector<ZZ> &cipher)
// {
//     ZZ c1 = cipher[0];
//     ZZ c2 = cipher[1];
//     ZZ s = PowerMod(c1, x, p);
//     ZZ s_inv;
//     InvMod(s_inv, s, p);
//     return (c2 * s_inv) % p;
// }
