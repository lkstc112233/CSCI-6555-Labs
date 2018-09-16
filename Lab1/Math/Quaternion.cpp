#include "Quaternion.h"

#include <cmath>

Quaternion::Quaternion() : w(1), x(0), y(0), z(0) {}

Quaternion::Quaternion(float wi, float xi, float yi, float zi)
    : w(wi), x(xi), y(yi), z(zi)
{
    normalize();
}

Quaternion &Quaternion::operator+=(const Quaternion &op2)
{
    w += op2.w;
    x += op2.x;
    y += op2.y;
    z += op2.z;
    normalize();
    return *this;
}
Quaternion &Quaternion::operator-=(const Quaternion &op2)
{
    w -= op2.w;
    x -= op2.x;
    y -= op2.y;
    z -= op2.z;
    normalize();
    return *this;
}
Quaternion &Quaternion::operator*=(const Quaternion &op2)
{
    w = w * op2.w - x * op2.x - y * op2.y - z * op2.z;
    x = w * op2.x + x * op2.w + y * op2.z - z * op2.y;
    y = w * op2.y - x * op2.z + y * op2.w + z * op2.x;
    z = w * op2.z + x * op2.y - y * op2.x + z * op2.w;
    normalize();
    return *this;
}
Quaternion &Quaternion::operator*=(float op2)
{
    w *= op2;
    x *= op2;
    y *= op2;
    z *= op2;
    normalize();
    return *this;
}
Quaternion &Quaternion::operator/=(float op2)
{
    w /= op2;
    x /= op2;
    y /= op2;
    z /= op2;
    normalize();
    return *this;
}

Quaternion Quaternion::operator+(const Quaternion &op2) const
{
    Quaternion result(*this);
    return result += op2;
}
Quaternion Quaternion::operator-(const Quaternion &op2) const
{
    Quaternion result(*this);
    return result -= op2;
}
Quaternion Quaternion::operator*(const Quaternion &op2)const
{
    Quaternion result(*this);
    return result *= op2;
}
Quaternion Quaternion::operator*(float op2)const
{
    Quaternion result(*this);
    return result *= op2;
}
Quaternion Quaternion::operator/(float op2) const
{
    Quaternion result(*this);
    return result /= op2;
}

void Quaternion::normalize()
{
    float d = sqrt(w * w + x * x + y * y + z * z);
    w /= d;
    x /= d;
    y /= d;
    z /= d;
}
