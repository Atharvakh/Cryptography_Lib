#include <iostream>
#include <NTL/mat_ZZ.h>
#include <NTL/ZZ.h>

using namespace NTL;
using namespace std;
// g++ Hill_Cipher.cpp -o Hill_Cipher -I/usr/local/include -L/usr/local/lib -lntl -lgmp to compile the code
int main()
{
    // Define the dimension of the matrix
    long n = 3;

    // Create an n x n matrix of ZZ type (arbitrary-precision integers)
    mat_ZZ A;
    A.SetDims(n, n);

    // Populate the matrix with some values
    // Using a nested loop for demonstration
    for (long i = 0; i < n; ++i)
    {
        for (long j = 0; j < n; ++j)
        {
            A.put(i, j, ZZ(i * n + j + 1));
        }
    }

    // Print the matrix
    cout << "The generated " << n << "x" << n << " matrix is:" << endl;
    cout << A << endl;

    return 0;
}