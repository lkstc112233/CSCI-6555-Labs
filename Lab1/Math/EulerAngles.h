#ifndef MATH_EULERANGLES_H
#define MATH_EULERANGLES_H

#include <glm/glm.hpp>

#include "Quaternion.h"

class Quaternion;
class EulerAngles
{
  private:
    friend class Quaternion;
    float yaw;
    float pitch;
    float roll;

  public:
    EulerAngles();
    EulerAngles(const Quaternion& quat);
    EulerAngles(float yaw, float pitch, float roll);

    EulerAngles &operator+=(const EulerAngles &);
    EulerAngles &operator-=(const EulerAngles &);
    EulerAngles &operator*=(float);
    EulerAngles &operator/=(float);

    EulerAngles operator+(const EulerAngles &) const;
    EulerAngles operator-(const EulerAngles &) const;
    EulerAngles operator*(float) const;
    EulerAngles operator/(float) const;

  	glm::mat3 getRotationMatrix();
};

EulerAngles operator*(float, const EulerAngles &);

#endif // MATH_EULERANGLES_H