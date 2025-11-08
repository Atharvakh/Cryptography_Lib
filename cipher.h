#pragma once
#include <string>

class Cipher
{
public:
    virtual void encrypt() = 0;
    virtual void decrypt() = 0;
};
