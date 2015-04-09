#include "bignum.h"
#include <iostream>
#include <algorithm>

const int BigNum::DefaultBase = 10;
const int BigNum::DefaultPrecision = 20;
int BigNum::precision = DefaultPrecision;

BigNum::BigNum() : neg(false), sig(), base(DefaultBase), exp(0)
{
}

BigNum::BigNum(const BigNum &rhs) : neg(rhs.neg), sig(rhs.sig), base(DefaultBase), exp(rhs.exp) { }

BigNum::BigNum(int n) : neg(n < 0), sig(), base(DefaultBase), exp(0)
{
    if (0 == n)
    {
        return;
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
    if (n.size() < 1)
    {
        throwInvalidNumber(n);
    }
    bool decimal = false, valid = false;
    bool leading_zero = true, zero = true;
    char val = 0;
    string::const_iterator i = n.begin();
    if (neg)
    {
        ++i;
    }
    for (; i != n.end(); ++i)
    {
        if ('-' == *i)
        {
            throwInvalidNumber(n);
        }
        if ('.' == *i)
        {
            if (decimal)
            {
                throwInvalidNumber(n);
            }
            decimal = true;
            continue;
        }
        if (*i < '0' || '9' < *i)
        {
            throwInvalidNumber(n);
        }
        valid = true;
        val = *i - '0';
        if (!zero || val != 0)
        {
            zero = false;
            sig.push_back(val);
        }
        if (decimal)
        {
            --exp;
        }
        if (-exp + 1 > precision)
        {
            break;
        }
    }
    if (!valid)
    {
        throwInvalidNumber(n);
    }
    if (zero)
    {
        sig.clear();
        neg = false;
        exp = 0;
        return;
    }
    removeZeros();
}

BigNum::~BigNum() { }

// Add significant digits, decimal places
void print(const BigNum &bn)
{
    using std::cout;
    using std::endl;
    cout << toStr(bn) << endl;
}

string toStr(const BigNum &bn)
{
    if (bn.sig.size() == 0)
    {
        return "0";
    }
    string s = "";
    if (bn.neg)
    {
        s += "-";
    }
    vector<char>::const_iterator i;
    if (!bn.floorZero())
    {
        for (i = bn.sig.begin(); i != bn.sig.begin() + bn.floorDigits(); ++i)
        {
            s += *i + '0';
        }
    }
    if (bn.fractDigits() > 0)
    {
        s += '.';
        if (bn.floorZero())
        {
            s.append(-bn.exp - (int)bn.sig.size(), '0');
            i = bn.sig.begin();
        }
        else
        {
            i = bn.sig.begin() + bn.floorDigits();
        }
        for (; i != bn.sig.end(); ++i)
        {
            s += *i + '0';
        }
    }
    return s;
}

BigNum Floor(const BigNum &bn)
{
    BigNum temp;
    if (bn.floorZero())
    {
        return 0;
    }
    for (int i = 0; i < bn.floorDigits(); ++i)
    {
        temp.sig.push_back(bn.sig[i]);
    }
    temp.neg = bn.neg;
    temp.base = bn.base;
    temp.exp = 0;
    return temp;
}

BigNum Fract(const BigNum &bn)
{
    BigNum temp;
    int i;
    if (bn.floorZero())
    {
        i = 0;
    }
    else
    {
        i = bn.floorDigits();
    }
    if (i == bn.sig.size())
    {
        return 0;
    }
    for (; i < bn.sig.size(); ++i)
    {
        temp.sig.push_back(bn.sig[i]);
    }
    temp.neg = bn.neg;
    temp.base = bn.base;
    temp.exp = bn.exp;
    return temp;
}

BigNum Abs(const BigNum &bn)
{
    if (bn.neg)
    {
        return -bn;
    }
    return bn;
}

int BigNum::getDigits() const
{
    return floorDigits() + fractDigits();
}

void BigNum::SetPrecision(int prec)
{
    if (prec < 0)
    {
        prec = 0;
    }
    precision = prec;
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
    if (exp != rhs.exp)
    {
        return false;
    }
    return 0 == compareMagnitude(*this, rhs);
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
    if (neg)    // Both negative
    {
        return -rhs < -*this; // larger magnitude <=> smaller
    }
    int result = compareMagnitude(Floor(*this), Floor(rhs));
    if (0 == result)
    {
        return -1 == compareMagnitude(Fract(*this), Fract(rhs));
    }
    else
    {
        return -1 == result;
    }
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
    alignDigits(rhs);
    vector<char>::reverse_iterator i = sig.rbegin() + (fractDigits() - rhs.fractDigits());
    vector<char>::const_reverse_iterator j = rhs.sig.rbegin();
    vector<char>::reverse_iterator k;
    for (; j != rhs.sig.rend(); ++i, ++j)
    {
        temp = *i + *j;
        if (carry)  // Previous
        {
            ++temp;
        }
        *i = temp % base;
        carry = (temp >= base);
    }
    while (carry)
    {
        if (sig.rend() == i)
        {
            sig.insert(sig.begin(), 1);
            i = sig.rend();
            break;
        }
        ++(*i);
        carry = (*i >= base);
        *i %= base;
        ++i;
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
    if (*this < rhs)
    {
        *this = -(rhs - *this);
        return *this;
    }
    while (exp > rhs.exp)
    {   // Align decimal places
        sig.push_back(0);
        --exp;
    }
    char temp = 0;
    bool borrow = false;
    vector<char>::reverse_iterator i = sig.rbegin() + (fractDigits() - rhs.fractDigits());
    vector<char>::const_reverse_iterator j = rhs.sig.rbegin();
    vector<char>::reverse_iterator k;
    for (; j != rhs.sig.rend(); ++i, ++j)
    {
        temp = *i - *j;
        if (borrow)  // Previous
        {
            --temp;
        }
        borrow = (temp < 0);
        if (borrow)
        {
            *i = temp + base;
        }
        else
        {
            *i = temp;
        }
    }
    while (borrow)
    {
        if (sig.rend() == i)
        {
            break;
        }
        --(*i);
        borrow = (*i < 0);
        if (borrow)
        {
            *i += base;
        }
        ++i;
    }
    removeZeros();
    return *this;
}

BigNum& BigNum::operator*=(const BigNum &rhs)
{
    if (getDigits() < rhs.getDigits())
    {
        *this = rhs * *this;
        return *this;
    }
    if (1 == rhs.getDigits())
    {
        if (rhs == 0)
        {
            *this = 0;
            return *this;
        }
        if (rhs == 1)
        {
            return *this;
        }
        if (rhs == -1)
        {
            *this = -(*this);
            return *this;
        }
    }
    bool negative = (neg && !rhs.neg) || (!neg && rhs.neg);
    int exponent = exp + rhs.exp;
    exp = 0;
    vector<BigNum> products(base);
    products[1] = Abs(*this);
    for (int i = 2; i < base; ++i)
    {
        products[i] = products[i - 1] + products[1];
    }
    vector<char>::const_reverse_iterator i = rhs.sig.rbegin();
    *this = products[*i];
    int j;
    BigNum temp;
    for (++i, j = 1; i != rhs.sig.rend(); ++i, ++j)
    {
        if (0 == *i)
        {
            continue;
        }
        temp = products[*i];
        temp.shiftLeft(j);
        *this += temp;
    }
    neg = negative;
    exp = exponent;
    return *this;
}

BigNum& BigNum::operator/=(BigNum const &rhs)
{
    if (1 == rhs.getDigits())
    {
        if (rhs == 0)
        {
            throw string("Error: Divide by zero.");
        }
        if (rhs == 1)
        {
            return *this;
        }
        if (rhs == -1)
        {
            *this = -(*this);
            return *this;
        }
    }
    if (*this == 0)
    {
        return *this;
    }
    if (*this == rhs)
    {
        *this = 1;
        return *this;
    }
    if (*this == -rhs)
    {
        *this = -1;
        return *this;
    }
    bool negative = (neg && !rhs.neg) || (!neg && rhs.neg);
    if (neg)
    {
        neg = false;
    }
    int exponent = -rhs.exp;
    BigNum dividend = Abs(*this), divisor = Abs(rhs);
    BigNum quotient, remainder;
    divisor.exp = 0;
    divisor.removeZeros();
    dividend.exp = 0;
    dividend.removeZeros();
    exponent += exp - dividend.exp;
    vector<char>::const_iterator i = dividend.sig.begin();
    BigNum temp = *i;
    ++i;
    int prod = base - 1, prec = precision;
    vector<BigNum> products(divisor.base);
    products[1] = divisor;
    for (int i = 2; i < divisor.base; ++i)
    {
        products[i] = products[i - 1] + products[1];
    }
    while (-exponent <= precision)
    {
        if (temp == divisor)
        {
            prod = 1;
        }
        else if(temp < divisor)
        {
            prod = 0;
        }
        while (prod > 0)
        {
            remainder = temp - products[prod];
            if (remainder >= 0)
            {
                temp = remainder;
                break;
            }
            else
            {
                --prod;
            }
        }
        quotient.shiftLeft(1);
        quotient += prod;
        temp.shiftLeft(1);
        if (i != dividend.sig.end())
        {
            temp += *i;
            ++i;
        }
        else
        {
            if (temp == 0)
            {
                break;
            }
            if (-exponent == precision)
            {
                break;
            }
            --exponent;
            --prec;
        }
        if (temp == 0)
        {
            prod = 0;
        }
        else
        {
            prod = base - 1;
        }
    }
    *this = quotient;
    neg = negative;
    exp = exponent;
    removeZeros();
    return *this;
}

BigNum& BigNum::operator%=(const BigNum &rhs)
{
    if (Fract(*this) != 0)
    {
        throw string("Error: Non-integer ") + toStr(*this);
    }
    if (rhs.getDigits() == 1)
    {
        if (rhs == 0)
        {
            throw string("Error: Modulo 0");
        }
        if (Abs(rhs) == 1)
        {
            *this = 0;
            return *this;
        }
    }
    // Common residue, not minimal residue
    *this -= Floor(*this / rhs) * rhs;
    if (*this < 0)
    {
        *this += Abs(rhs);
    }
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

BigNum BigNum::operator%(const BigNum &rhs) const
{
    BigNum temp = *this;
    return temp %= rhs;
}

void BigNum::removeZeros()
{
    if (sig.size() == 0)
    {
        neg = false;
        exp = 0;
    }
    while (floorDigits() > 1 && 0 == sig[0]) // Remove leading zeros
    {
        sig.erase(sig.begin());
    }
    while (fractDigits() > 0 && sig.back() == 0) // Remove trailing zeros
    {
        sig.pop_back();
        ++exp;
    }
    while (-exp > precision)
    {
        sig.pop_back();
        ++exp;
    }
    if (sig.size() == 1 && sig[0] == 0)
    {
        sig.clear();
        neg = false;
        exp = 0;
    }
}

int BigNum::floorDigits() const
{
    if (floorZero())
    {
        return 1;
    }
    else
    {
        return sig.size() + exp;
    }
}

int BigNum::fractDigits() const
{
    return -exp;
}

bool BigNum::floorZero() const
{
    return -exp >= sig.size();
}

// Multiply by base
void BigNum::shiftLeft(unsigned x)
{
    if (*this == 0)
    {
        return;
    }
    x -= 0 - exp;
    while (x > 0)
    {
        sig.push_back(0);
        --x;
    }
    exp = x;
}

// Divide by base
void BigNum::shiftRight(unsigned x)
{
    if (*this == 0)
    {
        return;
    }
    exp -= x;
    int i = getDigits() - floorDigits();
    for (; i >= 0; --i)
    {
        sig.insert(sig.begin(), 0);
    }
}

void BigNum::alignDigits(const BigNum &bn)
{
    while (exp > bn.exp)
    {
        sig.push_back(0);
        --exp;
    }
    while (sig.size() < bn.sig.size())
    {
        sig.insert(sig.begin(), 0);
    }
}

int BigNum::compareMagnitude(const BigNum &bn1, const BigNum &bn2)
{   // return -1 if 1 < 2, 0 if 1 == 2, 1 if 1 > 2
    if (bn1.base != bn2.base)
    {
        throw string("Error: Base mismatch.");
    }
    if (bn1.sig.size() == 0 || bn2.sig.size() == 0)
    {
        if (bn1.sig.size() < bn2.sig.size())
        {
            return -1;
        }
        if (bn1.sig.size() > bn2.sig.size())
        {
            return 1;
        }
        return 0;
    }
    if (bn1.floorZero() || bn2.floorZero())
    {
        if (!bn2.floorZero())
        {
            return -1;
        }
        if (!bn1.floorZero())
        {
            return 1;
        }
        BigNum temp1 = bn1, temp2 = bn2;
        if (bn1.fractDigits() < bn2.fractDigits())
        {
            temp1.shiftLeft(-temp2.exp);
            temp2.shiftLeft(-temp2.exp);
        }
        else
        {
            temp2.shiftLeft(-temp1.exp);
            temp1.shiftLeft(-temp1.exp);
        }
        return compareMagnitude(temp1, temp2);
    }
    else
    {
        if (bn1.getDigits() < bn2.getDigits())
        {
            return -1;
        }
        if (bn1.getDigits() > bn2.getDigits())
        {
            return 1;
        }
    }
    vector<char>::const_iterator i, j;
    for (i = bn1.sig.begin(), j = bn2.sig.begin(); j != bn2.sig.end(); ++i, ++j)
    {
        if (i == bn1.sig.end())
        {
            return -1;
        }
        if (*i == *j)
        {
            continue;
        }
        if (*i < *j)
        {
            return -1;
        }
        if (*i > *j)
        {
            return 1;
        }
    }
    if (i != bn1.sig.end())
    {
        return 1;
    }
    return 0;
}

void BigNum::throwInvalidNumber(const string &n)
{
    throw string("Error: Invalid Number ") + n;
}

void BigNum::printDebug(const BigNum &bn)
{
    std::cout << "Debug: Exp=" << bn.exp 
              << " Size=" << bn.sig.size() << " ";
    for (int i = 0; i < bn.sig.size(); ++i)
    {
        std::cout << (int)bn.sig[i];
    }
    std::cout << std::endl;
}

