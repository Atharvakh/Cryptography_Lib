#include <NTL/ZZ.h>
#include <fstream>
#include <string>

using namespace std;
using namespace NTL;

class KeyGenerator
{
public:
    static long generateDHKey(const string &filename)
    {
        // Diffie-Hellman parameters
        ZZ p(INIT_VAL, "23");
        ZZ g(INIT_VAL, "5");

        ZZ a = RandomBnd(p); // Alice
        ZZ b = RandomBnd(p); // Bob

        // Public keys
        ZZ A = PowerMod(g, a, p);
        ZZ B = PowerMod(g, b, p);

        // Shared secret
        ZZ s1 = PowerMod(B, a, p);
        ZZ s2 = PowerMod(A, b, p);

        long key = conv<long>(s1 % 26);

        ofstream fout(filename);
        fout << key;
        fout.close();

        return key;
    }

    static long readKey(const string &filename)
    {
        ifstream fin(filename);
        long key;
        fin >> key;
        fin.close();
        return key;
    }
};
