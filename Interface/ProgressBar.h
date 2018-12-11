#ifndef INTERFACE_PROGRESSBAR_H
#define INTERFACE_PROGRESSBAR_H

#include <memory>

#include "../Graphics/Object/Object.h"
#include "../Graphics/Shaders/Shader.h"
#include "KeyHandler.h"
#include "MouseHandler.h"
#include "Utilities/TwoStateButton.h"

#include "../Animate/Scripts.h"

/**
 * A class holding editing operations such as keyframe operations, and playing
 * operations.
 */
class ProgressBar {
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
  float process = 0;

 public:
  bool isPlaying() const { return playPauseButton.state; }
  float getProcess() const { return process; }
  void setProcess(float process);
  void addProcess(float process);
  ProgressBar();
  void attachControls(KeyHandlerContainer &keyContainer,
                      MouseHandlerContainer &mouseContainer);
  void draw(ShaderProgram &shader);
};

#endif  // INTERFACE_PROGRESSBAR_H