#ifndef Q_BIGNUM_H
#define Q_BIGNUM_H

#include <vector>
using std::vector;
#include <string>
using std::string;

class BigNum {
    public:
        BigNum();
        BigNum(const BigNum &rhs);
        BigNum& operator=(const BigNum &rhs);
        BigNum(int n);
        BigNum(string n);
        ~BigNum();
        void print() const;

        BigNum& operator++();
        BigNum operator++(int);
        const BigNum& operator--();
        const BigNum& operator--(int);
        BigNum& operator+=(const BigNum &rhs);
        BigNum& operator-=(const BigNum &rhs);
        BigNum operator-();
        BigNum operator+(const BigNum &rhs);
        BigNum operator-(const BigNum &rhs);
        BigNum operator*(const BigNum &rhs);
        BigNum operator/(const BigNum &rhs);

    private:
        bool isValidNumber(string n) const;
        inline int dp() const;  // Decimal places
        bool negative;
        vector<char> sig;       // Significand
        const static int base;
        int exp;                // Exponent
};

#endif
