#include "bignum.h"

BigNum BigNum::operator/(const BigNum &rhs) const
{
    printDebug(toStr(*this) + " / " + toStr(rhs));
    BigNum temp = *this;
    return temp /= rhs;
}

BigNum& BigNum::operator/=(const BigNum &rhs)
{
    printDebug(toStr(*this) + " /= " + toStr(rhs));
    if (rhs.equalZero())
    {
        throw string("Error: Divide by zero");
    }
    if (equalZero())
    {
        return *this;
    }
}
