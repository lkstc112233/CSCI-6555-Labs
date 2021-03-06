#ifndef ANIMATE_TIMELINE_HPP
#define ANIMATE_TIMELINE_HPP

#include <algorithm>
#include <initializer_list>
#include <vector>

#include "Keyframe.hpp"

#include "../Math/Interpolate.hpp"

/**
 * A class holding a movie script.
 */
template <typename T>
class Timeline {
 private:
  std::vector<float> timestamps;
  std::vector<Keyframe<T>> keyframes;
  void rebuildTimestampIndex() {
    timestamps.clear();
    timestamps.resize(keyframes.size());
    std::transform(keyframes.begin(), keyframes.end(), timestamps.begin(),
                   [](auto frame) { return frame.getTimestamp(); });
  }

 public:
  void addKeyframe(const Keyframe<T> keyframe) {
    keyframes.emplace(
        std::upper_bound(
            keyframes.begin(), keyframes.end(), keyframe,
            [](const Keyframe<T> &frame1, const Keyframe<T> &frame2) {
              return frame1.getTimestamp() < frame2.getTimestamp();
            }),
        keyframe);
  }
  Timeline() {}
  Timeline(const Timeline &another)
      : timestamps(another.timestamps), keyframes(another.keyframes) {}
  Timeline(Timeline &&another)
      : timestamps(std::move(another.timestamps)),
        keyframes(std::move(another.keyframes)) {}
  float getMaximumTime() const {
    if (keyframes.size() < 2) {
      return 0;
    }
    return keyframes[keyframes.size() - 2].getTimestamp();
  }
  float getMaximumId() const { return keyframes.size(); }
  const std::vector<float> &getTimestamps() const { return timestamps; }
  T getDataAt(float time) const {
    time = fmod(time, getMaximumTime());
    auto position = std::upper_bound(keyframes.begin(), keyframes.end(), time,
                                     [](float val, const Keyframe<T> &frame) {
                                       return val < frame.getTimestamp();
                                     }) -
                    1;

    time -= position->getTimestamp();
    time /= (position + 1)->getTimestamp() - position->getTimestamp();

    return catmullRomInterpolate(time, position - 1).getData();
  }
  T getDataOf(int i) const { return keyframes[i]; }
  void rearrangeKeyframes() {
    std::sort(keyframes.begin(), keyframes.end(), [](auto &key1, auto &key2) {
      return key1.getTimestamp() < key2.getTimestamp();
    });
    rebuildTimestampIndex();
  }
};

class ManagedTimelineInterface {
 public:
  virtual void setRate(float rate) {}
  virtual void handleDataAt(float time) {}
};

template <typename T>
class ManagerTimeline : public ManagedTimelineInterface {
 private:
  std::vector<Timeline<T>> timelines;
  T &managing;
  float rate;
  float offset;
  T getDataAt(float time) {
    if (rate < 0) {
      return timelines.front().getDataAt(time + offset);
    }
    if (rate >= timelines.size() - 1) {
      return timelines.back().getDataAt(time + offset);
    }
    // Linear interpolate
    int base = floor(rate);
    float t = rate - base;
    return simpleLinearInterpolate(
        t, timelines[base].getDataAt(time + offset),
        timelines[base + 1].getDataAt(time + offset));
  }

 public:
  ManagerTimeline(T &managingi, Timeline<T> &&timelinei, float offseti = 0)
      : managing(managingi), offset(offseti), rate(0) {
    timelines.emplace_back(std::move(timelinei));
  }
  ManagerTimeline(T &managingi, std::initializer_list<Timeline<T>> list,
                  float offseti = 0)
      : managing(managingi), offset(offseti), rate(0) {
    for (auto &elem : list) {
      timelines.emplace_back(std::move(elem));
    }
  }
  ManagerTimeline(ManagerTimeline &&another)
      : timelines(std::move(another.timelines)),
        managing(another.managing),
        offset(another.offset),
        rate(another.rate) {}
  virtual void setRate(float ratei) { rate = ratei; }
  virtual void handleDataAt(float time) { managing = getDataAt(time); }
};

#endif  // ANIMATE_TIMELINE_HPP