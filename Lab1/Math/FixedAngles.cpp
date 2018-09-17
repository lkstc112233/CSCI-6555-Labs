#include "FixedAngles.h"

#include <cmath>

FixedAngles::FixedAngles() : x(0), y(0), z(0) {}

FixedAngles::FixedAngles(float xi, float yi, float zi)
    : x(xi), y(yi), z(zi)
{
}

FixedAngles &FixedAngles::operator+=(const FixedAngles &op2)
{
    x += op2.x;
    y += op2.y;
    z += op2.z;
    return *this;
}
FixedAngles &FixedAngles::operator-=(const FixedAngles &op2)
{
    x -= op2.x;
    y -= op2.y;
    z -= op2.z;
    return *this;
}
FixedAngles &FixedAngles::operator*=(float op2)
{
    x *= op2;
    y *= op2;
    z *= op2;
    return *this;
}
FixedAngles &FixedAngles::operator/=(float op2)
{
    x /= op2;
    y /= op2;
    z /= op2;
    return *this;
}

FixedAngles FixedAngles::operator+(const FixedAngles &op2) const
{
    FixedAngles result(*this);
    return result += op2;
}
FixedAngles FixedAngles::operator-(const FixedAngles &op2) const
{
    FixedAngles result(*this);
    return result -= op2;
}
FixedAngles FixedAngles::operator*(float op2)const
{
    FixedAngles result(*this);
    return result *= op2;
}
FixedAngles operator*(float op1, const FixedAngles &op2) {
	FixedAngles result(op2);
	return result *= op1;
}
FixedAngles FixedAngles::operator/(float op2) const
{
    FixedAngles result(*this);
    return result /= op2;
}

// Heads up for myself: matrix in glm is stored in a column major way.
// So references to element happens as mat[col][row].
glm::mat3 FixedAngles::getRotationMatrix() {
	glm::mat3 result(1.0f);
    glm::mat3 rotationX(1.0f);
    rotationX[1][1] = cos(x);
    rotationX[1][2] = sin(x);
    rotationX[2][1] = -sin(x);
    rotationX[2][2] = cos(x);
    glm::mat3 rotationY(1.0f);
    rotationY[0][0] = cos(y);
    rotationY[0][2] = -sin(y);
    rotationY[2][0] = sin(y);
    rotationY[2][2] = cos(y);
    glm::mat3 rotationZ(1.0f);
    rotationZ[0][0] = cos(z);
    rotationZ[0][1] = sin(z);
    rotationZ[1][0] = -sin(z);
    rotationZ[1][1] = cos(z);
    // TODO: form these matrics together.
	return rotationZ * rotationY * rotationX * result;
}