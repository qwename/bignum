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
        friend void print(const BigNum &);
        int getDigits() const;
        friend string toStr(const BigNum &);
        friend BigNum Floor(const BigNum &);    // Integer digits
        friend BigNum Fract(const BigNum &);    // Fractional digits
        friend BigNum Abs(const BigNum &);      // Absolute value

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
        BigNum operator--(int); // Post-decrement
        BigNum& operator+=(const BigNum &rhs);
        BigNum& operator-=(const BigNum &rhs);
        BigNum& operator*=(const BigNum &rhs);
        BigNum& operator/=(const BigNum &rhs);
        BigNum operator+() const;
        BigNum operator-() const;
        BigNum operator+(const BigNum &rhs) const;
        BigNum operator-(const BigNum &rhs) const;
        BigNum operator*(const BigNum &rhs) const;
        BigNum operator/(const BigNum &rhs) const;

    private:
        bool isValidNumber(const string n) const;
        void removeZeros();     // Remove leading and trailing zeros
        int floorDigits() const;
        int fractDigits() const;

        static const unsigned int DefaultBase;
        bool neg;               // Negative
        vector<char> sig;       // Significand
        int base;
        int exp;                // Exponent
};

#endif
