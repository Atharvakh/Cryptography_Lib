#include "../cipher.h"
#include <NTL/ZZ.h>
#include <NTL/ZZ_p.h>
#include <vector>
using namespace NTL;
using namespace std;

class AsymmetricCipher : public Cipher
{
public:
    virtual void keygen() = 0;
};

// ---------------- ElGamal ----------------
class ElGamalEncryption : public AsymmetricCipher
{
private:
    ZZ p, g, h, x;
    ZZ G;
    vector<ZZ> lastCipher;
    bool keyGenerated = false;
    bool loadKeys();

public:
    ElGamalEncryption();
    void keygen();
    void encrypt() override;
    void decrypt() override;
};
