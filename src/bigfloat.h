#ifndef Q_BIGFLOAT_H
#define Q_BIGFLOAT_H

#include "bignum.h"
#include <vector>
using std::vector;

class BigFloat : public BigNum {
    public:
        BigFloat();
        BigFloat(const BigFloat &rhs);
        BigFloat(int n);
        BigFloat(string n);
        ~BigFloat();
        int getDigits() const;
        static void SetPrecision(int);
        friend BigFloat Floor(const BigFloat &);    // Integer digits
        friend BigFloat Fract(const BigFloat &);    // Fractional digits
        friend BigFloat Abs(const BigFloat &);      // Absolute value

        BigFloat& operator=(const BigFloat &rhs);

        // Comparison Operators
        bool operator==(const BigFloat &rhs) const;
        bool operator!=(const BigFloat &rhs) const;
        bool operator<(const BigFloat &rhs) const;
        bool operator>(const BigFloat &rhs) const;
        bool operator<=(const BigFloat &rhs) const;
        bool operator>=(const BigFloat &rhs) const;

        // Mutators
        BigFloat& operator++();   // Pre-increment
        BigFloat operator++(int); // Post-increment
        BigFloat& operator--();   // Pre-decrement
        BigFloat operator--(int); // Post-decrement
        BigFloat& operator+=(const BigFloat &rhs);
        BigFloat& operator-=(const BigFloat &rhs);
        BigFloat& operator*=(const BigFloat &rhs);
        BigFloat& operator/=(const BigFloat &rhs);
        BigFloat& operator%=(const BigFloat &rhs);

        BigFloat operator+() const;
        BigFloat operator-() const;
        BigFloat operator+(const BigFloat &rhs) const;
        BigFloat operator-(const BigFloat &rhs) const;
        BigFloat operator*(const BigFloat &rhs) const;
        BigFloat operator/(const BigFloat &rhs) const;
        BigFloat operator%(const BigFloat &rhs) const;

    protected:
        string toStr() const;
        bool equalZero() const;
        // Check if number is power of the base
        // In representation, it is always 1, 10, 100,...
        bool equalBasePower() const;
        void removeZeros();     // Remove leading and trailing zeros
        // Number of digits in representation
        int floorDigits() const;
        int fractDigits() const;
        // Actual number of bytes stored
        int floorSize() const;
        int fractSize() const;
        static void throwInvalidNumber(const string &);
        friend string toStrDebug(const BigFloat &);

        static const int DefaultPrecision;
        static int precision;
        bool neg;               // Negative
        vector<char> sig;       // Significand
        int exp;                // Exponent
};

void printDebug(const string &s);
istream& operator>>(istream &, BigFloat&);

#endif
