#ifndef INTERFACE_PROGRESSBAR_H
#define INTERFACE_PROGRESSBAR_H

#include <memory>

#include "../Graphics/Object/Object.h"
#include "../Graphics/Shaders/Shader.h"
#include "Utilities/TwoStateButton.h"
#include "KeyHandler.h"
#include "MouseHandler.h"

#include "../Animate/Scripts.h"

class ProgressBar
{
  private:
    constexpr static const float KEYFRAME_INDICATOR_HEIGHT_DIFF = 0.02;
    constexpr static const float KEYFRAME_INDICATOR_WIDTH_HALFED = 0.005;
    constexpr static const float PROGRESS_BAR_LOWER_BOUND = -0.967;
    constexpr static const float PROGRESS_BAR_LEFT_BOUND = -0.867;
    constexpr static const float PROGRESS_BAR_LENGTH = 1.833;
    constexpr static const float PROGRESS_BAR_HEIGHT = 0.033;
    Object2D playedProgressBar;
    Object2D unplayedProgressBar;
    Object2D keyframeObject;
    TwoStateButton playPauseButton;
    // For some reason the edit button is here...
    TwoStateButton editButton;
    // Again for some reason the script is kept here...
    const std::unique_ptr<Scripts>& script;
    float process = 0;

  public:
  bool isPlaying() const { return playPauseButton.state; }
  bool isEditing() const { return !editButton.state; }
  float getProcess() const { return process; }
  void setProcess(float process);
  void addProcess(float process);
    ProgressBar(const std::unique_ptr<Scripts> &script);
    void attachControls(KeyHandlerContainer& keyContainer, MouseHandlerContainer& mouseContainer);
	void draw(ShaderProgram& shader);
};

#endif // INTERFACE_PROGRESSBAR_H