#include "bigfloat.h"

BigFloat BigFloat::operator*(const BigFloat &rhs) const
{
    printDebug(toStr() + " * " + rhs.toStr());
    BigFloat temp = *this;
    return temp *= rhs;
}

BigFloat& BigFloat::operator*=(const BigFloat &rhs)
{
    printDebug(toStr() + " *= " + rhs.toStr());
    if (equalZero() || rhs.equalZero())
    {
        *this = 0;
        return *this;
    }
    // XOR, product is negative if only one of this or rhs is neg
    neg = neg ^ rhs.neg;
    exp += rhs.exp;
    if (rhs.equalBasePower())
    {
        printDebug(toStrDebug(*this));
        return *this;
    }
    if (equalBasePower())
    {
        *this = rhs * *this;
        printDebug(toStrDebug(*this));
        return *this;
    }
    // Long Multiplication
    BigFloat old = *this;
    *this = 0;
    vector<char>::iterator i;
    vector<char>::const_iterator j = rhs.sig.begin();
    int carry = 0;
    for (BigFloat prod = old; j != rhs.sig.end(); ++j)
    {
        ++old.exp;
        if (*j == 0)
        {
            ++prod.exp;
            continue;
        }
        else if (*j != 1)
        {
            for (i = prod.sig.begin(); i != prod.sig.end(); ++i)
            {
                *i *= *j;
                *i += carry;
                carry = *i / base;
                if (carry != 0)
                {
                    *i -= carry * base;
                }
                printDebug(toStrDebug(prod));
            }
            if (carry != 0)
            {
                prod.sig.push_back(carry);
                printDebug(toStrDebug(prod));
                carry = 0;
            }
        }
        *this += prod;
        prod = old;
        printDebug(toStrDebug(*this));
    }
    return *this;
}
