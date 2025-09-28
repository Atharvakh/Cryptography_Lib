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

    ZZ m;
    cout << "Enter message as integer: ";
    cin >> m;

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

    cout << "Ciphertext: (" << c1 << ", " << c2 << ")" << endl;
}

void ElGamalEncryption::decrypt()
{
    if (!keyGenerated)
        return;

    cout << lastCipher[0] << endl;
    cout << lastCipher[1] << endl;
    if (lastCipher.empty())
    {
        cout << "Error: No ciphertext found. Please encrypt a message first.\n";
        return;
    }

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
}
