#include "bigfloat.h"

BigFloat BigFloat::operator%(const BigFloat &rhs) const
{
    printDebug(toStr() + " % " + rhs.toStr());
    BigFloat temp = *this;
    return temp %= rhs;
}

BigFloat& BigFloat::operator%=(const BigFloat &rhs)
{

}
