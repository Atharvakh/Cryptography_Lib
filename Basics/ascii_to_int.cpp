#include <string>
#include <iostream>
#include <NTL/ZZ.h>

using namespace NTL;
using namespace std;
// long long asciiTOint(string m)
// {
//     long long enc = 0;
//     for (char c : m)
//         enc = enc * 1000 + static_cast<int>(c);
//     return enc;
// }
ZZ stringToZZ(const string &input)
{
    ZZ result;
    result = ZZFromBytes(reinterpret_cast<const unsigned char *>(input.data()), input.size());
    return result;
}

// string intTOascii(long long n)
// {
//     string dec = "";
//     while (n > 0)
//     {
//         int rem = n % 1000;
//         dec = char(rem) + dec;
//         n /= 1000;
//     }
//     return dec;
// }
string ZZToString(const ZZ &number)
{
    long size = NumBytes(number);
    string output(size, '\0');
    BytesFromZZ(reinterpret_cast<unsigned char *>(&output[0]), number, size);
    return output;
}

int main()
{
    string input;
    cout << "Enter a string: ";
    getline(cin, input); // No need for cin.ignore() here

    ZZ result = stringToZZ(input);
    cout << "Encoded integer: " << result << endl;

    string decoded = ZZToString(result);
    cout << "Decoded string: " << decoded << endl;

    return 0;
}
