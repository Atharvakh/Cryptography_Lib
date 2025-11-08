#include "asymmetric.h"
#include <iostream>
#include <fstream>

using namespace std;
using namespace NTL;

void RSA::keygen()
{
    if (keyGenerated)
    {
        cout << "Keys have already been generated.\n";
        return;
    }

    p = GenPrime_ZZ(512);
    q = GenPrime_ZZ(508);
    n = p * q;
    phi = (p - 1) * (q - 1);

    e = ZZ(65537);
    if (GCD(e, phi) != 1)
    {
        e = ZZ(3);
        while (GCD(e, phi) != 1)
        {
            e += 2;
        }
    }

    d = InvMod(e, phi);
    keyGenerated = true;

    // Save public key
    ofstream pubFile("publickey_rsa.txt");
    if (pubFile.is_open())
    {
        pubFile << n << "\n"
                << e << "\n";
        pubFile.close();
        cout << "Public key saved to publickey_rsa.txt\n";
    }
    else
    {
        cout << "Error: Unable to save public key.\n";
    }

    // Save private key
    ofstream privFile("privatekey_rsa.txt");
    if (privFile.is_open())
    {
        privFile << n << "\n"
                 << d << "\n";
        privFile.close();
        cout << "Private key saved to privatekey_rsa.txt\n";
    }
    else
    {
        cout << "Error: Unable to save private key.\n";
    }
}
bool RSA::loadKeys()
{
    ifstream pubFile("publickey_rsa.txt");
    ifstream privFile("privatekey_rsa.txt");

    if (!pubFile.is_open() || !privFile.is_open())
    {
        cout << "Error: Could not open key files.\n";
        return false;
    }

    // Load public key
    ZZ pub_n, pub_e;
    pubFile >> pub_n >> pub_e;
    pubFile.close();

    // Load private key
    ZZ priv_n, priv_d;
    privFile >> priv_n >> priv_d;
    privFile.close();

    n = pub_n;
    e = pub_e;
    d = priv_d;
    keyGenerated = true;

    return true;
}
void RSA::encrypt()
{
    cout << "Enter message as an integer " << endl;
    ZZ msg;
    cin >> msg;
    if (msg >= n)
    {
        cout << "Error: Plaintext must be less than modulus n.\n";
        return;
    }
    ZZ cipher = PowerMod(msg, e, n);
    // cout << "Ciphertext: " << cipher << "\n";
    sign(msg);
    ofstream cipherFile("rsa_cipher.txt");
    if (cipherFile.is_open())
    {

        cipherFile << cipher << "\n";
        cipherFile.close();
        cout << "Ciphertext saved to rsa_cipher.txt\n";
    }
    else
    {
        cout << "Error: Unable to save ciphertext.\n";
    }
}

void RSA::decrypt()
{

    ifstream cipherFile("rsa_cipher.txt");
    if (!cipherFile.is_open())
    {
        cout << "Error: Could not open ciphertext file.\n";
        return;
    }

    ZZ cipher;
    cipherFile >> cipher;
    cipherFile.close();

    ZZ msg = PowerMod(cipher, d, n);
    cout << "Decrypted message: " << msg << "\n";
    verify(msg, "rsa_signature.txt");
}

void RSA::sign(const ZZ &m)
{
    if (!keyGenerated)
    {
        cout << "Error: Keys not generated or loaded.\n";
        return;
    }

    ZZ signature = PowerMod(m, d, n);

    ofstream sigFile("rsa_signature.txt");
    if (sigFile.is_open())
    {
        sigFile << signature << "\n";
        sigFile.close();
        cout << "Signature saved to rsa_signature.txt\n";
    }
    else
    {
        cout << "Error: Unable to save signature.\n";
    }
}

bool RSA::verify(const ZZ &m, const string &signatureFile)
{
    ifstream sigFile(signatureFile);
    if (!sigFile.is_open())
    {
        cout << "Error: Could not open signature file.\n";
        return false;
    }

    ZZ signature;
    sigFile >> signature;
    sigFile.close();

    ZZ v = PowerMod(signature, e, n);

    if (v == m)
    {
        cout << "Signature is valid.\n";
        return true;
    }
    else
    {
        cout << "Signature is invalid.\n";
        return false;
    }
}