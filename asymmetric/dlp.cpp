#include "asymmetric.h"
#include <fstream>
#include <iostream>

using namespace std;

ElGamalEncryption::ElGamalEncryption()
{
    if (!loadKeys())
    {
        cout << "Generating a 2048-bit prime p...\n";
        p = GenPrime_ZZ(512, 160);
        g = ZZ(2);
    }

    // cout << "Prime p generated: " << p << endl;
    // do
    // {
    //     g = RandomBnd(p - 2) + 1;
    // } while (g <= 1 || g >= p);
    // cout << "Generator g chosen: " << g << endl;
}

void ElGamalEncryption::keygen()
{
    do
    {
        x = RandomBnd(p - 2) + 1;
    } while (x <= 1 || x >= p - 1);
    G = p - 1;
    h = PowerMod(g, x, p);

    ofstream pub("public_key.txt");
    pub << p << "\n"
        << g << "\n"
        << h << endl;
    pub.close();
    ofstream pri("private_key.txt");
    pri << x << endl;
    pri.close();

    // cout << "Public key (p, g, h): (" << p << ", " << g << ", " << h << ")\n";
}
bool ElGamalEncryption::loadKeys()
{
    ifstream pub("public_key.txt");
    ifstream priv("private_key.txt");

    if (!pub || !priv)
    {
        cout << "Key files not found. Run keygen first.\n";
        return false;
    }

    pub >> p >> g >> h;
    priv >> x;
    G = p - 1;
    keyGenerated = true;

    pub.close();
    priv.close();
    return true;
}

void ElGamalEncryption::encrypt()
{
    if (!keyGenerated)
        return;
    // string msg;
    ZZ m;
    cout << "Enter message as integer: ";
    cin >> m;

    // type_conversion converter;
    // ZZ m = converter.stringToZZ(msg_text);

    if (m >= p)
    {
        cout << "Error: Message must be less than prime p.\n";
        return;
    }

    ZZ y, s;
    ZZ c1, c2;

    do
    {
        y = RandomBnd(p - 2) + 1;
        c1 = PowerMod(g, y, p);
        s = PowerMod(h, y, p);
    } while (GCD(s, p) != 1); // retry until s is invertible

    c2 = (m * s) % p;
    lastCipher = {c1, c2};

    // cout << "Ciphertext: (" << c1 << ", " << c2 << ")" << endl;

    cout << "Signing the message...\n";
    sign(m);
    cout << "Message signed.\n";
}
void ElGamalEncryption::sign(const ZZ &m)
{

    if (m >= p)
    {
        cout << "Error: Message must be less than prime p.\n";
        return;
    }

    ZZ k, r, delta;
    do
    {
        k = RandomBnd(p - 2) + 1;
    } while (GCD(k, p - 1) != 1); // k must be coprime to p-1

    r = PowerMod(g, k, p);
    delta = (InvMod(k, p - 1) * (m - x * r)) % (p - 1);
    if (delta < 0)
        delta += (p - 1);
    saveSignature(r, delta, "signature.txt");
}
void ElGamalEncryption::saveCipher(const string &ciphertext)
{
    ofstream out(ciphertext);
    out << lastCipher[0] << "\n"
        << lastCipher[1] << "\n";
    out.close();
}
bool ElGamalEncryption::loadCipher(const string &ciphertext)
{
    ifstream in(ciphertext);
    if (!in)
        return false;
    ZZ c1, c2;
    in >> c1 >> c2;
    lastCipher = {c1, c2};
    in.close();
    return true;
}
void ElGamalEncryption::saveSignature(const ZZ &r, const ZZ &delta, const string &signature)
{
    ofstream out(signature);
    out << r << "\n"
        << delta << "\n";
    out.close();
}

void ElGamalEncryption::decrypt()
{
    if (!keyGenerated)
        return;

    if (lastCipher.empty())
    {
        cout << "Error: No ciphertext found. Please encrypt a message first.\n";
        return;
    }
    cout << lastCipher[0] << endl;
    cout << lastCipher[1] << endl;

    ZZ c1 = lastCipher[0];
    ZZ c2 = lastCipher[1];

    ZZ s = PowerMod(c1, x, p);

    cout << "s = " << s << endl;
    cout << "GCD(s, p) = " << GCD(s, p) << endl;

    if (GCD(s, p) != 1)
    {
        cout << "Error: Shared secret is not invertible. Ciphertext may be corrupted.\n";
        return;
    }

    ZZ s_inv = InvMod(s, p);

    ZZ m = (c2 * s_inv) % p;
    cout << "Decrypted message: " << m << endl;

    verify(m, "signature.txt");
}

bool ElGamalEncryption::verify(const ZZ &m, const string &signature)
{

    ZZ r, delta;
    ifstream in(signature);
    if (!in)
    {
        cout << "Error: Signature file not found.\n";
        return false;
    }
    in >> r >> delta;
    in.close();

    if (r <= 0 || r >= p || delta <= 0 || delta >= p - 1)
    {
        cout << "Error: Invalid signature values.\n";
        return false;
    }

    ZZ v1 = (PowerMod(h, r, p) * PowerMod(r, delta, p)) % p;
    ZZ v2 = PowerMod(g, m, p);

    if (v1 == v2)
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