#include "Scripts.h"

#include <algorithm>
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
static bool operator< (const Keyframe<T> &frame1, const Keyframe<T> &frame2)
{
    return frame1.getTimestamp() < frame2.getTimestamp();
}

template <typename T>
static bool operator< (float timestamp, const Keyframe<T> &frame)
{
    return timestamp < frame.getTimestamp();
}

template <typename T>
void ScriptsImplementation<T>::addKeyframe(const Keyframe<T> keyframe)
{
    keyframes.emplace(std::upper_bound(keyframes.begin(), keyframes.end(), keyframe), keyframe);
}

template <typename T>
float ScriptsImplementation<T>::getMaximumTime() const
{
    if (keyframes.size() < 2) {
        return 0;
    }
    return keyframes[keyframes.size() - 2].getTimestamp();
}

template <typename T>
glm::mat4 ScriptsImplementation<T>::getTranscationMatrixAt(float time)
{
    time = fmod(time, getMaximumTime());
    auto position = std::upper_bound(keyframes.begin(), keyframes.end(), time) - 1;

    time -= position->getTimestamp();
    time /= (position + 1)->getTimestamp() - position->getTimestamp();

    if (useCatmullRomInterpolate) {
        return catmullRomInterpolate(time, position - 1).getTranscationMatrix();
    } else {
        return uniformNonrationalBasisSplineInterpolate(time, position - 1).getTranscationMatrix();
    }
}

template <typename T>
float ScriptsImplementation<T>::getActivedTimestamp() const {
    return keyframes[getActivedKeyframe()].getTimestamp();
}

template <typename T>
void ScriptsImplementation<T>::addKeyframeAt(float time) {
    // For now we add the keyframe at where it's supposed to be, 
    // but no orientation, to reduce workload.
    auto transform = getTranscationMatrixAt(time);
    addKeyframe(Keyframe<T>(time, transform[3][0], transform[3][1], transform[3][2], T()));
    rearrangeKeyframes();
    activeKeyframeIndex = find(timestamps.begin(), timestamps.end(), time) - timestamps.begin();
}

template <typename T>
void ScriptsImplementation<T>::removeKeyframeOf(int index) {
    keyframes.erase(keyframes.begin() + index);
    rebuildTimestampIndex();
    activeKeyframeIndex = std::min(activeKeyframeIndex, int(keyframes.size()) - 1);
}

template <typename T>
void ScriptsImplementation<T>::setActiveTimestamp(float timestamp) {
    keyframes[getActivedKeyframe()].setTimestamp(timestamp);
    rebuildTimestampIndex();
}

template <typename T>
void ScriptsImplementation<T>::moveActiveKeyframeBy(glm::vec3 movement) {
    keyframes[getActivedKeyframe()].moveX(movement.x);
    keyframes[getActivedKeyframe()].moveY(movement.y);
    keyframes[getActivedKeyframe()].moveZ(movement.z);
}

template <typename T>
void ScriptsImplementation<T>::rotateActiveKeyframeBy(float yaw, float pitch, float roll) {
    keyframes[getActivedKeyframe()].rotateBy(yaw, pitch, roll);
}

template <typename T>
void ScriptsImplementation<T>::rearrangeKeyframes() {
    std::sort(keyframes.begin(), keyframes.end(), [](auto &key1, auto &key2){return key1.getTimestamp() < key2.getTimestamp();});
    rebuildTimestampIndex();
}

template <typename T>
void ScriptsImplementation<T>::rebuildTimestampIndex() {
    timestamps.clear();
    timestamps.resize(keyframes.size());
    std::transform(
        keyframes.begin(), 
        keyframes.end(), 
        timestamps.begin(), 
        [](auto frame){ return frame.getTimestamp(); });
}

template <typename T>
template <typename U>
std::unique_ptr<Scripts> ScriptsImplementation<T>::switchRotationRepresentationImplementation() {
    class ConvertedScript:public ScriptsImplementation<U>{};
    auto convertedScript = std::make_unique<ConvertedScript>();
    for (auto& key: keyframes) {
        convertedScript->addKeyframe(Keyframe<U>(key));
    }
    convertedScript->rebuildTimestampIndex();
    return convertedScript;
}

template<>
std::unique_ptr<Scripts> ScriptsImplementation<Quaternion>::switchRotationRepresentation() {
    return switchRotationRepresentationImplementation<EulerAngles>();
}

template<>
std::unique_ptr<Scripts> ScriptsImplementation<EulerAngles>::switchRotationRepresentation() {
    return switchRotationRepresentationImplementation<Quaternion>();
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

    // Load until end of control points
    while (parser.isValid())
    {
        if (!parser.expect('('))
        {
            break;
        }
        float data[8];
        int loaded = parser.tryParseFloat(data, 8);
        if (!parser.expect(')'))
        {
            std::cerr << "ScriptsLoader: Warning: Error happened parsing the script: " << filename << std::endl;
            break;
        }
        if (loaded == 8)
        {
            qcount += 1;
            quaternionScripts->addKeyframe(Keyframe<Quaternion>(data[0], data[1], data[2], data[3], Quaternion(data[4], data[5], data[6], data[7])));
        }
        if (loaded == 7)
        {
            ecount += 1;
            eulerAnglesScripts->addKeyframe(Keyframe<EulerAngles>(data[0], data[1], data[2], data[3], 
            EulerAngles(glm::radians(data[4]), glm::radians(data[5]), glm::radians(data[6]))));
        }
    }
    // Return whether one is more.
    if (qcount < ecount)
    {
        eulerAnglesScripts->rebuildTimestampIndex();
        return eulerAnglesScripts;
    }
    quaternionScripts->rebuildTimestampIndex();
    return quaternionScripts;
}