#include "../cipher.h"
#include <NTL/ZZ.h>
#include <NTL/ZZ_p.h>
#include <NTL/ZZ_pX.h>
#include <vector>
#include <string>
#include <fstream>

using namespace NTL;
using namespace std;

// ----------------------------------------------------
// Base Class for all Asymmetric Ciphers
// ----------------------------------------------------
class AsymmetricCipher : public Cipher
{
public:
    virtual void keygen() = 0;
};

// ----------------------------------------------------
// Point structure for Elliptic Curve operations
// ----------------------------------------------------
struct Point
{
    ZZ x;
    ZZ y;
};

// ----------------------------------------------------
// Utility function declarations
// // ----------------------------------------------------
// vector<Point> find_points(const ZZ &a, const ZZ &b, const ZZ &p);
// void save_points_to_file(const vector<Point> &points,
//                          const ZZ &a, const ZZ &b,
//                          const ZZ &p, const string &filename);
// bool has_square_root(const ZZ_p &val, ZZ_p &root);

// ----------------------------------------------------
// -------- ElGamal (Modular Arithmetic) ---------------
// ----------------------------------------------------
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

    void saveCipher(const string &ciphertext);
    bool loadCipher(const string &ciphertext);

    void saveSignature(const ZZ &r, const ZZ &delta, const string &signature);
    void sign(const ZZ &m);
    bool verify(const ZZ &m, const string &signatureFile);
};

// ----------------------------------------------------
// -------- ECDLP Core (ECC Arithmetic) ----------------
// // ----------------------------------------------------
// class ECDLP : public AsymmetricCipher
// {
// private:
//     ZZ p;      // prime modulus
//     ZZ m;      // private key
//     ZZ a, b;   // curve parameters
//     ZZ Gx, Gy; // base point coordinates
//     ZZ n;      // order of base point
//     ZZ yQ, yP; // public key (if needed)

// public:
//     ECDLP(const ZZ &p, const ZZ &a, const ZZ &b);

//     void keygen();
//     void encrypt() override;
//     void decrypt() override;
// };

// ----------------------------------------------------
// ---- Elliptic Curve ElGamal Encryption --------------
// ----------------------------------------------------
class EC_ElGamal : public AsymmetricCipher
{
private:
    ZZ p;    // Field prime
    ZZ a, b; // Curve parameters
    Point G; // Base point
    ZZ n;    // Order of G
    ZZ d;    // Private key
    Point Q; // Public key = d * G

    // Core EC arithmetic
    bool isvalid(const Point &P);
    Point add(const Point &P, const Point &Q);
    Point dbl(const Point &P);
    Point mul(const ZZ &k, const Point &P);

public:
    EC_ElGamal(const ZZ &p, const ZZ &a, const ZZ &b, const Point &G, const ZZ &n);

    void keygen();
    void encrypt() override;
    void decrypt() override;
};

class RSA : public AsymmetricCipher
{
private:
    ZZ p, q; // prime numbers
    ZZ n;    // modulus
    ZZ phi;  // Euler's totient
    ZZ e;    // public exponent
    ZZ d;    // private exponent
    vector<ZZ> lastCipher;
    bool keyGenerated = false;

public:
    bool loadKeys();
    void keygen() override;
    void encrypt() override;
    void decrypt() override;

    void sign(const ZZ &m);
    bool verify(const ZZ &m, const string &signatureFile);
};