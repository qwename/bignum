#include "bignum.h"
#include <iostream>

const unsigned int BigNum::DefaultBase = 10;

BigNum::BigNum() : neg(false), sig(), base(DefaultBase), exp(0)
{
    sig.push_back(0);
}

BigNum::BigNum(const BigNum &rhs) : neg(rhs.neg), sig(rhs.sig), base(DefaultBase), exp(rhs.exp) { }

BigNum::BigNum(int n) : neg(n < 0), sig(), base(DefaultBase), exp(0)
{
    if (0 == n)
    {
        sig.push_back(0);
    }
    if (neg)
    {
        n = -n;
    }
    while (n != 0)
    {
        sig.insert(sig.begin(), n % base);
        n /= base;
    }
}

BigNum::BigNum(string n) : neg('-' == n[0]), sig(), base(DefaultBase), exp(0)
{
    if (!isValidNumber(n))
    {
        throw string("Invalid Number: ") + n;
    }
    bool decimal = false, nonzero = false, leading_zero = true;
    char val = 0;
    string::const_iterator i = n.begin();
    if (neg)
    {
        ++i;
    }
    int trailing_zero = 0;
    for (; i != n.end(); ++i)
    {
        if ('.' == *i)
        {
            decimal = true;
            leading_zero = false;
            continue;
        }
        val = *i - '0';
        if (leading_zero)
        {
            if (val != 0)
            {
                leading_zero = false;
            }
            else
            {
                continue;
            }
        }
        if (!nonzero && val != 0)
        {
            nonzero = true;
        }
        if (decimal)
        {
            if (0 == val)
            {
                ++trailing_zero;
            }
            else if (trailing_zero > 0)
            {
                trailing_zero = 0;
            }
            --exp;
        }
        sig.push_back(val);
    }
    if (!nonzero)
    {
        sig.clear();
        sig.push_back(0);
        exp = 0;
        return;
    }
    for (; trailing_zero != 0; --trailing_zero)
    {
        sig.pop_back();
        ++exp;
    }
}

BigNum::~BigNum() { }

// Add significant digits, decimal places
void BigNum::print() const
{
    using std::cout;
    using std::endl;
    if (neg)
    {
        cout << "-";
    }
    for (vector<char>::const_iterator i = sig.begin(); i != sig.end(); ++i)
    {
        if (sig.size() + exp == i - sig.begin())
        {
            cout << '.';
        }
        cout << (int)*i;
    }
    cout << endl;
}

int BigNum::getDigits() const
{
    return sig.size();
}

BigNum& BigNum::operator=(const BigNum &rhs)
{
    neg = rhs.neg;
    sig = rhs.sig;
    exp = rhs.exp;
    base = rhs.base;
    return *this;
}

bool BigNum::operator==(const BigNum &rhs) const
{
    if (neg != rhs.neg)
    {
        return false;
    }
    if (getDigits() != rhs.getDigits())
    {
        return false;
    }
    vector<char>::const_iterator i, j;
    for (i = sig.begin(), j = rhs.sig.begin(); i != sig.end(); ++i, ++j)
    {
        if (*j != *i)
        {
            return false;
        }
    }
    return true;
}

bool BigNum::operator!=(const BigNum &rhs) const
{
    return !(*this == rhs);
}

bool BigNum::operator<(const BigNum &rhs) const
{
    if (!neg && rhs.neg)    // - !> +
    {
        return false;
    }
    if (neg && !rhs.neg)    // - < +
    {
        return true;
    }
    vector<char>::const_iterator i, j;
    int digits = getDigits() + exp;
    int rhs_digits = rhs.getDigits() + rhs.exp;
    if (!neg)   // Both positive
    {
        if (digits > rhs_digits)
        {
            return false;
        }
        else if (digits == rhs_digits)
        {
            for (i = sig.begin(), j = rhs.sig.begin(); i != sig.end(); ++i, ++j)
            {
                if (*i >= *j)
                {
                    return false;
                }
            }
        }
    }
    else        // Both negative
    {
        if (digits < rhs_digits)
        {
            return false;
        }
        else if (digits == rhs_digits)
        {
            for (i = sig.begin(), j = rhs.sig.begin(); i != sig.end(); ++i, ++j)
            {
                if (*i <= *j)
                {
                    return false;
                }
            }
        }
    }
    return true;
}

bool BigNum::operator<=(const BigNum &rhs) const
{
    return ((*this < rhs) || (*this == rhs));
}

bool BigNum::operator>(const BigNum &rhs) const
{
    return !(*this <= rhs);
}

bool BigNum::operator>=(const BigNum &rhs) const
{
    return ((*this > rhs) || (*this == rhs));
}

BigNum& BigNum::operator++()
{
    *this += BigNum(1);
    return *this;
}

BigNum BigNum::operator++(int)
{
    BigNum temp = *this;
    *this += BigNum(1);
    return temp;
}

BigNum& BigNum::operator--()
{
    *this -= BigNum(1);
    return *this;
}

BigNum BigNum::operator--(int)
{
    BigNum temp = *this;
    *this -= BigNum(1);
    return temp;
}

BigNum& BigNum::operator+=(const BigNum &rhs)
{
    char carry = 0, temp = 0;
    // Add zeros for easier addition logic
    while (dp() < rhs.dp())
    {
        sig.push_back(0);
        --exp;
    }
    // Align
    vector<char>::reverse_iterator i = sig.rbegin() + (dp() - rhs.dp());
    vector<char>::const_reverse_iterator j = rhs.sig.rbegin();
    for (; j != rhs.sig.rend(); ++i, ++j)
    {
        if (sig.rend() == i)
        {
            sig.insert(sig.begin(), *j);
            i = sig.rend() - 1;
            continue;
        }
        temp = *i + *j + carry;
        carry = temp / base;
        *i = temp % base;
    }
    return *this;
}

BigNum& BigNum::operator-=(const BigNum &rhs)
{
    return *this;
}

BigNum BigNum::operator+()
{
    return *this;
}

BigNum BigNum::operator-()
{
    BigNum temp = *this;
    temp.neg = !temp.neg;
    return temp;
}

bool BigNum::isValidNumber(string n) const
{
    if (n.size() < 1)
    {
        return false;
    }
    bool decimal = false, digits = false;
    for (string::const_iterator i = n.begin(); i != n.end(); ++i)
    {
        switch (*i)
        {
            case '.':
            {
                if (decimal)
                {
                    return false;
                }
                decimal = true;
                continue;
            }
            case '-':
            {
                if (i != n.begin())
                {
                    return false;
                }
                continue;
            }
            default:
            {
                if (*i < '0' || '9' < *i)
                {
                    return false;
                }
                digits = true;
            }
        }
    }
    if (!digits)
    {
        return false;
    }
    return true;
}

int BigNum::dp() const
{
    if (exp < 0)
    {
        return -exp;
    }
    else
    {
        return 0;
    }
}
