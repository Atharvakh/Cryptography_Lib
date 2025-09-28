#include <iostream>
#include <string>
using namespace std;
int main()
{
    string text, key;
    cout << "Enter the text: ";
    getline(cin, text);
    cout << "Enter the key: ";
    getline(cin, key);
    int m = text.length();
    int n = key.length();
    string cipher = "";
    for (int i = 0; i < m; i++)
    {
        if (text[i] >= 'A' && text[i] <= 'Z')
        {
            cipher += char((text[i] + key[i % n] - 2 * 'A') % 26 + 'A'); // Here we subtract 2*'A' because we are adding two characters and we need to bring them in the range of 0-25
            // Then we take mod 26 to wrap around if it goes beyond 'Z'
            // Finally we add 'A' to bring it back to the ASCII range of uppercase letters
        }
        else if (text[i] >= 'a' && text[i] <= 'z')
        {
            cipher += char((text[i] + key[i % n] - 2 * 'a') % 26 + 'a');
        }
        else
        {
            cipher += text[i];
        }
    }
    cout << cipher << endl;
}