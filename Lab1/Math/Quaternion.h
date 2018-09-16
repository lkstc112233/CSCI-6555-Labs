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

    void normalize();
};

#endif // MATH_QUATERNION_H