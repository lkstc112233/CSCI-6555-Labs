#ifndef ANIMATE_SCRIPTS_H
#define ANIMATE_SCRIPTS_H

#include <memory>

#include <glm/glm.hpp>

class Scripts
{
  public:
    virtual ~Scripts();
    virtual glm::mat4 getTransformMatrixAt(float time) = 0;
};

template <typename T>
class ScriptsImplementation : public Scripts
{
  private:
    friend class ScriptsLoader;
  public:
    virtual glm::mat4 getTransformMatrixAt(float time);
};

class ScriptsLoader
{
  public:
    static std::unique_ptr<Scripts> loadScript(const char *filename);
};

#endif // ANIMATE_SCRIPTS_H