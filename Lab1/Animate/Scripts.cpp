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
glm::mat4 ScriptsImplementation<T>::getTranscationMatrixAt(float time)
{
    // TODO: implement keyframe speed.
    // For now it's only uniform, repeating.

    int maximum = keyframes.size() - 3;
    if (maximum <= 3)
    {
        // Too less keyframes. Just return what's available.
        if (keyframes.size())
        {
            return keyframes[0].getTranscationMatrix();
        }
        return glm::mat4(1.0f);
    }

    float frame = fmod(time, maximum);
    float t = fmod(frame, 1.0);
    int indexBegin = frame - 1;
    
    return catmullRomInterpolate(t, keyframes.begin() + indexBegin).getTranscationMatrix();
}

std::unique_ptr<Scripts> ScriptsLoader::loadScript(const char *filename)
{
    
}