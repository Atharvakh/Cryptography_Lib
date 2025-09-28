#include <iostream>
using namespace std;

int main()
{
    int shift;
    cout << "Enter shift: " << endl;
    cin >> shift;
    shift = shift % 26;
    string message;
    cout << "Enter Your Message: " << endl;
    cin.ignore();
    /*When you use cin >> to read input (like an integer or word),
    it leaves behind a newline character ('\n') in the input buffer.
    If you then use getline() immediately after, it will read that leftover newline as an empty line.
    */
    getline(cin, message);

    for (int i = 0; i < message.size(); i++)
    {
        if (isupper(message[i]))
            message[i] = 65 + (message[i] - 65 + shift) % 26;
        else if (islower(message[i]))
            message[i] = 97 + (message[i] - 97 + shift) % 26;
        else
            message[i] = message[i];
    }

    cout << "Encrypted Message: " << message << endl;
    return 0;
}