#include "Scripts.h"

#include <iostream>
#include <memory>
#include <vector>

#include <glm/glm.hpp>

#include "Keyframe.hpp"
#include "Interpolate.hpp"
#include "../fileop/fileParser.h"
#include "../Math/Quaternion.h"


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
    class QuaternionScript:public ScriptsImplementation<Quaternion>{};
    auto quaternionScripts = std::make_unique<QuaternionScript>();
    FileParser parser(filename);
    int keyframesCount;
    parser.tryParseInt(&keyframesCount);

    for (int i = 0; i < keyframesCount; ++i)
    {
        if (!parser.expect('('))
        {
            std::cerr << "ScriptsLoader: Warning: Error happened parsing the script: " << filename << std::endl;
            break;
        }
        float data[7];
        int loaded = parser.tryParseFloat(data, 7);
        if (!parser.expect(')'))
        {
            std::cerr << "ScriptsLoader: Warning: Error happened parsing the script: " << filename << std::endl;
            break;
        }
        if (loaded == 7) {
            quaternionScripts->addKeyframe(Keyframe<Quaternion>(data[0], data[1], data[2], Quaternion(data[3], data[4], data[5], data[6])));
        }

    }
    return quaternionScripts;
}