#ifndef INTERFACE_PROGRESSBAR_H
#define INTERFACE_PROGRESSBAR_H

#include "../Graphics/Object/Object.h"
#include "../Graphics/Shaders/Shader.h"
#include "KeyHandler.h"
#include "MouseHandler.h"

class ProgressBar
{
  private:
	static const float PROGRESS_BAR_LOWER_BOUND;
	static const float PROGRESS_BAR_LEFT_BOUND;
	static const float PROGRESS_BAR_LENGTH;
	static const float PROGRESS_BAR_HEIGHT;
    const Model playModel;
    const Model pauseModel;
    Object2D playedProgressBar;
    Object2D unplayedProgressBar;
	Object2D play;
	Object2D pause;

  public:
    bool playing = true;
    float process = 0;
    ProgressBar();
    void attachControls(KeyHandlerContainer& keyContainer, MouseHandlerContainer& mouseContainer);
	void draw(ShaderProgram& shader);
};

#endif // INTERFACE_PROGRESSBAR_H