#ifndef ANIMATE_SCRIPTS_H
#define ANIMATE_SCRIPTS_H

#include <memory>
#include <vector>

#include <glm/glm.hpp>

#include "Keyframe.hpp"

class Scripts
{
  public:
    virtual ~Scripts(){}
    virtual float getMaximumTime() const = 0;
    virtual glm::mat4 getTranscationMatrixAt(float time) = 0;
};

template <typename T>
class ScriptsImplementation : public Scripts
{
  private:
    friend class ScriptsLoader;
    static bool compare(const Keyframe<T>& frame1, const Keyframe<T>& frame2);
    std::vector<Keyframe<T>> keyframes;
    void addKeyframe(const Keyframe<T> keyframe);

  public:
    virtual float getMaximumTime() const;
    virtual glm::mat4 getTranscationMatrixAt(float time);
};

class ScriptsLoader
{
  public:
    static std::unique_ptr<Scripts> loadScript(const char *filename);
};

#endif // ANIMATE_SCRIPTS_H