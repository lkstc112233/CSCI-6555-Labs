#ifndef ANIMATE_KEYFRAME_HPP
#define ANIMATE_KEYFRAME_HPP

#include <glm/glm.hpp>

/**
 * A template class for keyframe. It handles the data we interest.
 */
template <typename Interpolatable>
class Keyframe {
 private:
  float timestamp;
  Interpolatable data;

 public:
  Keyframe(float timestampi, const Interpolatable &datai)
      : timestamp(timestampi), data(datai) {}
  Keyframe(const Keyframe<Interpolatable> &another)
      : timestamp(another.timestamp), data(another.data) {}
  float getTimestamp() const { return timestamp; }
  void setTimestamp(float timestampi) { timestamp = timestampi; }
  void setData(Interpolatable datai) { datai = data; }
  Interpolatable getData() { return data; }
  // Math operations designed to ease interpolation.
  Keyframe operator+(const Keyframe &op2) const {
    Keyframe result(*this);
    result.data += op2.data;
    return result;
  }
  Keyframe operator*(float f) const {
    Keyframe result(*this);
    result.data *= f;
    return result;
  }
};

template <typename Interpolatable>
Keyframe<Interpolatable> operator*(float f,
                                   const Keyframe<Interpolatable> &key) {
  Keyframe<Interpolatable> result(key);
  return result * f;
}

#endif  // ANIMATE_KEYFRAME_HPP