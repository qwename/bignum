#include "bigfloat.h"

BigFloat BigFloat::operator-(const BigFloat &rhs) const
{
    printDebug(toStr() + " - " + rhs.toStr());
    BigFloat temp = *this;
    return temp -= rhs;
}

BigFloat& BigFloat::operator--()
{
    printDebug("--" + toStr());
    *this -= BigFloat(1);
    return *this;
}

BigFloat BigFloat::operator--(int)
{
    printDebug(toStr() + "--");
    BigFloat temp = *this;
    *this -= BigFloat(1);
    return temp;
}

BigFloat& BigFloat::operator-=(const BigFloat &rhs)
{
    printDebug(toStr() + " -= " + rhs.toStr());
    if (rhs.equalZero())
    {   // Check rhs first to prevent -0
        return *this;
    }
    if (equalZero())
    {
        *this = -rhs;
        return *this;
    }
    if (neg)
    {
        if (!rhs.neg)
        {   // For this < 0, rhs > 0, this - rhs == -(|this| + rhs)
            neg = false;
            *this += rhs;
            neg = true;
        }
        else
        {   // For this, rhs < 0, this - rhs == |rhs| - |this|
            neg = false;
            *this = -rhs - *this;
        }
        return *this;
    }
    if (rhs.neg)
    {   // this > 0
        return *this += -rhs;
    }
    if (*this < rhs)
    {
        *this = -(rhs - *this);
        return *this;
    }
    // Remaining case of this, rhs > 0, this >= rhs
    vector<char>::iterator i;
    vector<char>::const_iterator j, end;
    i = sig.begin();
    j = rhs.sig.begin();
    const int diff = exp - rhs.exp;
    if (diff > 0)
    {   // rhs has lower index
        exp = rhs.exp;
        end = rhs.sig.begin() + std::min((int)rhs.sig.size(), diff);
        sig.insert(sig.begin(), base - *j);
        int k = 1;
        for (++j; j != end; ++j, ++k)
        {
            sig.insert(sig.begin() + k, base - 1 - *j);
        }
        const int gap = diff - rhs.sig.size();
        if (gap > 0)
        {   // rhs has no more indices remaining
            // Fill gaps with 9s
            sig.insert(sig.begin() + k, gap, 9);
            // Don't forget to subtract the borrow!
            sig[diff] -= 1;
            removeZeros();
            return *this;
        }
        // Don't forget to subtract the borrow!
        sig[diff] -= 1;
        i = sig.begin() + k; 
    }
    else if (diff < 0)
    {   // this has lower index
        i += -diff;
    }
    // Overlapping digits
    int borrow = 0;
    for (; i != sig.end(); ++i)
    {
        if (j != rhs.sig.end())
        {
            *i -= *j;
            ++j;
        }
        *i -= borrow;
        if (*i < 0)
        {
            borrow = 1;
            *i += base;
        }
        else
        {
            borrow = 0;
        }
        printDebug(toStrDebug(*this));
    }
    if (1 == borrow)
    {
        throw string("Error: Subtraction failed");
    }
    removeZeros();
    return *this;
}
