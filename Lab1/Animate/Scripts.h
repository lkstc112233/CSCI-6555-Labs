#ifndef ANIMATE_SCRIPTS_H
#define ANIMATE_SCRIPTS_H

#include <memory>
#include <vector>

#include <glm/glm.hpp>

#include "Keyframe.hpp"

class Scripts
{
  protected:
    int activeKeyframeIndex = 0;
  public:
    virtual ~Scripts(){}
    // Returns a list of timestamps.
    virtual const std::vector<float>& getTimestamps() const = 0;
    virtual float getMaximumTime() const = 0;
    virtual glm::mat4 getTranscationMatrixAt(float time) = 0;
    void activeKeyframe(int index) {activeKeyframeIndex = index;}
    int getActivedKeyframe() const {return activeKeyframeIndex;}
    virtual float getActivedTimestamp() const = 0;
    virtual void setActiveTimestamp(float timestamp) = 0;
    virtual void moveActiveKeyframeBy(glm::vec3) = 0;
    virtual void rearrangeKeyframes() = 0;
};

template <typename T>
class ScriptsImplementation : public Scripts
{
  private:
    friend class ScriptsLoader;
    std::vector<float> timestamps;
    std::vector<Keyframe<T>> keyframes;
    void addKeyframe(const Keyframe<T> keyframe);
    void rebuildTimestampIndex();

  public:
    virtual float getMaximumTime() const;
    virtual const std::vector<float>& getTimestamps() const { return timestamps; }
    virtual glm::mat4 getTranscationMatrixAt(float time);
    virtual float getActivedTimestamp() const;
    virtual void setActiveTimestamp(float timestamp);
    virtual void moveActiveKeyframeBy(glm::vec3);
    virtual void rearrangeKeyframes();
};

class ScriptsLoader
{
  public:
    static std::unique_ptr<Scripts> loadScript(const char *filename);
};

#endif // ANIMATE_SCRIPTS_H