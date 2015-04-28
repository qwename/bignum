#include "bignum.h"

BigNum::BigNum() : neg(false), sig(), base(DefaultBase), exp(0)
{   // Only representation of zero
    sig.push_back(0);
}

BigNum::BigNum(const BigNum &rhs) : neg(rhs.neg), sig(rhs.sig), base(DefaultBase), exp(rhs.exp) { }

BigNum::BigNum(int n) : neg(n < 0), sig(), base(DefaultBase), exp(0)
{
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

BigNum::BigNum(string n) : neg('-' == n[0]), sig(), base(DefaultBase), exp(0)
{
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

BigNum& BigNum::operator=(const BigNum &rhs)
{
    neg = rhs.neg;
    sig = rhs.sig;
    exp = rhs.exp;
    base = rhs.base;
    return *this;
}
