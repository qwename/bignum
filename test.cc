#include "src/bigfloat.h"
#include <ctime>
using std::clock_t;
#include <iomanip>
using std::cout;
using std::cerr;
using std::cin;
using std::endl;
#include <array>
using std::array;

void printTest(ostream &os, BigFloat a, BigFloat b, string op)
{
    os << a << " " << op << " " << b << " = ";
    if ("+" == op)
    {
        os << a + b;
    }
    else if("-" == op)
    {
        os << a - b;
    }
    else if("*" == op)
    {
        os << a * b;
    }
    else if("/" == op)
    {
        os << a / b;
    }
    os << endl;
}

void testOps(ostream &os, BigFloat a, BigFloat b)
{
    static const array<string, 4> ops = { "+", "-", "*", "/" };
    for (array<string, 4>::const_iterator i = ops.begin(); i != ops.end(); ++i)
    {
        printTest(os, a, b, *i);
    }
}

void test()
{
    cout << "Enter two numbers: \n";
    string a, b;
    while (true)
    {
        cin >> a;
        if (cin.eof() || "quit" == a)
        {
            break;
        }
        try
        {
            BigFloat x(a);
            while (true)
            {
                cin >> b;
                if (cin.eof() || "quit" == b)
                {
                    break;
                }
                try
                {
                    BigFloat y(b);
                    testOps(cout, x, y);
                    break;
                }
                catch (string e)
                {
                    cerr << e << endl;
                }
            }
        }
        catch (string e)
        {
            cerr << e << endl;
        }
    }
}

BigFloat fact(BigFloat x)
{
    if (x == 0)
    {
        return 1;
    }
    BigFloat val = x;
    while (x > 1)
    {
        --x;
        val *= x;
    }
    return val;
}

BigFloat sum(BigFloat x)
{
    if (x <= 0)
    {
        return 0;
    }
    BigFloat val = x;
    while (x > 0)
    {
        --x;
        val += x;
    }
    return val;
}

vector<BigFloat> primes;
bool isPrime(const BigFloat &x)
{
    for (vector<BigFloat>::const_iterator j = primes.begin() + 1; j != primes.end() && *j * *j <= x; ++j)
    {
        if (x % *j == 0)
        {
            return false;
        }
    }
    primes.push_back(x);
    return true;
}

vector<unsigned> primes_i;
bool isPrime(const unsigned &x)
{
    for (vector<unsigned>::const_iterator j = primes_i.begin() + 1; j != primes_i.end() && *j * *j <= x; ++j)
    {
        if (x % *j == 0)
        {
            return false;
        }
    }
    primes_i.push_back(x);
    return true;
}

void printTime(std::clock_t i, std::clock_t f)
{
    std::cout << std::fixed << std::setprecision(2) 
              << 1000.0 * (f - i) / CLOCKS_PER_SEC
              << " ms" << std::endl;
}

int main(int argc, char *argv[])
{
    string s1;
    vector<BigFloat> v;
    cout << "Reading inputs while testing Floor(), Fract() functions, "
         << "press Ctrl-D (EOF) to stop" << endl
         << "Format: [Input] Digits: [# of digits]" << endl
         << "[Floor] [Fract]" << endl
         << "[Floor(Floor)] [Fract(Floor)]" << endl
         << "[Floor(Fract)] [Fract(Fract)]" << endl;
    while (cin >> s1)
    {
        try
        {
            BigFloat temp(s1);
            v.push_back(temp);
            BigFloat fl(Floor(temp)), fr(Fract(temp));
            cout << temp << " Digits: " << temp.getDigits() << endl;
            cout << fl << " " << fr << endl
                 << Floor(fl) 
                 << " " << Fract(fl) << endl
                 << Floor(fr) 
                 << " " << Fract(fr) << endl;
        }
        catch (string s)
        {
            cout << s << endl;
        }
    }
    cout << "Now testing comparison operators ==, <, "
         << "only prints if true" << endl;
    for (unsigned i = 0; i < v.size(); ++i)
    {
        for (unsigned j = 0; j < v.size(); ++j)
        {
            bool equal = v[i] == v[j];
            bool less = v[i] < v[j];
            if (equal)
            {
                cout << v[i] << " == " << v[j] << endl;
            }
            if (less)
            {
                cout << v[i] << " < " << v[j] << endl;
            }
        }
    }
    cout << "Now testing arithmetic operators" << endl;
    for (unsigned i = 0; i < v.size(); ++i)
    {
        for (unsigned j = 0; j < v.size(); ++j)
        {
            cout << v[i] << " + " << v[j] << " = " 
                 << v[i] + v[j] << endl;
            cout << v[i] << " - " << v[j] << " = " 
                 << v[i] - v[j] << endl;
            cout << v[i] << " * " << v[j] << " = " 
                 << v[i] * v[j] << endl;
        }
    }
    try
    {
        BigFloat max = 100;
        if (argc == 2)
        {
            max = BigFloat(argv[1]);
        }
        clock_t begin, finish;
        begin = clock();

        BigFloat x, y;
        begin = clock();
        y = sum(max);
        finish = clock();
        cout << y << endl;
        printTime(begin, finish);
        cout << "Summation from 1 to " << max << " has " << y.getDigits() << " digits." << endl;

        begin = clock();
        // while ((clock()-begin)/CLOCKS_PER_SEC < 5)
        x = fact(max);
        finish = clock();
        cout << x << endl;
        printTime(begin, finish);
        cout << max << "! has " << x.getDigits() << " digits." << endl;
        return 0;

        const int loops = 100000;
        const BigFloat loop = loops;
        for (BigFloat i = 0; i < loop; ++i)
        {

        }
        finish = clock();
        printTime(begin, finish);
        begin = clock();
        for (int i = 0; i < loops; ++i)
        {

        }
        finish = clock();
        printTime(begin, finish);

        primes_i.push_back(2);
        begin = clock();
        for (unsigned i = 3; max > primes_i.size(); i += 2)
        {
            isPrime(i);
        }
        finish = clock();
        printTime(begin, finish);
        cout << primes_i.back() << std::endl;

        primes.push_back(2);
        begin = clock();
        for (BigFloat i = 3; max > primes.size(); i += 2)
        {
            isPrime(i);
        }
        finish = clock();
        printTime(begin, finish);
        cout << primes.back() << endl;

        return 0;
    }
    catch (string e)
    {
        cerr << e << endl;
    }
    return 0;
    test();
    return 0;
}
