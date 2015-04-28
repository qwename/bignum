#include "bignum.h"

bool BigNum::operator==(const BigNum &rhs) const
{
    printDebug(toStr(*this) + " == " + toStr(rhs));
    if (base != rhs.base)
    {
        throw string("Error: Base mismatch.");
    }
    if (neg != rhs.neg || exp != rhs.exp || 
        getDigits() != rhs.getDigits() )
    {
        return false;
    }
    vector<char>::const_iterator i, j;
    for (i = sig.begin(), j = rhs.sig.begin(); i != sig.end(); ++i, ++j)
    {
        if (*i != *j)
        {
            return false;
        }
    }
    return true;
}

bool BigNum::operator!=(const BigNum &rhs) const
{
    printDebug(toStr(*this) + " != " + toStr(rhs));
    return !(*this == rhs);
}

bool BigNum::operator<(const BigNum &rhs) const
{
    printDebug(toStr(*this) + " < " + toStr(rhs));
    return false;
}

bool BigNum::operator<=(const BigNum &rhs) const
{
    printDebug(toStr(*this) + " <= " + toStr(rhs));
    return ((*this < rhs) || (*this == rhs));
}

bool BigNum::operator>(const BigNum &rhs) const
{
    printDebug(toStr(*this) + " > " + toStr(rhs));
    return !(*this <= rhs);
}

bool BigNum::operator>=(const BigNum &rhs) const
{
    printDebug(toStr(*this) + " >= " + toStr(rhs));
    return ((*this > rhs) || (*this == rhs));
}
