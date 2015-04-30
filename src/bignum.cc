#include "bignum.h"

const int BigNum::DefaultBase = 10;

BigNum::BigNum() : base(DefaultBase) { }

BigNum::BigNum(int base) : base(base) { }

BigNum::~BigNum() { }

ostream& operator<<(ostream &os, const BigNum& bn)
{
    os << bn.toStr();
    return os;
}
