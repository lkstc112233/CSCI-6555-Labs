#ifndef INTERFACE_PROGRESSBAR_H
#define INTERFACE_PROGRESSBAR_H

#include "../Graphics/Object/Object.h"
#include "../Graphics/Shaders/Shader.h"
#include "Utilities/TwoStateButton.h"
#include "KeyHandler.h"
#include "MouseHandler.h"

class ProgressBar
{
  private:
	static const float PROGRESS_BAR_LOWER_BOUND;
	static const float PROGRESS_BAR_LEFT_BOUND;
	static const float PROGRESS_BAR_LENGTH;
	static const float PROGRESS_BAR_HEIGHT;
    Object2D playedProgressBar;
    Object2D unplayedProgressBar;
    TwoStateButton playPauseButton;
    // For some reason the edit button is here...
    TwoStateButton editButton;
    float process = 0;

  public:
  bool isPlaying() const { return playPauseButton.state; }
  float getProcess() const { return process; }
  void setProcess(float process);
  void addProcess(float process);
    ProgressBar();
    void attachControls(KeyHandlerContainer& keyContainer, MouseHandlerContainer& mouseContainer);
	void draw(ShaderProgram& shader);
};

#endif // INTERFACE_PROGRESSBAR_H