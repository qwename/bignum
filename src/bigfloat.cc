#include "bigfloat.h"
#include <algorithm>

const int BigFloat::DefaultPrecision = 30;
int BigFloat::precision = DefaultPrecision;

BigFloat::~BigFloat() { }

BigFloat Floor(const BigFloat &bn)
{
    BigFloat temp;
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

BigFloat Fract(const BigFloat &bn)
{
    BigFloat temp;
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

int BigFloat::getDigits() const
{
    return floorDigits() + fractDigits();
}

void BigFloat::SetPrecision(int prec)
{
    if (prec < 0)
    {
        prec = 0;
    }
    precision = prec;
}

BigFloat BigFloat::operator+() const
{
    printDebug("operator+ " + toStr());
    return *this;
}

BigFloat BigFloat::operator-() const
{
    printDebug("operator- " + toStr());
    BigFloat temp = *this;
    temp.neg = !temp.neg;
    return temp;
}

void BigFloat::removeZeros()
{
    while (sig.size() > 1 && 0 == sig.back()) // Remove leading zeros
    {
        sig.pop_back();
    }
    const int digits = sig.size();
    // Make sure ZERO has the right values
    if (1 == digits && 0 == sig[0])
    {
        neg = false;
        exp = 0;
        return;
    }
    int trail_zero = 0;
    while (trail_zero < digits && 0 == sig[trail_zero])
    {   // Check for trailling zeros
        ++trail_zero;
    }
    // Remove trailing zeros, if any
    sig.erase(sig.begin(), sig.begin() + trail_zero);
    exp += trail_zero;
}

int BigFloat::floorDigits() const
{
    const int temp = sig.size() + exp;
    if (temp <= 0)
    {   // Treat 0 before . as one digit
        return 1;
    }
    return temp;
}

int BigFloat::fractDigits() const
{
    if (exp >= 0)
    {
        return 0;
    }
    return -exp;
}

int BigFloat::floorSize() const
{
    if (exp >= 0)
    {
        return sig.size();
    }
    const int size = sig.size() + exp;
    if (size <= 0)
    {   // Magnitude between 0 and 1
        return 0;
    }
    return size;
}

int BigFloat::fractSize() const
{
    return sig.size() - floorSize();
}

void BigFloat::throwInvalidNumber(const string &n)
{
    throw string("Error: Invalid Number ") + n;
}

