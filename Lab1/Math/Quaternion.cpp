#include "Quaternion.h"

#include <cmath>

Quaternion::Quaternion() : w(1), x(0), y(0), z(0) {}

Quaternion::Quaternion(float wi, float xi, float yi, float zi)
    : w(wi), x(xi), y(yi), z(zi)
{
    normalize();
}

void Quaternion::normalize()
{
    float d = sqrt(w * w + x * x + y * y + z * z);
    w /= d;
    x /= d;
    y /= d;
    z /= d;
}
