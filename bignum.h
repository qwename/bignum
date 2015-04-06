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
        BigNum(int n);
        BigNum(string n);
        ~BigNum();
        void print() const;
        int getDigits() const;

        BigNum& operator=(const BigNum &rhs);
        bool operator==(const BigNum &rhs) const;
        bool operator!=(const BigNum &rhs) const;
        bool operator<(const BigNum &rhs) const;
        bool operator>(const BigNum &rhs) const;
        bool operator<=(const BigNum &rhs) const;
        bool operator>=(const BigNum &rhs) const;
        BigNum& operator++();   // Pre-increment
        BigNum operator++(int); // Post-increment
        BigNum& operator--();   // Pre-decrement
        BigNum operator--(int);// Post-decrement
        BigNum& operator+=(const BigNum &rhs);
        BigNum& operator-=(const BigNum &rhs);
        BigNum operator+();
        BigNum operator-();
        BigNum operator+(const BigNum &rhs);
        BigNum operator-(const BigNum &rhs);
        BigNum operator*(const BigNum &rhs);
        BigNum operator/(const BigNum &rhs);

    private:
        bool isValidNumber(string n) const;
        static const unsigned int DefaultBase;
        inline int dp() const;  // Decimal places
        bool neg;               // Negative
        vector<char> sig;       // Significand
        int base;
        int exp;                // Exponent
};

#endif
