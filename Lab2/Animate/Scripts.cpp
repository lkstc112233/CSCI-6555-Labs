#include "Scripts.h"

#include <algorithm>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include <glm/glm.hpp>

#include "../Math/Interpolate.hpp"
#include "../Math/Quaternion.h"
#include "../fileop/fileParser.h"
#include "Keyframe.hpp"
#include "Timeline.hpp"

Scripts::Scripts(Scripts &&another)
    : timestamps(std::move(another.timestamps)),
      loadedFloatTimelines(std::move(another.loadedFloatTimelines)),
      loadedQuaternionTimelines(std::move(another.loadedQuaternionTimelines)),
      xLine(std::move(another.xLine)),
      yLine(std::move(another.yLine)),
      zLine(std::move(another.zLine)),
      orientationLine(std::move(another.orientationLine)) {}

Timeline<float> Scripts::getFloatTimeline(std::string name) {
  return loadedFloatTimelines[name];
}

Timeline<Quaternion> Scripts::getQuaternionTimeline(std::string name) {
  return loadedQuaternionTimelines[name];
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
      loadedScript.xLine.addKeyframe(Keyframe<float>(data[0], data[1]));
      loadedScript.yLine.addKeyframe(Keyframe<float>(data[0], data[2]));
      loadedScript.zLine.addKeyframe(Keyframe<float>(data[0], data[3]));
      loadedScript.orientationLine.addKeyframe(Keyframe<Quaternion>(
          data[0], Quaternion(data[4], data[5], data[6], data[7])));
    } else {
      std::cerr << "ScriptsLoader: Warning: Error happened parsing the script: "
                << filename << std::endl;
      break;
    }
  }
  loadedScript.rearrangeKeyframes();
  loadedScript.maximumTime = std::max(
      {loadedScript.xLine.getMaximumTime(), loadedScript.yLine.getMaximumTime(),
       loadedScript.zLine.getMaximumTime(),
       loadedScript.orientationLine.getMaximumTime()});
  return loadedScript;
}