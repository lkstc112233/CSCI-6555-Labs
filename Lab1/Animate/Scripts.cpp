#include "Scripts.h"

#include <iostream>
#include <memory>
#include <vector>

#include <glm/glm.hpp>

#include "Keyframe.hpp"
#include "Interpolate.hpp"
#include "../fileop/fileParser.h"
#include "../Math/Quaternion.h"
#include "../Math/EulerAngles.h"


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
    if (maximum <= 0)
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
    int indexBegin = floor(frame);

    return catmullRomInterpolate(t, keyframes.begin() + indexBegin).getTranscationMatrix();
}

std::unique_ptr<Scripts> ScriptsLoader::loadScript(const char *filename)
{
    class QuaternionScript:public ScriptsImplementation<Quaternion>{};
    class EulerAnglesScript:public ScriptsImplementation<EulerAngles>{};
    auto quaternionScripts = std::make_unique<QuaternionScript>();
    auto eulerAnglesScripts = std::make_unique<EulerAnglesScript>();
    FileParser parser(filename);

    int qcount = 0;
    int ecount = 0;

    while (parser.isValid())
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
            qcount += 1;
            quaternionScripts->addKeyframe(Keyframe<Quaternion>(data[0], data[1], data[2], Quaternion(data[3], data[4], data[5], data[6])));
        }
        if (loaded == 6) {
            ecount += 1;
            eulerAnglesScripts->addKeyframe(Keyframe<EulerAngles>(data[0], data[1], data[2], 
            EulerAngles(glm::radians(data[3]), glm::radians(data[4]), glm::radians(data[5]))));
        }

    }
    if (qcount < ecount)
    {
        return eulerAnglesScripts;
    }
    return quaternionScripts;
}