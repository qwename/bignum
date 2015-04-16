#ifndef Q_BIGNUM_H
#define Q_BIGNUM_H

#include <vector>
using std::vector;
#include <string>
using std::string;
#include <iostream>
using std::ostream;
using std::istream;

class BigNum {
    public:
        BigNum();
        BigNum(const BigNum &rhs);
        BigNum(int n);
        BigNum(string n);
        ~BigNum();
        int getDigits() const;
        static void SetPrecision(int);
        friend void print(const BigNum &);
        friend string toStr(const BigNum &);
        friend BigNum Floor(const BigNum &);    // Integer digits
        friend BigNum Fract(const BigNum &);    // Fractional digits
        friend BigNum Abs(const BigNum &);      // Absolute value

        BigNum& operator=(const BigNum &rhs);

        // Comparison Operators
        bool operator==(const BigNum &rhs) const;
        bool operator!=(const BigNum &rhs) const;
        bool operator<(const BigNum &rhs) const;
        bool operator>(const BigNum &rhs) const;
        bool operator<=(const BigNum &rhs) const;
        bool operator>=(const BigNum &rhs) const;

        // Mutators
        BigNum& operator++();   // Pre-increment
        BigNum operator++(int); // Post-increment
        BigNum& operator--();   // Pre-decrement
        BigNum operator--(int); // Post-decrement
        BigNum& operator+=(const BigNum &rhs);
        BigNum& operator-=(const BigNum &rhs);
        BigNum& operator*=(const BigNum &rhs);
        BigNum& operator/=(const BigNum &rhs);
        BigNum& operator%=(const BigNum &rhs);

        BigNum operator+() const;
        BigNum operator-() const;
        BigNum operator+(const BigNum &rhs) const;
        BigNum operator-(const BigNum &rhs) const;
        BigNum operator*(const BigNum &rhs) const;
        BigNum operator/(const BigNum &rhs) const;
        BigNum operator%(const BigNum &rhs) const;

    private:
        void removeZeros();     // Remove leading and trailing zeros
        void alignDigits(const BigNum &);
        int floorDigits() const;
        int fractDigits() const;
        bool floorZero() const;
        void shiftLeft(unsigned);
        void shiftRight(unsigned);
        static int compareMagnitude(const BigNum &, const BigNum &);
        static void throwInvalidNumber(const string &);
        static string toStrDebug(const BigNum &);

        static const int DefaultBase;
        static const int DefaultPrecision;
        bool neg;               // Negative
        vector<char> sig;       // Significand
        int base;
        int exp;                // Exponent
        static int precision;
};

ostream& operator<<(ostream &os, const BigNum &bn);
istream& operator>>(istream &is, BigNum &bn);

#endif
