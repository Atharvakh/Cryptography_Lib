#include <iostream>
#include <NTL/ZZ.h>
using namespace std;
using namespace NTL;

int main()
{
    ZZ base = conv<ZZ>(2);
    long exponent = 65537;
    ZZ result = power(base, exponent);
    cout << "2^65537 = " << result << endl;
    return 0;
}
