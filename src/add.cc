#include "bignum.h"

BigNum BigNum::operator+(const BigNum &rhs) const
{
    printDebug(toStr(*this) + " + " + toStr(rhs));
    BigNum temp = *this;
    return temp += rhs;
}

BigNum& BigNum::operator++()
{
    printDebug("++" + toStr(*this));
    *this += BigNum(1);
    return *this;
}

BigNum BigNum::operator++(int)
{
    printDebug(toStr(*this) + "++");
    BigNum temp = *this;
    *this += BigNum(1);
    return temp;
}

BigNum& BigNum::operator+=(const BigNum &rhs)
{
    printDebug(toStr(*this) + " += " + toStr(rhs));
    if (neg && !rhs.neg)
    {
        *this = rhs - -(*this);
        return *this;
    }
    if (!neg && rhs.neg)
    {
        return *this -= -rhs;
    }
    if (sig.size() == 0)
    {
        *this = rhs;
        return *this;
    }
    char temp = 0;
    bool carry = false;
    alignDigits(rhs);
    vector<char>::reverse_iterator i = sig.rbegin() + (fractDigits() - rhs.fractDigits());
    vector<char>::const_reverse_iterator j = rhs.sig.rbegin();
    vector<char>::reverse_iterator k;
    printDebug(toStrDebug(*this));
    for (; j != rhs.sig.rend(); ++i, ++j)
    {
        temp = *i + *j;
        if (carry)  // Previous
        {
            ++temp;
        }
        *i = temp % base;
        carry = (temp >= base);
        printDebug(toStrDebug(*this));
    }
    while (carry)
    {
        if (sig.rend() == i)
        {
            sig.insert(sig.begin(), 1);
            i = sig.rend();
            printDebug(toStrDebug(*this));
            break;
        }
        ++(*i);
        carry = (*i >= base);
        *i %= base;
        ++i;
        printDebug(toStrDebug(*this));
    }
    removeZeros();
    return *this;
}

