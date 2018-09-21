#ifndef ANIMATE_KEYFRAME_HPP
#define ANIMATE_KEYFRAME_HPP

#include <glm/glm.hpp>

template <typename Rotation>
class Keyframe
{
private:
  float timestamp;
  float x;
  float y;
  float z;
  Rotation orientation;

public:
  Keyframe(float timestampi, float xi, float yi, float zi, const Rotation &orientationi)
      : timestamp(timestampi), x(xi), y(yi), z(zi), orientation(orientationi)
  {
  }
  float getTimestamp() { return timestamp; }
  glm::mat4 getTranscationMatrix()
  {
    glm::mat4 result(orientation.getRotationMatrix());
    result[3][0] = x;
    result[3][1] = y;
    result[3][2] = z;
    result[3][3] = 1.0f;
    return result;
  }
  Keyframe<Rotation> operator+(const Keyframe<Rotation>& op2) const {
    Keyframe<Rotation> result(*this);
    result.x += op2.x;
    result.y += op2.y;
    result.z += op2.z;
    result.orientation += op2.orientation;
    return result; 
  }

  Keyframe<Rotation> operator*(float f) const {
    Keyframe<Rotation> result(*this);
    result.x *= f;
    result.y *= f;
    result.z *= f;
    result.orientation *= f;
    return result; 
  }
};

template<typename T>
Keyframe<T> operator*(float f, const Keyframe<T>& key) {
  Keyframe<T> result(key);
  return result * f;
}

#endif // ANIMATE_KEYFRAME_HPP