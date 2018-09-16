#ifndef MATH_QUATERNION_H
#define MATH_QUATERNION_H

class Quaternion
{
  private:
    float w;
    float x;
    float y;
    float z;

  public:
    Quaternion();
    Quaternion(float w, float x, float y, float z);

    Quaternion &operator+=(const Quaternion &);
    Quaternion &operator-=(const Quaternion &);
    Quaternion &operator*=(const Quaternion &);
    Quaternion &operator*=(float);
    Quaternion &operator/=(float);

    Quaternion operator+(const Quaternion &) const;
    Quaternion operator-(const Quaternion &) const;
    Quaternion operator*(const Quaternion &) const;
    Quaternion operator*(float) const;
    Quaternion operator/(float) const;

    void normalize();
};

#endif // MATH_QUATERNION_H