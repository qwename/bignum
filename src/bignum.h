#ifndef Q_BIGNUM_H
#define Q_BIGNUM_H

#include <string>
using std::string;
#include <iostream>
using std::ostream;
using std::istream;

class BigNum {
    public:
        BigNum();
        BigNum(int base);
        virtual ~BigNum();
        virtual int getDigits() const = 0;

        friend ostream& operator<<(ostream &os, const BigNum& bn);

    protected:
        virtual string toStr() const = 0;
        virtual bool equalZero() const = 0;
        // Check if number is power of the base
        // In representation, it is always 1, 10, 100,...
        virtual bool equalBasePower() const = 0;
        friend string toStrDebug(const BigNum &);

        static const int DefaultBase;
        int base;
};

#endif
