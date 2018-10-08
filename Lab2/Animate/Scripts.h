#ifndef ANIMATE_SCRIPTS_H
#define ANIMATE_SCRIPTS_H

#include <vector>

#include <glm/glm.hpp>

#include "Keyframe.h"

/**
 * A class holding a movie script.
 */
class Scripts {
 private:
  Scripts() {}
  int activeKeyframeIndex = 0;
  bool useCatmullRomInterpolate = true;
  std::vector<float> timestamps;
  std::vector<Keyframe> keyframes;
  // Function used by the loader to add a keyframe to the script.
  void addKeyframe(const Keyframe keyframe);
  void rebuildTimestampIndex();

 public:
  Scripts(Scripts &&anotherScript);
  static Scripts loadScript(const char *filename);
  float getMaximumTime() const;
  float getMaximumId() const { return keyframes.size(); }
  const std::vector<float> &getTimestamps() const { return timestamps; }
  glm::mat4 getTranscationMatrixAt(float time);
  glm::mat4 getTranscationMatrixOf(int i) {
    return keyframes[i].getTranscationMatrix();
  }
  void activeKeyframe(int index) { activeKeyframeIndex = index; }
  int getActivedKeyframe() const { return activeKeyframeIndex; }
  float getActivedTimestamp() const;
  void addKeyframeAt(float time);
  void removeKeyframeOf(int index);
  void setActiveTimestamp(float timestamp);
  void moveActiveKeyframeBy(glm::vec3);
  void rotateActiveKeyframeBy(float yaw, float pitch, float roll);
  void rearrangeKeyframes();
  void switchInterpolate() {
    useCatmullRomInterpolate = !useCatmullRomInterpolate;
  }
};

#endif  // ANIMATE_SCRIPTS_H