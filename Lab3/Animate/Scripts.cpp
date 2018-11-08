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
      loadedQuaternionTimelines(std::move(another.loadedQuaternionTimelines)) {}

Timeline<float> Scripts::getFloatTimeline(std::string name) {
  return loadedFloatTimelines[name];
}

Timeline<Quaternion> Scripts::getQuaternionTimeline(std::string name) {
  return loadedQuaternionTimelines[name];
}

template <typename T>
static constexpr int loadCount() {
  return 1;
}

template <>
constexpr int loadCount<Quaternion>() {
  return 4;
}

template <typename T>
static T readType(float *pointer) {
  return T();
}

template <>
float readType<float>(float *pointer) {
  return *pointer;
}
template <>
Quaternion readType<Quaternion>(float *pointer) {
  return Quaternion(pointer[0], pointer[1], pointer[2], pointer[3]);
}

// Loads a script.
template <typename T>
static std::vector<Timeline<T>> loadTimeline(FileParser &parser,
                                             int count = 1) {
  int floatingCount = count * loadCount<T>() + 1;
  std::vector<Timeline<T>> result(count);

  // Load until end of control points
  while (parser.isValid()) {
    if (!parser.expect('(')) {
      break;
    }
    float data[floatingCount];
    int loaded = parser.tryParseFloat(data, floatingCount);
    if (!parser.expect(')')) {
      std::cerr << "ScriptsLoader: Warning: Error happened parsing the script."
                << std::endl
                << "\t:Missing )" << std::endl;
      break;
    }
    if (loaded == floatingCount) {
      float timestamp = data[0];
      auto pointer = data + 1;
      for (int i = 0; i < count; ++i) {
        result[i].addKeyframe(Keyframe<T>(timestamp, readType<T>(pointer)));
        pointer += loadCount<T>();
      }
    } else {
      std::cerr << "ScriptsLoader: Warning: Error happened parsing the script."
                << std::endl
                << "\t:Dismatching count: expecting " << floatingCount
                << std::endl;
      break;
    }
  }
  return result;
}

Scripts Scripts::loadScript(const char *filename) {
  Scripts loadedScript;
  FileParser parser(filename);

  // @ command parameters
  std::string name;
  char type;
  int defaultCount = 0;

  // Expect an @ command
  while (parser.isValid()) {
    while (parser.expect('@')) {
      auto command = parser.parseString();
      if (command == "name") {
        name = parser.parseString();
      } else if (command == "type") {
        type = parser.parseString()[0];
      }
    }
    if (name == "") {
      name = "default_" + std::to_string(defaultCount++);
    }
    switch (type) {
      case 'f':
        loadedScript.loadedFloatTimelines.emplace(
            name, std::move(loadTimeline<float>(parser)[0]));
        break;
      case 'p': {
        auto timeline = loadTimeline<float>(parser, 3);
        loadedScript.loadedFloatTimelines.emplace("x" + name,
                                                  std::move(timeline[0]));
        loadedScript.loadedFloatTimelines.emplace("y" + name,
                                                  std::move(timeline[1]));
        loadedScript.loadedFloatTimelines.emplace("z" + name,
                                                  std::move(timeline[2]));
      } break;
      case 'q':
        loadedScript.loadedQuaternionTimelines.emplace(
            name, std::move(loadTimeline<Quaternion>(parser)[0]));
        break;
      default:
        std::cerr
            << "ScriptsLoader: Warning: Error happened parsing the script."
            << std::endl
            << "\t:Unexpected type" << std::endl;
        break;
    }
    name = "";
  }

  loadedScript.rearrangeKeyframes();
  loadedScript.maximumTime =
      std::max({std::max_element(loadedScript.loadedFloatTimelines.begin(),
                                 loadedScript.loadedFloatTimelines.end(),
                                 [](const auto &i1, const auto &i2) {
                                   return i1.second.getMaximumTime() <
                                          i2.second.getMaximumTime();
                                 })
                    ->second.getMaximumTime(),
                std::max_element(loadedScript.loadedQuaternionTimelines.begin(),
                                 loadedScript.loadedQuaternionTimelines.end(),
                                 [](const auto &i1, const auto &i2) {
                                   return i1.second.getMaximumTime() <
                                          i2.second.getMaximumTime();
                                 })
                    ->second.getMaximumTime()});
  return loadedScript;
}