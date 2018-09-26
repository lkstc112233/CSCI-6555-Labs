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
    bool useCatmullRomInterpolate = true;
  public:
    virtual ~Scripts(){}
    // Returns a list of timestamps.
    virtual const std::vector<float>& getTimestamps() const = 0;
    virtual float getMaximumTime() const = 0;
    virtual float getMaximumId() const = 0;
    virtual glm::mat4 getTranscationMatrixAt(float time) = 0;
    virtual glm::mat4 getTranscationMatrixOf(int i) = 0;
    void activeKeyframe(int index) {activeKeyframeIndex = index;}
    int getActivedKeyframe() const {return activeKeyframeIndex;}
    virtual float getActivedTimestamp() const = 0;
    virtual void setActiveTimestamp(float timestamp) = 0;
    virtual void moveActiveKeyframeBy(glm::vec3) = 0;
    virtual void rotateActiveKeyframeBy(float yaw, float pitch, float roll) = 0;
    virtual void rearrangeKeyframes() = 0;
    void switchInterpolate() { useCatmullRomInterpolate = !useCatmullRomInterpolate; }
    virtual std::unique_ptr<Scripts> switchRotationRepresentation() = 0;
};

template <typename T>
class ScriptsImplementation : public Scripts
{
  private:
    friend class ScriptsLoader;
    template <typename AnyKeyframeType>
    friend class ScriptsImplementation;
    std::vector<float> timestamps;
    std::vector<Keyframe<T>> keyframes;
    void addKeyframe(const Keyframe<T> keyframe);
    void rebuildTimestampIndex();
    template <typename U>
    std::unique_ptr<Scripts> switchRotationRepresentationImplementation();

  public:
    virtual float getMaximumTime() const;
    virtual float getMaximumId() const { return keyframes.size(); }
    virtual const std::vector<float>& getTimestamps() const { return timestamps; }
    virtual glm::mat4 getTranscationMatrixAt(float time);
    virtual glm::mat4 getTranscationMatrixOf(int i) { return keyframes[i].getTranscationMatrix(); }
    virtual float getActivedTimestamp() const;
    virtual void setActiveTimestamp(float timestamp);
    virtual void moveActiveKeyframeBy(glm::vec3);
    virtual void rotateActiveKeyframeBy(float yaw, float pitch, float roll);
    virtual void rearrangeKeyframes();
    virtual std::unique_ptr<Scripts> switchRotationRepresentation();
};

class ScriptsLoader
{
  public:
    static std::unique_ptr<Scripts> loadScript(const char *filename);
};

#endif // ANIMATE_SCRIPTS_H