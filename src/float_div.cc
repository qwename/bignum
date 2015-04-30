#include "bigfloat.h"

BigFloat BigFloat::operator/(const BigFloat &rhs) const
{
    printDebug(toStr() + " / " + rhs.toStr());
    BigFloat temp = *this;
    return temp /= rhs;
}

BigFloat& BigFloat::operator/=(const BigFloat &rhs)
{
    printDebug(toStr() + " /= " + rhs.toStr());
    if (rhs.equalZero())
    {
        throw string("Error: Divide by zero");
    }
    if (equalZero())
    {
        return *this;
    }
    // XOR, neg only if one of the operand is neg
    neg = neg ^ rhs.neg;
    exp -= rhs.exp;
    
}
