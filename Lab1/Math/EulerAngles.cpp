#include "EulerAngles.h"

#include <cmath>

#include "Quaternion.h"

EulerAngles::EulerAngles() : yaw(0), pitch(0), roll(0) {}

// Convertation from Quaternion to EulerAngles.
EulerAngles::EulerAngles(const Quaternion &quat)
{
    auto q = quat;
    q.normalize();
    // For some reason asin(1) returns nan. I will check that explictly here.
    float sinroll = 2 * q.x * q.y + 2 * q.z * q.w;
    roll = asin(sinroll);
    if (std::isnan(roll)) {
        roll = sinroll > 0 ? M_PI_2 : -M_PI_2;
    }
    if (q.x * q.y + q.z * q.w == 0.5)
    {
        // North pole
        yaw = 2 * atan2(q.x, q.w);
        pitch = 0;
    }
    else if (q.x * q.y + q.z * q.w == -0.5)
    {
        // South pole
        yaw = -2 * atan2(q.x, q.w);
        pitch = 0;
    }
    else
    {
        yaw = atan2(2 * q.y * q.w - 2 * q.x * q.z, 1 - 2 * q.y * q.y - 2 * q.z * q.z);
        pitch = atan2(2 * q.x * q.w - 2 * q.y * q.z, 1 - 2 * q.x * q.x - 2 * q.z * q.z);
    }
}

EulerAngles::EulerAngles(float yawi, float pitchi, float rolli)
    : yaw(yawi), pitch(pitchi), roll(rolli)
{
}

void EulerAngles::rotateBy(float yawi, float pitchi, float rolli) {
    yaw += yawi;
    pitch += pitchi;
    roll += rolli;
}

EulerAngles &EulerAngles::operator+=(const EulerAngles &op2)
{
    yaw += op2.yaw;
    pitch += op2.pitch;
    roll += op2.roll;
    return *this;
}
EulerAngles &EulerAngles::operator-=(const EulerAngles &op2)
{
    yaw -= op2.yaw;
    pitch -= op2.pitch;
    roll -= op2.roll;
    return *this;
}
EulerAngles &EulerAngles::operator*=(float op2)
{
    yaw *= op2;
    pitch *= op2;
    roll *= op2;
    return *this;
}
EulerAngles &EulerAngles::operator/=(float op2)
{
    yaw /= op2;
    pitch /= op2;
    roll /= op2;
    return *this;
}

EulerAngles EulerAngles::operator+(const EulerAngles &op2) const
{
    EulerAngles result(*this);
    return result += op2;
}
EulerAngles EulerAngles::operator-(const EulerAngles &op2) const
{
    EulerAngles result(*this);
    return result -= op2;
}
EulerAngles EulerAngles::operator*(float op2) const
{
    EulerAngles result(*this);
    return result *= op2;
}
EulerAngles operator*(float op1, const EulerAngles &op2)
{
    EulerAngles result(op2);
    return result *= op1;
}
EulerAngles EulerAngles::operator/(float op2) const
{
    EulerAngles result(*this);
    return result /= op2;
}

// Heads up for myself: matrix in glm is stored in a column major way.
// So references to element happens as mat[col][row].
glm::mat3 EulerAngles::getRotationMatrix()
{
    glm::mat3 result(1.0f);
    glm::mat3 rotationX(1.0f);
    rotationX[1][1] = cos(pitch);
    rotationX[1][2] = sin(pitch);
    rotationX[2][1] = -sin(pitch);
    rotationX[2][2] = cos(pitch);
    glm::mat3 rotationY(1.0f);
    rotationY[0][0] = cos(yaw);
    rotationY[0][2] = -sin(yaw);
    rotationY[2][0] = sin(yaw);
    rotationY[2][2] = cos(yaw);
    glm::mat3 rotationZ(1.0f);
    rotationZ[0][0] = cos(roll);
    rotationZ[0][1] = sin(roll);
    rotationZ[1][0] = -sin(roll);
    rotationZ[1][1] = cos(roll);
    // TODO: form these matrics together.
    return rotationY * rotationZ * rotationX * result;
}