#ifndef MATH_FIXEDANGLES_H
#define MATH_FIXEDANGLES_H

#include <glm/glm.hpp>

class FixedAngles
{
  private:
    float x;
    float y;
    float z;

  public:
    FixedAngles();
    FixedAngles(float x, float y, float z);

    FixedAngles &operator+=(const FixedAngles &);
    FixedAngles &operator-=(const FixedAngles &);
    FixedAngles &operator*=(float);
    FixedAngles &operator/=(float);

    FixedAngles operator+(const FixedAngles &) const;
    FixedAngles operator-(const FixedAngles &) const;
    FixedAngles operator*(float) const;
    FixedAngles operator/(float) const;

  	glm::mat3 getRotationMatrix();
};

FixedAngles operator*(float, const FixedAngles &);

#endif // MATH_FIXEDANGLES_H