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
        n = n.substr(1);
        i = n.begin();
    }
    int trailing_zero = 0;
    for (; i != n.end(); ++i)
    {
        if ('.' == *i)
        {
            if (0 == sig.size())
            {
                sig.push_back(0);
            }
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
    cout << toStr() << endl;
}

int BigNum::getDigits() const
{
    return sig.size();
}

string BigNum::toStr() const
{
    string s = "";
    if (neg)
    {
        s += "-";
    }
    for (vector<char>::const_iterator i = sig.begin(); i != sig.end(); ++i)
    {
        if (floorDigits() == i - sig.begin())
        {
            s += '.';
        }
        s += (*i + '0');
    }
    return s;
}

BigNum BigNum::Floor() const
{
    string s = "";
    if (neg)
    {
        s += "-";
    }
    for (vector<char>::const_iterator i = sig.begin(); i != sig.begin() + floorDigits(); ++i)
    {
        s += (*i + '0');
    }
    return BigNum(s);
}

BigNum BigNum::Fract() const
{
    string s = "";
    if (neg)
    {
        s += "-";
    }
    s += ".";
    for (vector<char>::const_iterator i = sig.begin() + floorDigits(); i != sig.end(); ++i)
    {
        s += (*i + '0');
    }
    return BigNum(s);
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
                if (*i == *j)
                {
                    continue;
                }
                if (*i > *j)
                {
                    return false;
                }
            }
            if (sig.back() == rhs.sig.back()) // Equal
            {
                return false;
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
    if (neg && !rhs.neg)
    {
        *this = rhs - -(*this);
        return *this;
    }
    if (!neg && rhs.neg)
    {
        return *this -= -rhs;
    }
    char temp = 0;
    bool carry = false;
    // Add zeros for easier addition logic
    while (fractDigits() < rhs.fractDigits())
    {
        sig.push_back(0);
        --exp;
    }
    // Align
    vector<char>::reverse_iterator i = sig.rbegin() + (fractDigits() - rhs.fractDigits());
    vector<char>::const_reverse_iterator j = rhs.sig.rbegin();
    vector<char>::reverse_iterator k;
    for (;j != rhs.sig.rend(); ++i, ++j)
    {
        if (sig.rend() == i)
        {
            sig.insert(sig.begin(), *j);
            i = sig.rend() - 1;
            continue;
        }
        temp = *i + *j;
        carry = (temp >= base);
        *i = temp % base;
        if (carry)
        {
            for (k = i + 1; k != sig.rend(); ++k)
            {
                if (*k < base - 1)
                {
                    ++(*k);
                    for (--k; k != i; --k)
                    {
                        *k = 0;
                    }
                    break;
                }
            }
            if (sig.rend() == k)
            {
                int pos = i - sig.rbegin();
                sig.insert(sig.begin(), 1);
                i = sig.rbegin() + pos;
                for (k = sig.rend() - 2; k != i; --k)
                {
                    *k = 0;
                }
            }
        }
    }
    removeZeros();
    return *this;
}

BigNum& BigNum::operator-=(const BigNum &rhs)
{
    if (rhs.neg)
    {
        return *this += -rhs;
    }
    if (neg)
    {
        *this = -(-(*this) + rhs);
        return *this;
    }
    if (*this < rhs)
    {
        *this = -(rhs - *this);
        return *this;
    }
    // Add zeros for easier subtraction logic
    while (fractDigits() < rhs.fractDigits())
    {
        sig.push_back(0);
        --exp;
    }
    char temp = 0;
    bool borrow = false;
    // Align
    vector<char>::reverse_iterator i = sig.rbegin() + (fractDigits() - rhs.fractDigits());
    vector<char>::const_reverse_iterator j = rhs.sig.rbegin();
    vector<char>::reverse_iterator k;
    for (; j != rhs.sig.rend(); ++i, ++j)
    {
        temp = *i - *j;
        borrow = (temp < 0);
        if (borrow)
        {
            for (k = i + 1; k != sig.rend(); ++k)
            {
                if (*k != 0)
                {
                    --(*k);
                    do
                    {
                        --k;
                        *k += base;
                    } while (k != i);
                    *i -= *j;
                    break;
                }
            }
        }
        else
        {
            *i = temp;
        }
    }
    removeZeros();
    return *this;
}

BigNum& BigNum::operator*=(const BigNum &rhs)
{
    BigNum temp;
    vector<BigNum> products(10);
    products[0] = BigNum(0);
    products[1] = *this;
    for (int i = 2; i < 10; ++i)
    {
        products[i] = products[i - 1] + products[1];
    }
    *this = BigNum(0);
    vector<char>::const_reverse_iterator i;
    int j;
    for (i = rhs.sig.rbegin(), j = 0; i != rhs.sig.rend(); ++i, ++j)
    {
        temp = products[*i];
        for (int k = 0; k < j; ++k)
        {
            if (temp.exp >= 0)
            {
                temp.sig.push_back(0);
            }
            ++temp.exp;
        }
        *this += temp;
    }
    return *this;
}

BigNum& BigNum::operator/=(BigNum const &rhs)
{
    return *this;
}

BigNum BigNum::operator+() const
{
    return *this;
}

BigNum BigNum::operator-() const
{
    BigNum temp = *this;
    temp.neg = !temp.neg;
    return temp;
}

BigNum BigNum::operator+(const BigNum &rhs) const
{
    BigNum temp = *this;
    return temp += rhs;
}

BigNum BigNum::operator-(const BigNum &rhs) const
{
    BigNum temp = *this;
    return temp -= rhs;
}

BigNum BigNum::operator*(const BigNum &rhs) const
{
    BigNum temp = *this;
    return temp *= rhs;
}

BigNum BigNum::operator/(const BigNum &rhs) const
{
    BigNum temp = *this;
    return temp /= rhs;
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

void BigNum::removeZeros()
{
    while (floorDigits() > 1 && 0 == sig[0]) // Remove leading zeros
    {
        sig.erase(sig.begin());
    }
    while (fractDigits() > 1 && sig.back() == 0) // Remove trailing zeros
    {
        sig.pop_back();
        ++exp;
    }
}

int BigNum::floorDigits() const
{
    return sig.size() + exp;
}

int BigNum::fractDigits() const
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
