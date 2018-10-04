#include "Keyframe.h"

Keyframe::Keyframe(float timestampi, float xi, float yi, float zi, const Quaternion &orientationi)
    : timestamp(timestampi), x(xi), y(yi), z(zi), orientation(orientationi)
{
}

void Keyframe::setTimestamp(float timestampi)
{
    timestamp = timestampi;
}

void Keyframe::moveX(float dx)
{
    x += dx;
}

void Keyframe::moveY(float dy)
{
    y += dy;
}

void Keyframe::moveZ(float dz)
{
    z += dz;
}

// Rotates the keyframe by given angles.
void Keyframe::rotateBy(float yaw, float pitch, float roll)
{
    orientation.rotateBy(yaw, pitch, roll);
}

float Keyframe::getTimestamp() const { return timestamp; }

/**
 * Returns the transcation matrix of the keyframe. Including rotation, scale(Not implemented), 
 * and transcations.
 */
glm::mat4 Keyframe::getTranscationMatrix()
{
    glm::mat4 result(orientation.getRotationMatrix());
    result[3][0] = x;
    result[3][1] = y;
    result[3][2] = z;
    result[3][3] = 1.0F;
    return result;
}

// Math operations designed to ease interpolation.
Keyframe Keyframe::operator+(const Keyframe &op2) const
{
    Keyframe result(*this);
    result.x += op2.x;
    result.y += op2.y;
    result.z += op2.z;
    result.orientation += op2.orientation;
    return result;
}

Keyframe Keyframe::operator*(float f) const
{
    Keyframe result(*this);
    result.x *= f;
    result.y *= f;
    result.z *= f;
    result.orientation *= f;
    return result;
}

Keyframe operator*(float f, const Keyframe &key)
{
    Keyframe result(key);
    return result * f;
}
