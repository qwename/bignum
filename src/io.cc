#include "bignum.h"
using std::cout;
using std::endl;

// Add significant digits, decimal places
void print(const BigNum &bn)
{
    printDebug("print");
    cout << toStr(bn) << endl;
}

string toStr(const BigNum &bn)
{
    string s = "";
    if (bn.neg)
    {
        s += "-";
    }
    vector<char>::const_reverse_iterator i, end;
    if (bn.floorSize() == 0)
    {   // 0 < abs(bn) < 1
        s += "0.";
        // Add leading zeros
        s.insert(s.end(), -bn.exp - bn.sig.size(), '0');
        i = bn.sig.rbegin(); end = bn.sig.rend();
        for (int j = 0; i != end && j < bn.precision; ++i, ++j)
        {
            s += *i + '0';
        }
    }
    else
    {   // bn == 0 or abs(bn) >= 1
        if (bn.exp >= 0)
        {   // Integer
            for (i = bn.sig.rbegin(); i != bn.sig.rend(); ++i)
            {
                s += *i + '0';
            }
            // Add trailing zeros
            s.insert(s.end(), bn.exp, '0');
        }
        else
        {   // Real number
            i = bn.sig.rbegin();
            end = i + bn.floorDigits();
            for (; i != end; ++i)
            {
                s += *i + '0';
            }
            if (i != bn.sig.rend())
            {
                s += '.';
                end = bn.sig.rend();
                for (int j = 0; i != end && j < bn.precision; ++i, ++j)
                {
                    s += *i + '0';
                }
            }
        }
    }
    return s;
}

ostream& operator<<(ostream &os, const BigNum &bn)
{
    os << toStr(bn);
    return os;
}

istream& operator>>(istream &is, BigNum &bn)
{
    string temp;
    is >> temp;
    bn = BigNum(temp);
    return is;
}

void printDebug(const string &s)
{
#ifdef DEBUG
    std::cout << "Debug: " << s << std::endl;
#endif
}

#ifdef DEBUG
static string const digits[] = { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9" };
string itoa(int i)
{
    if (0 == i)
    {
        return "0";
    }
    bool neg = i < 0;
    if (neg)
    {
        i = -i;
    }
    string s;
    while (i != 0)
    {
        s.insert(0, digits[i % 10]);
        i /= 10;
    }
    if (neg)
    {
        s.insert(0, "-");
    }
    return s;
}
#endif

string toStrDebug(const BigNum &bn)
{
    string s;
#ifdef DEBUG
    s += "Neg=";
    s += (bn.neg ? "true" : "false");
    s += " Exp=" + itoa(bn.exp);
    s += " Size=" + itoa(bn.sig.size()) + " ";
    for (int i = bn.sig.size() - 1; i >= 0; --i)
    {
        s += digits[bn.sig[i]];
    }
#endif
    return s;
}

