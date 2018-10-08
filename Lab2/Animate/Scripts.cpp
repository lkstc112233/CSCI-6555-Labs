#include "Scripts.h"

#include <algorithm>
#include <iostream>
#include <memory>
#include <vector>

#include <glm/glm.hpp>

#include "../Math/Interpolate.hpp"
#include "../Math/Quaternion.h"
#include "../fileop/fileParser.h"
#include "Keyframe.hpp"

Scripts::Scripts(Scripts &&another)
    : timestamps(std::move(another.timestamps)),
      xLine(std::move(another.xLine)),
      yLine(std::move(another.yLine)),
      zLine(std::move(another.zLine)),
      orientationLine(std::move(another.orientationLine)) {}

float Scripts::getMaximumTime() const {
  return std::max({xLine.getMaximumTime(), yLine.getMaximumTime(),
                   zLine.getMaximumTime(), orientationLine.getMaximumTime()});
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
  return loadedScript;
}