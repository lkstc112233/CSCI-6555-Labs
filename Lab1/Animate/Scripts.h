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
    virtual glm::mat4 getTranscationMatrixAt(float time) = 0;
};

template <typename T>
class ScriptsImplementation : public Scripts
{
  private:
    friend class ScriptsLoader;
    std::vector<Keyframe<T>> keyframes;
    void addKeyframe(const Keyframe<T> keyframe);

  public:
    virtual glm::mat4 getTranscationMatrixAt(float time);
};

class ScriptsLoader
{
  public:
    static std::unique_ptr<Scripts> loadScript(const char *filename);
};

#endif // ANIMATE_SCRIPTS_H