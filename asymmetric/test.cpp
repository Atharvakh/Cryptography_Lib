#include "asymmetric.h"
#include <iostream>

using namespace std;
using namespace NTL;

int main()
{
    // Example parameters for the elliptic curve
    // ZZ p = conv<ZZ>("9739");
    // ZZ a = conv<ZZ>("497");
    // ZZ b = conv<ZZ>("1768");
    // Point G = {conv<ZZ>("1804"), conv<ZZ>("5368")};
    // ZZ n = conv<ZZ>("9739"); // Example order, should be set correctly
    // EC_ElGamal ecc(p, a, b, G, n);
    // ecc.keygen();
    // ecc.encrypt();
    // ecc.decrypt();

    RSA rsa_obj;
    rsa_obj.loadKeys();
    rsa_obj.keygen();
    rsa_obj.encrypt();
    rsa_obj.decrypt();
    return 0;
}