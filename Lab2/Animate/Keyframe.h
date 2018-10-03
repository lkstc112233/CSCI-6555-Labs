#ifndef ANIMATE_KEYFRAME_HPP
#define ANIMATE_KEYFRAME_HPP

#include <glm/glm.hpp>
#include "../Math/Quaternion.h"

/** 
 * A template class for keyframe. It handles the data we interest in in one place. 
 */
class Keyframe
{
private:
  float timestamp;
  float x;
  float y;
  float z;
  Quaternion orientation;

public:
  Keyframe(float timestampi, float xi, float yi, float zi, const Quaternion &orientationi);
  void setTimestamp(float timestampi);
  void moveX(float dx);
  void moveY(float dy);
  void moveZ(float dz);
  // Rotates the keyframe by given angles. 
  void rotateBy(float yaw, float pitch, float roll);
  float getTimestamp() const;
  /**
   * Returns the transcation matrix of the keyframe. Including rotation, scale(Not implemented), 
   * and transcations.
   */ 
  glm::mat4 getTranscationMatrix();
  // Math operations designed to ease interpolation.
  Keyframe operator+(const Keyframe& op2) const;
  Keyframe operator*(float f) const;
};

Keyframe operator*(float f, const Keyframe& key);

#endif // ANIMATE_KEYFRAME_HPP