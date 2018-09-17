#include "Scripts.h"

#include <memory>
#include <vector>

#include <glm/glm.hpp>

#include "Keyframe.hpp"
#include "Interpolate.hpp"
#include "../fileop/fileParser.h"

template <typename T>
void ScriptsImplementation<T>::addKeyframe(const Keyframe<T> keyframe)
{
    keyframes.emplace_back(keyframe);
}

template <typename T>
glm::mat4 ScriptsImplementation<T>::getTransformMatrixAt(float time)
{
    // TODO: implement keyframe speed.

}

std::unique_ptr<Scripts> ScriptsLoader::loadScript(const char *filename)
{
    
}