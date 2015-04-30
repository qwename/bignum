#include "bigfloat.h"

BigFloat::BigFloat() : BigNum(), neg(false), sig(), exp(0)
{   // Only representation of zero
    sig.push_back(0);
}

BigFloat::BigFloat(const BigFloat &rhs) : BigNum(), neg(rhs.neg), sig(rhs.sig), exp(rhs.exp) { }

BigFloat::BigFloat(int n) : BigNum(), neg(n < 0), sig(), exp(0)
{
    printDebug("Init BigFloat(int)");
    if (0 == n)
    {
        sig.push_back(0);
        return;
    }
    bool extra = true;
    if (neg)
    {   // Get absolute value
        n = -n;
    }
    for (int val; n != 0; n /= base)
    {
        val = n % base;
        if (extra)
        {
            if (0 == val)
            {
                ++exp;
                continue;
            }
            extra = false;
        }
        sig.push_back(val);
    }
}

BigFloat::BigFloat(string n) : BigNum(), neg('-' == n[0]), sig(), exp(0)
{
    printDebug("Init BigFloat(string): " + n);
    if (n.size() < 1)
    {
        throwInvalidNumber(n);
    }
    bool decimal = false, valid = false, extra = true;
    char val = 0;
    // Iterating from the end
    string::const_reverse_iterator i = n.rbegin();
    string::const_reverse_iterator end = n.rend();
    if (neg)
    {   // Stop before '-' symbol
        --end;
    }
    for (; i != end; ++i)
    {
        if ('-' == *i)
        {
            // Only one '-' allowed
            throwInvalidNumber(n);
        }
        if ('.' == *i)
        {
            if (decimal)
            {   // Only one '.' allowed
                throwInvalidNumber(n);
            }
            decimal = true;
            // Number of digits so far
            exp = -sig.size();
            continue;
        }
        if (*i < '0' || '9' < *i)
        {   // Not within range
            throwInvalidNumber(n);
        }
        valid = true;
        // Subtract to get offset from '0', the actual value
        val = *i - '0';
        if (extra)
        {
            if (0 == val)
            {   // Trailing digits still zero
                ++exp;
                continue;
            }
            extra = false;
        }
        sig.push_back(val);
    }
    if (!valid)
    {   // No digits present
        throwInvalidNumber(n);
    }
    if (sig.size() == 0)
    {   // ZERO
        neg = false;
        sig.push_back(0);
        exp = 0;
    }
    // Remove all leading zeros to save space
    removeZeros();
}

BigFloat& BigFloat::operator=(const BigFloat &rhs)
{
    printDebug("Init operator=: " + rhs.toStr());
    neg = rhs.neg;
    sig = rhs.sig;
    exp = rhs.exp;
    base = rhs.base;
    return *this;
}
