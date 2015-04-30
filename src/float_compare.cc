#include "bigfloat.h"

bool BigFloat::operator==(const BigFloat &rhs) const
{
    printDebug(toStr() + " == " + rhs.toStr());
    if (base != rhs.base)
    {   // No support for different bases
        throw string("Error: Base mismatch.");
    }
    if (neg != rhs.neg || exp != rhs.exp || sig.size() != rhs.sig.size())
    {
        return false;
    }
    vector<char>::const_iterator i, j;
    for (i = sig.begin(), j = rhs.sig.begin(); i != sig.end(); ++i, ++j)
    {   // Compare each digit, order doesn't matter
        if (*i != *j)
        {
            return false;
        }
    }
    return true;
}

bool BigFloat::operator!=(const BigFloat &rhs) const
{
    printDebug(toStr() + " != " + rhs.toStr());
    return !(*this == rhs);
}

bool BigFloat::operator<(const BigFloat &rhs) const
{
    printDebug(toStr() + " < " + rhs.toStr());
    if (base != rhs.base)
    {
        throw string("Error: Base mismatch.");
    }
    if (!neg && rhs.neg)
    {   // + > -, thus false
        return false;
    }
    if (neg && !rhs.neg)
    {   // - < +, thus true
        return true;
    }
    // Same polarity
    // Compare Floor
    if (floorDigits() != rhs.floorDigits())
    {   // Obtain XNOR expression using truth tables
        // Works only if the if statement is true
        return !((floorDigits() > rhs.floorDigits()) ^ neg);
    }
    vector<char>::const_reverse_iterator i, j, end;
    i = sig.rbegin();
    j = rhs.sig.rbegin();
    if (floorSize() != rhs.floorSize() && (floorSize() <= 1 || rhs.floorSize() <= 1))
    {   // Both have one digit floor
        if (equalZero())
        {   // If this == 0, this < rhs if both positive, or neg false
            return !neg;
        }
        else
        {
            if (floorSize() == 0)
            {
                if (rhs.equalZero())
                {
                    return neg;
                }
                return !neg;
            }
            return neg;
        }
    }
    else if (floorSize() != 0)
    {   // Both magnitude greater than zero
        end = i + floorSize();
        for (; i != end; ++i, ++j)
        {   // Compare digits starting from most significant position
            if (*i != *j)
            {   // Derived XNOR using previous logic
                return !((*i > *j) ^ neg);
            }
        }
    }
    // Equal Floor, now compare Fract
    if (floorSize() == 0)
    {   // Both have magnitude between 0 and 1
        // Need to taken care of missing zeros
        // Number of zeros before first nonzero digit
        int zeros = fractDigits() - fractSize();
        int rhs_zeros = rhs.fractDigits() - rhs.fractSize();
        if (zeros != rhs_zeros)
        {   // Similar to previous comparison between floorDigits
            return !((zeros < rhs_zeros) ^ neg);
        }
    }
    for (; i != sig.rend() && j != rhs.sig.rend(); ++i, ++j)
    {   // Compare each digit
        if (*i != *j)
        {   // Derived XNOR using previous logic
            return !((*i > *j) ^ neg);
        }
    }
    // Only way for this < rhs is to reach end of this
    // The case where both are equal is considered
    return i == sig.rend() && j != rhs.sig.rend();
}

bool BigFloat::operator<=(const BigFloat &rhs) const
{
    printDebug(toStr() + " <= " + rhs.toStr());
    return ((*this < rhs) || (*this == rhs));
}

bool BigFloat::operator>(const BigFloat &rhs) const
{
    printDebug(toStr() + " > " + rhs.toStr());
    return rhs < *this;
}

bool BigFloat::operator>=(const BigFloat &rhs) const
{
    printDebug(toStr() + " >= " + rhs.toStr());
    return rhs <= *this;
}

bool BigFloat::equalZero() const
{
    return neg == false && sig.size() == 1 && 0 == sig[0] && exp == 0;
}

bool BigFloat::equalBasePower() const
{
    return sig.size() == 1 && 1 == sig[0];
}
