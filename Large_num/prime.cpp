#include <iostream>
#include <fstream>
#include <chrono>

#include <NTL/ZZ.h>

using namespace std;
using namespace NTL;
using namespace std::chrono;

int main()
{
    int flag = 0;
    ifstream file;
    file.open("num.txt");

    if (!file.is_open())
    {
        cerr << "Error opening file!" << endl;
        return 1;
    }

    ZZ num;
    file >> num;
    auto start = high_resolution_clock::now();

    for (ZZ i = conv<ZZ>(2); i <= SqrRoot(num); i++)
    {
        if (num % i == 0)
        {
            cout << "Not Prime" << endl;
            flag = 1;
            break;
        }
    }
    auto end = high_resolution_clock::now();
    duration<double> elapsed = end - start;
    cout << "Time taken: " << elapsed.count() << " seconds" << endl;
    if (!flag)
        cout << "Prime" << endl;
    return 0;
}