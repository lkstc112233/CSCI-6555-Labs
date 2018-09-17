#include "Scripts.h"

#include <memory>

#include <glm/glm.hpp>

template <typename T>
glm::mat4 ScriptsImplementation<T>::getTransformMatrixAt(float time)
{
    // TODO: implement keyframe speed.

}

std::unique_ptr<Scripts> ScriptsLoader::loadScript(const char *filename)
{
    
}