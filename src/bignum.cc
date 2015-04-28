#include "bignum.h"
#include <algorithm>

const int BigNum::DefaultBase = 10;
const int BigNum::DefaultPrecision = 30;
int BigNum::precision = DefaultPrecision;

BigNum::~BigNum() { }

BigNum Floor(const BigNum &bn)
{
    BigNum temp;
    temp.sig.clear();
    if (bn.exp > 0)
    {
        temp.exp = bn.exp;
    }
    bool trail_zero = true;
    const int digits = bn.sig.size();
    for (int i = bn.fractDigits(); i < digits; ++i)
    {
        if (trail_zero)
        {
            if (0 == bn.sig[i])
            {   // Skip trailing zeros
                ++temp.exp;
                continue;
            }
            trail_zero = false;
        }
        temp.sig.push_back(bn.sig[i]);
    }
    if (0 == temp.sig.size())
    {   // ZERO
        temp.sig.push_back(0);
        temp.exp = 0;
    }
    else
    {
        temp.neg = bn.neg;
    }
    temp.base = bn.base;
    return temp;
}

BigNum Fract(const BigNum &bn)
{
    BigNum temp;
    temp.sig.clear();
    const int digits = bn.fractDigits();
    for (int i = 0; i < digits; ++i)
    {
        temp.sig.push_back(bn.sig[i]);
    }
    // Zero before decimal point
    temp.sig.push_back(0);
    if (1 != temp.sig.size())
    {
        temp.neg = bn.neg;
        temp.exp = -digits;
    }
    temp.base = bn.base;
    return temp;
}

/*
BigNum Abs(const BigNum &bn)
{
    if (bn.neg)
    {
        return -bn;
    }
    return bn;
}
*/

int BigNum::getDigits() const
{
    return floorDigits() + fractDigits();
}

void BigNum::SetPrecision(int prec)
{
    if (prec < 0)
    {
        prec = 0;
    }
    precision = prec;
}

/*
BigNum& BigNum::operator--()
{
    printDebug("--" + toStr(*this));
    *this -= BigNum(1);
    return *this;
}

BigNum BigNum::operator--(int)
{
    printDebug(toStr(*this) + "--");
    BigNum temp = *this;
    *this -= BigNum(1);
    return temp;
}

BigNum BigNum::operator+() const
{
    printDebug("operator+ " + toStr(*this));
    return *this;
}

BigNum BigNum::operator-() const
{
    printDebug("operator- " + toStr(*this));
    BigNum temp = *this;
    temp.neg = !temp.neg;
    return temp;
}

BigNum BigNum::operator-(const BigNum &rhs) const
{
    printDebug(toStr(*this) + " - " + toStr(rhs));
    BigNum temp = *this;
    return temp -= rhs;
}

BigNum BigNum::operator*(const BigNum &rhs) const
{
    printDebug(toStr(*this) + " * " + toStr(rhs));
    BigNum temp = *this;
    return temp *= rhs;
}

BigNum BigNum::operator/(const BigNum &rhs) const
{
    printDebug(toStr(*this) + " / " + toStr(rhs));
    BigNum temp = *this;
    return temp /= rhs;
}

BigNum BigNum::operator%(const BigNum &rhs) const
{
    printDebug(toStr(*this) + " % " + toStr(rhs));
    BigNum temp = *this;
    return temp %= rhs;
}
*/

void BigNum::removeZeros()
{
    while (floorDigits() > 1 && 0 == sig.back()) // Remove leading zeros
    {
        sig.pop_back();
    }
    const int digits = getDigits();
    int trail_zero = 0;
    while (trail_zero < digits && 0 == sig[trail_zero])
    {   // Check for trailling zeros
        ++trail_zero;
    }
    // Remove trailing zeros, if any
    sig.erase(sig.begin(), sig.begin() + trail_zero);
    exp += trail_zero;
    // Make sure ZERO has the right values
    if (1 == sig.size() && 0 == sig[0])
    {
        neg = false;
        exp = 0;
    }
}

int BigNum::floorDigits() const
{
    const int temp = sig.size() + exp;
    if (temp <= 0)
    {   // Treat 0 before . as one digit
        return 1;
    }
    return temp;
}

int BigNum::fractDigits() const
{
    if (exp >= 0)
    {
        return 0;
    }
    return -exp;
}

int BigNum::floorSize() const
{
    int size = sig.size() + exp;
    if (size <= 0)
    {   // Magnitude between 0 and 1
        return 0;
    }
    return size;
}

int BigNum::fractSize() const
{
    return sig.size() - floorSize();
}

void BigNum::throwInvalidNumber(const string &n)
{
    throw string("Error: Invalid Number ") + n;
}

