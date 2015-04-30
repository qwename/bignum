#include "bigfloat.h"
using std::cout;
using std::endl;

istream& operator>>(istream &is, BigFloat& bf)
{
    string temp;
    is >> temp;
    bf = BigFloat(temp);
    return is;
}

string BigFloat::toStr() const
{
    string s = "";
    if (neg)
    {
        s += "-";
    }
    vector<char>::const_reverse_iterator i, end;
    if (floorSize() == 0)
    {   // 0 < abs( < 1
        s += "0.";
        // Add leading zeros
        s.insert(s.end(), -exp - sig.size(), '0');
        i = sig.rbegin(); end = sig.rend();
        for (int j = 0; i != end && j < precision; ++i, ++j)
        {
            s += *i + '0';
        }
    }
    else
    {   // == 0 or abs( >= 1
        if (exp >= 0)
        {   // Integer
            for (i = sig.rbegin(); i != sig.rend(); ++i)
            {
                s += *i + '0';
            }
            // Add trailing zeros
            s.insert(s.end(), exp, '0');
        }
        else
        {   // Real number
            i = sig.rbegin();
            end = i + floorDigits();
            for (; i != end; ++i)
            {
                s += *i + '0';
            }
            if (i != sig.rend())
            {
                s += '.';
                end = sig.rend();
                for (int j = 0; i != end && j < precision; ++i, ++j)
                {
                    s += *i + '0';
                }
            }
        }
    }
    return s;
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

string toStrDebug(const BigFloat &bn)
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

