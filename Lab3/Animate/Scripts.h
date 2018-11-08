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
  Scripts(Scripts &&anotherScript);
  static Scripts loadScript(const char *filename);
  float getMaximumTime() const { return maximumTime; }
  const std::vector<float> &getTimestamps() const { return timestamps; }
  float getActivedTimestamp() const;
  void rearrangeKeyframes() {
    for_each(loadedFloatTimelines.begin(), loadedFloatTimelines.end(),
             [](auto &i) { i.second.rearrangeKeyframes(); });
    for_each(loadedQuaternionTimelines.begin(), loadedQuaternionTimelines.end(),
             [](auto &i) { i.second.rearrangeKeyframes(); });
  }
};

#endif  // ANIMATE_SCRIPTS_H