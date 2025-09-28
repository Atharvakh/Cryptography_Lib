#include <string>
#include <NTL/mat_ZZ_p.h>
#include <NTL/ZZ_p.h>
#include <NTL/ZZ.h>
#include <cmath>
#include <numeric>
#include <vector>
#include <stdexcept>
using namespace NTL;
using namespace std;

class Cipher
{
public:
    virtual void encrypt() = 0;
    virtual void decrypt() = 0;
};
class Encryption
{
private:
    ZZ x, m, y; // private key, message, ephemeral key

public:
    ZZ g = GenPrime_ZZ(64), G, h, p = GenPrime_ZZ(128); // public parameters 170141183460469231731687303715884105727

    ZZ mod_pow(const ZZ &base, const ZZ &exp, const ZZ &mod)
    {
        return PowerMod(base, exp, mod);
    }
    void generate_key();
    vector<ZZ> encrypt();
    ZZ getPrivateKey() const
    {
        return x;
    }
};
class Decryption
{
public:
    ZZ mod_pow(const ZZ &base, const ZZ &exp, const ZZ &mod)
    {
        return PowerMod(base, exp, mod);
    }
    ZZ decrypt(ZZ &x, vector<ZZ> &cipher, const ZZ &p);
};

class CaesarCipher : public Cipher
{
public:
    void encrypt() override;
    void decrypt() override;
};

class VigenereCipher : public Cipher
{
public:
    void encrypt() override;
    void decrypt() override;

private:
    char encryptChar(char plainChar, char keyChar);
};

class HillCipher : public Cipher
{
public:
    void encrypt() override;
    void decrypt() override;
};