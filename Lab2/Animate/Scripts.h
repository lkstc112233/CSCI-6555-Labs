#ifndef ANIMATE_SCRIPTS_H
#define ANIMATE_SCRIPTS_H

#include <map>
#include <string>
#include <vector>

#include <glm/glm.hpp>

#include "../Math/Quaternion.h"
#include "Timeline.hpp"

/**
 * A class holding a movie script.
 */
class Scripts {
 private:
  Scripts() {}
  std::vector<float> timestamps;
  std::map<std::string, Timeline<float>> loadedFloatTimelines;
  std::map<std::string, Timeline<Quaternion>> loadedQuaternionTimelines;
  float maximumTime = 0;

 public:
  Timeline<float> getFloatTimeline(std::string name);
  Timeline<Quaternion> getQuaternionTimeline(std::string name);
  Timeline<float> xLine;
  Timeline<float> yLine;
  Timeline<float> zLine;
  Timeline<Quaternion> orientationLine;

 public:
  Scripts(Scripts &&anotherScript);
  static Scripts loadScript(const char *filename);
  float getMaximumTime() const { return maximumTime; }
  const std::vector<float> &getTimestamps() const { return timestamps; }
  float getXAt(float time) { return xLine.getDataAt(time); }
  float getYAt(float time) { return yLine.getDataAt(time); }
  float getZAt(float time) { return zLine.getDataAt(time); }
  Quaternion getOrientationAt(float time) {
    return orientationLine.getDataAt(time);
  }
  float getActivedTimestamp() const;
  void rearrangeKeyframes() {
    xLine.rearrangeKeyframes();
    yLine.rearrangeKeyframes();
    zLine.rearrangeKeyframes();
    orientationLine.rearrangeKeyframes();
  }
};

#endif  // ANIMATE_SCRIPTS_H