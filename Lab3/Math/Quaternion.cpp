#include "Quaternion.h"

#include <cmath>

Quaternion::Quaternion() : w(1), x(0), y(0), z(0) {}
Quaternion::Quaternion(const Quaternion &another)
    : w(another.w), x(another.x), y(another.y), z(another.z) {}

Quaternion::Quaternion(float wi, float xi, float yi, float zi)
    : w(wi), x(xi), y(yi), z(zi) {
  normalize();
}

void Quaternion::rotateBy(float yaw, float pitch, float roll) {
  // EulerAngles rotator(*this);
  // rotator.yaw += yaw;
  // rotator.pitch += pitch;
  // rotator.roll += roll;
  // *this = Quaternion(rotator);
}

Quaternion &Quaternion::operator+=(const Quaternion &op2) {
  w += op2.w;
  x += op2.x;
  y += op2.y;
  z += op2.z;
  return *this;
}
Quaternion &Quaternion::operator-=(const Quaternion &op2) {
  w -= op2.w;
  x -= op2.x;
  y -= op2.y;
  z -= op2.z;
  return *this;
}
Quaternion &Quaternion::operator*=(const Quaternion &op2) {
  w = w * op2.w - x * op2.x - y * op2.y - z * op2.z;
  x = w * op2.x + x * op2.w + y * op2.z - z * op2.y;
  y = w * op2.y - x * op2.z + y * op2.w + z * op2.x;
  z = w * op2.z + x * op2.y - y * op2.x + z * op2.w;
  return *this;
}
Quaternion &Quaternion::operator*=(float op2) {
  w *= op2;
  x *= op2;
  y *= op2;
  z *= op2;
  return *this;
}
Quaternion &Quaternion::operator/=(float op2) {
  w /= op2;
  x /= op2;
  y /= op2;
  z /= op2;
  return *this;
}

Quaternion Quaternion::operator+(const Quaternion &op2) const {
  Quaternion result(*this);
  return result += op2;
}
Quaternion Quaternion::operator-(const Quaternion &op2) const {
  Quaternion result(*this);
  return result -= op2;
}
Quaternion Quaternion::operator*(const Quaternion &op2) const {
  Quaternion result(*this);
  return result *= op2;
}
Quaternion Quaternion::operator*(float op2) const {
  Quaternion result(*this);
  return result *= op2;
}
Quaternion operator*(float op1, const Quaternion &op2) {
  Quaternion result(op2);
  return result *= op1;
}
Quaternion Quaternion::operator/(float op2) const {
  Quaternion result(*this);
  return result /= op2;
}

Quaternion Quaternion::reciprocal() const { return Quaternion(w, -x, -y, -z); }

void Quaternion::normalize() {
  float d = sqrt(w * w + x * x + y * y + z * z);
  w /= d;
  x /= d;
  y /= d;
  z /= d;
}

// Heads up for myself: matrix in glm is stored in a column major way.
// So references to element happens as mat[col][row].
glm::mat3 Quaternion::getRotationMatrix() {
  // It's a good idea to keep the quaternion normalized.
  normalize();

  /* Use this magic trick I found from wikipedia to convert the quaternion to a
   * rotation matrix:
   *
   *  /  1 - 2*qy2 - 2*qz2	2*qx*qy - 2*qz*qw	2*qx*qz + 2*qy*qw  \
   *  |  2*qx*qy + 2*qz*qw	1 - 2*qx2 - 2*qz2	2*qy*qz - 2*qx*qw  |
   *  \  2*qx*qz - 2*qy*qw	2*qy*qz + 2*qx*qw	1 - 2*qx2 - 2*qy2  /
   */

  glm::mat3 result;
  result[0][0] = 1 - 2 * y * y - 2 * z * z;
  result[1][0] = 2 * x * y - 2 * z * w;
  result[2][0] = 2 * x * z + 2 * y * w;
  result[0][1] = 2 * x * y + 2 * z * w;
  result[1][1] = 1 - 2 * x * x - 2 * z * z;
  result[2][1] = 2 * y * z - 2 * x * w;
  result[0][2] = 2 * x * z - 2 * y * w;
  result[1][2] = 2 * y * z + 2 * x * w;
  result[2][2] = 1 - 2 * x * x - 2 * y * y;
  return result;
}