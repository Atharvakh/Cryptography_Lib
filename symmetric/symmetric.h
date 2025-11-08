#include "../cipher.h"
#include <vector>
#include <string>
#include <NTL/mat_ZZ_p.h>
#include <NTL/ZZ_p.h>

using namespace std;
using namespace NTL;

// ---------------- Caesar Cipher ----------------
class CaesarCipher : public Cipher
{
private:
    int shift;

public:
    CaesarCipher(int s = 3) : shift(s) {}
    void encrypt() override;
    void decrypt() override;
};

// ---------------- Vigenere Cipher ----------------
class VigenereCipher : public Cipher
{
private:
    string key;
    char encryptChar(char ch, char keyChar);

public:
    VigenereCipher(const string &k = "") : key(k) {}
    void encrypt() override;
    void decrypt() override;
};

// ---------------- Hill Cipher ----------------
class HillCipher : public Cipher
{
private:
    vector<vector<int>> keyMatrix; // stored for reference
    mat_ZZ_p keyNTL;               // NTL version for calculations
    long blockSize;

    void initKey(const vector<vector<int>> &keyMat)
    {
        blockSize = keyMat.size();
        keyMatrix = keyMat;

        ZZ_p::init(ZZ(26));
        keyNTL.SetDims(blockSize, blockSize);

        for (long i = 0; i < blockSize; i++)
            for (long j = 0; j < blockSize; j++)
                keyNTL[i][j] = keyMat[i][j];

        // check invertibility
        ZZ_p det = determinant(keyNTL);
        if (GCD(rep(det), ZZ(26)) != 1)
        {
            throw runtime_error("Key matrix determinant not coprime with 26!");
        }
    }

public:
    HillCipher(const vector<vector<int>> &keyMat);
    // constructor takes normal vector for convenience
    void encrypt() override;
    void decrypt() override;
    static bool isInvertible(const vector<vector<int>> &key);
};

// ---------------- One-Time Pad ----------------
class OneTimePad : public Cipher
{
private:
    string key;
    string generateKey(int length);

public:
    OneTimePad(const string &k = "");
    void encrypt() override;
    void decrypt() override;
};
