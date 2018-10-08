#include "Scripts.h"

#include <algorithm>
#include <iostream>
#include <memory>
#include <vector>

#include <glm/glm.hpp>

#include "../Math/Interpolate.hpp"
#include "../Math/Quaternion.h"
#include "../fileop/fileParser.h"
#include "Keyframe.h"

static bool operator<(const Keyframe &frame1, const Keyframe &frame2) {
  return frame1.getTimestamp() < frame2.getTimestamp();
}

static bool operator<(float timestamp, const Keyframe &frame) {
  return timestamp < frame.getTimestamp();
}

Scripts::Scripts(Scripts &&another)
    : activeKeyframeIndex(another.activeKeyframeIndex),
      useCatmullRomInterpolate(another.useCatmullRomInterpolate),
      timestamps(std::move(another.timestamps)),
      keyframes(std::move(another.keyframes)) {}

void Scripts::addKeyframe(const Keyframe keyframe) {
  keyframes.emplace(
      std::upper_bound(keyframes.begin(), keyframes.end(), keyframe), keyframe);
}

float Scripts::getMaximumTime() const {
  if (keyframes.size() < 2) {
    return 0;
  }
  return keyframes[keyframes.size() - 2].getTimestamp();
}

glm::mat4 Scripts::getTranscationMatrixAt(float time) {
  time = fmod(time, getMaximumTime());
  auto position =
      std::upper_bound(keyframes.begin(), keyframes.end(), time) - 1;

  time -= position->getTimestamp();
  time /= (position + 1)->getTimestamp() - position->getTimestamp();

  if (useCatmullRomInterpolate) {
    return catmullRomInterpolate(time, position - 1).getTranscationMatrix();
  } else {
    return uniformNonrationalBasisSplineInterpolate(time, position - 1)
        .getTranscationMatrix();
  }
}

float Scripts::getActivedTimestamp() const {
  return keyframes[getActivedKeyframe()].getTimestamp();
}

void Scripts::addKeyframeAt(float time) {
  // For now we add the keyframe at where it's supposed to be,
  // but no orientation, to reduce workload.
  auto transform = getTranscationMatrixAt(time);
  addKeyframe(Keyframe(time, transform[3][0], transform[3][1], transform[3][2],
                       Quaternion()));
  rearrangeKeyframes();
  activeKeyframeIndex =
      find(timestamps.begin(), timestamps.end(), time) - timestamps.begin();
}

void Scripts::removeKeyframeOf(int index) {
  keyframes.erase(keyframes.begin() + index);
  rebuildTimestampIndex();
  activeKeyframeIndex =
      std::min(activeKeyframeIndex, int(keyframes.size()) - 1);
}

void Scripts::setActiveTimestamp(float timestamp) {
  keyframes[getActivedKeyframe()].setTimestamp(timestamp);
  rebuildTimestampIndex();
}

void Scripts::moveActiveKeyframeBy(glm::vec3 movement) {
  keyframes[getActivedKeyframe()].moveX(movement.x);
  keyframes[getActivedKeyframe()].moveY(movement.y);
  keyframes[getActivedKeyframe()].moveZ(movement.z);
}

void Scripts::rotateActiveKeyframeBy(float yaw, float pitch, float roll) {
  keyframes[getActivedKeyframe()].rotateBy(yaw, pitch, roll);
}

void Scripts::rearrangeKeyframes() {
  std::sort(keyframes.begin(), keyframes.end(), [](auto &key1, auto &key2) {
    return key1.getTimestamp() < key2.getTimestamp();
  });
  rebuildTimestampIndex();
}

void Scripts::rebuildTimestampIndex() {
  timestamps.clear();
  timestamps.resize(keyframes.size());
  std::transform(keyframes.begin(), keyframes.end(), timestamps.begin(),
                 [](auto frame) { return frame.getTimestamp(); });
}

Scripts Scripts::loadScript(const char *filename) {
  Scripts loadedScript;
  FileParser parser(filename);

  // Load until end of control points
  while (parser.isValid()) {
    if (!parser.expect('(')) {
      break;
    }
    float data[8];
    int loaded = parser.tryParseFloat(data, 8);
    if (!parser.expect(')')) {
      std::cerr << "ScriptsLoader: Warning: Error happened parsing the script: "
                << filename << std::endl;
      break;
    }
    if (loaded == 8) {
      loadedScript.addKeyframe(
          Keyframe(data[0], data[1], data[2], data[3],
                   Quaternion(data[4], data[5], data[6], data[7])));
    } else {
      std::cerr << "ScriptsLoader: Warning: Error happened parsing the script: "
                << filename << std::endl;
      break;
    }
  }
  loadedScript.rebuildTimestampIndex();
  return loadedScript;
}