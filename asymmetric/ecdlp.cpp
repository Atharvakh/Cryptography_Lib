#include "asymmetric.h"
#include <iostream>
#include <stdexcept>

using namespace std;
using namespace NTL;

// ----------------------------------------------------
// Utility: Modular inverse with NTL ZZ
// ----------------------------------------------------
static ZZ modInverse(const ZZ &a, const ZZ &p)
{
    ZZ inv;
    long status = InvModStatus(inv, a, p);
    if (status == 0)
        return inv;
    else
        throw runtime_error("No modular inverse exists!");
}

// ----------------------------------------------------
// EC_ElGamal Constructor
// ----------------------------------------------------
EC_ElGamal::EC_ElGamal(const ZZ &p, const ZZ &a, const ZZ &b, const Point &G, const ZZ &n)
{
    this->p = p;
    this->a = a;
    this->b = b;
    this->G = G;
    this->n = n;
    this->d = ZZ(0);
    this->Q = {ZZ(0), ZZ(0)};
}

// ----------------------------------------------------
// Check if a point lies on the curve
// ----------------------------------------------------
bool EC_ElGamal::isvalid(const Point &P)
{
    if (P.x == 0 && P.y == 0)
        return true; // Point at infinity (by convention)

    ZZ_p::init(p);

    ZZ_p lhs = power(conv<ZZ_p>(P.y), 2);
    ZZ_p rhs = power(conv<ZZ_p>(P.x), 3) + conv<ZZ_p>(a) * conv<ZZ_p>(P.x) + conv<ZZ_p>(b);
    return lhs == rhs;
}

// ----------------------------------------------------
// Point Doubling: R = 2P
// ----------------------------------------------------
Point EC_ElGamal::dbl(const Point &P)
{
    if (P.y == 0)
        return {ZZ(0), ZZ(0)}; // point at infinity

    ZZ_p::init(p);
    ZZ_p slope = (3 * power(conv<ZZ_p>(P.x), 2) + conv<ZZ_p>(a)) *
                 inv(conv<ZZ_p>(2) * conv<ZZ_p>(P.y));

    ZZ_p xr = power(slope, 2) - conv<ZZ_p>(2) * conv<ZZ_p>(P.x);
    ZZ_p yr = slope * (conv<ZZ_p>(P.x) - xr) - conv<ZZ_p>(P.y);

    Point R = {rep(xr), rep(yr)};
    return R;
}

// ----------------------------------------------------
// Point Addition: R = P + Q
// ----------------------------------------------------
Point EC_ElGamal::add(const Point &P, const Point &Q)
{
    if (P.x == 0 && P.y == 0)
        return Q;
    if (Q.x == 0 && Q.y == 0)
        return P;

    if (P.x == Q.x && (P.y + Q.y) % p == 0)
        return {ZZ(0), ZZ(0)}; // P + (-P) = O

    ZZ_p::init(p);

    ZZ_p slope = (conv<ZZ_p>(Q.y) - conv<ZZ_p>(P.y)) *
                 inv(conv<ZZ_p>(Q.x) - conv<ZZ_p>(P.x));

    ZZ_p xr = power(slope, 2) - conv<ZZ_p>(P.x) - conv<ZZ_p>(Q.x);
    ZZ_p yr = slope * (conv<ZZ_p>(P.x) - xr) - conv<ZZ_p>(P.y);

    Point R = {rep(xr), rep(yr)};
    return R;
}

// ----------------------------------------------------
// Scalar Multiplication: R = k * P
// (Double-and-Add Algorithm)
// ----------------------------------------------------
Point EC_ElGamal::mul(const ZZ &k, const Point &P)
{
    Point R = {ZZ(0), ZZ(0)}; // point at infinity
    Point base = P;

    long nbits = NumBits(k);
    for (long i = 0; i < nbits; i++)
    {
        if (bit(k, i))
            R = add(R, base);
        base = dbl(base);
    }
    return R;
}

// ----------------------------------------------------
// Key Generation
// ----------------------------------------------------
void EC_ElGamal::keygen()
{
    // Private key d ∈ [1, n-1]
    RandomBnd(d, n - 1);
    if (d == 0)
        d = 1;

    // Public key Q = d * G
    Q = mul(d, G);

    cout << "ECC Key Generated\n";
    cout << "Private key (d): " << d << "\n";
    cout << "Public key (Q): (" << Q.x << ", " << Q.y << ")\n";
}

// ----------------------------------------------------
// Encrypt message as a point on the curve
// Ciphertext = (C1, C2) = (kG, M + kQ)
// ----------------------------------------------------
void EC_ElGamal::encrypt()
{
    // For demo purposes: we use a small "message point"
    Point M = {conv<ZZ>(1804), conv<ZZ>(4371)}; // You can later map actual data to points
    if (!isvalid(M))
        throw runtime_error("Message point M is not on the curve!");

    ZZ k;
    RandomBnd(k, n - 1);
    if (k == 0)
        k = 1;

    Point C1 = mul(k, G);
    Point kQ = mul(k, Q);
    Point C2 = add(M, kQ);

    cout << "\nEncryption complete:\n";
    cout << "Random k: " << k << "\n";
    cout << "C1 = (" << C1.x << ", " << C1.y << ")\n";
    cout << "C2 = (" << C2.x << ", " << C2.y << ")\n";

    // Optionally, save ciphertext to file
    ofstream fout("ec_elgamal_cipher.txt");
    fout << C1.x << " " << C1.y << "\n"
         << C2.x << " " << C2.y << endl;
    fout.close();
}

// ----------------------------------------------------
// Decrypt ciphertext
// M = C2 - d * C1
// ----------------------------------------------------
void EC_ElGamal::decrypt()
{
    ifstream fin("ec_elgamal_cipher.txt");
    if (!fin.is_open())
        throw runtime_error("Ciphertext file not found!");

    Point C1, C2;
    fin >> C1.x >> C1.y >> C2.x >> C2.y;
    fin.close();

    Point dC1 = mul(d, C1);

    // Compute -dC1
    Point neg_dC1 = {dC1.x, (p - dC1.y) % p};

    Point M = add(C2, neg_dC1);

    cout << "\nDecryption complete:\n";
    cout << "Recovered Message Point M = (" << M.x << ", " << M.y << ")\n";
}
