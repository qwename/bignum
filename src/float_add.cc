#include "bigfloat.h"
#include <algorithm>

BigFloat BigFloat::operator+(const BigFloat &rhs) const
{
    printDebug(toStr() + " + " + rhs.toStr());
    BigFloat temp = *this;
    return temp += rhs;
}

BigFloat& BigFloat::operator++()
{
    printDebug("++" + toStr());
    *this += BigFloat(1);
    return *this;
}

BigFloat BigFloat::operator++(int)
{
    printDebug(toStr() + "++");
    BigFloat temp = *this;
    *this += BigFloat(1);
    return temp;
}

BigFloat& BigFloat::operator+=(const BigFloat &rhs)
{
    printDebug(toStr() + " += " + rhs.toStr());
    if (equalZero())
    {
        *this = rhs;
        return *this;
    }
    if (rhs.equalZero())
    {
        return *this;
    }
    if (neg && !rhs.neg)
    {   // For this < 0, rhs > 0, this + rhs == -(|this| - rhs)
        neg = false;
        *this -= rhs;
        neg = true;
        return *this;
    }
    if (!neg && rhs.neg)
    {
        return *this -= -rhs;
    }
    vector<char>::iterator i;
    vector<char>::const_iterator j, end;
    i = sig.begin();
    j = rhs.sig.begin();
    const int diff = exp - rhs.exp;
    if (diff > 0)
    {   // rhs has lower index
        exp = rhs.exp;
        end = rhs.sig.begin() + std::min((int)rhs.sig.size(), diff);
        int k;
        for (k = 0; j != end; ++j, ++k)
        {
            sig.insert(sig.begin() + k, *j);
        }
        const int gap = diff - rhs.sig.size();
        if (gap > 0)
        {   // rhs has no more indices remaining
            // Fill gaps with 0s
            sig.insert(sig.begin() + k, gap, 0);
            removeZeros();
            return *this;
        }
        i = sig.begin() + k; 
    }
    else if (diff < 0)
    {   // this has lower index
        const int gap = -diff - sig.size();
        if (gap > 0)
        {   // this has no more indices remaining
            // Fill gaps with 0s
            sig.insert(sig.end(), gap, 0);
            // Copy remaining digits from rhs
            sig.reserve(sig.size() + (rhs.sig.end() - j));
            for (; j != rhs.sig.end(); ++j)
            {
                sig.push_back(*j);
            }
            removeZeros();
            return *this;
        }
        i = sig.begin() + -diff;
    }
    // Overlapping digits
    int carry = 0;
    for (; i != sig.end(); ++i)
    {
        if (j != rhs.sig.end())
        {
            *i += *j;
            ++j;
        }
        else if (0 == carry)
        {   // Nothing more to add
            break;
        }
        *i += carry;
        carry = *i / base;
        if (carry != 0)
        {
            *i -= base;
        }
        printDebug(toStrDebug(*this));
    }
    for (; j != rhs.sig.end(); ++j)
    {
        sig.push_back(*j + carry);
        carry = sig.back() / base;
        if (carry != 0)
        {
            sig.back() -= base;
        }
        printDebug(toStrDebug(*this));
    }
    if (1 == carry)
    {
        sig.push_back(carry);
        printDebug(toStrDebug(*this));
    }
    removeZeros();
    return *this;
}

