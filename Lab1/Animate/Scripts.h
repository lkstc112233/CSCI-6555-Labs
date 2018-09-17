#ifndef ANIMATE_SCRIPTS_H
#define ANIMATE_SCRIPTS_H

#include <memory>
class Scripts
{
  public:
    virtual ~Scripts();
};

template <typename T>
class ScriptsImplementation : public Scripts
{
  private:
    friend class ScriptsLoader;
};

class ScriptsLoader
{
  public:
    static std::unique_ptr<Scripts> loadScript(const char *filename);
};

#endif // ANIMATE_SCRIPTS_H