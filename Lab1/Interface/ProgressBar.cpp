#include "ProgressBar.h"

#include <glad/glad.h>

#include "../Graphics/WindowInitializer.h"

#include "../Graphics/Models/Model.h"
#include "../Graphics/Object/Object.h"
#include "../Graphics/Shaders/Shader.h"

#include "Utilities/TwoStateButton.h"

#include <glm/glm.hpp>

const float ProgressBar::PROGRESS_BAR_LOWER_BOUND = -0.967;
const float ProgressBar::PROGRESS_BAR_LEFT_BOUND = -0.867;
const float ProgressBar::PROGRESS_BAR_LENGTH = 1.833;
const float ProgressBar::PROGRESS_BAR_HEIGHT = 0.033;

ProgressBar::ProgressBar() 
    : playedProgressBar(ModelLoader::getUnitSquareShape())
    , unplayedProgressBar(ModelLoader::getUnitSquareShape())
    , playPauseButton("res/shapes/pause.shp", "res/shapes/play.shp", -1, -1, 0.1)
    , editButton("res/shapes/hammer.shp", "res/shapes/confirm.shp", -1, 0.9, 0.1, glm::radians(45.0f))
{
    playedProgressBar.setColor(glm::vec3(1.0, 0, 0));
    unplayedProgressBar.setColor(glm::vec3(0.9));
}

void ProgressBar::attachControls(KeyHandlerContainer& keyContainer, MouseHandlerContainer& mouseContainer) {
	playPauseButton.attachControls(mouseContainer);
	editButton.attachControls(mouseContainer);
	keyContainer.emplace_handler(GLFW_KEY_SPACE, [this]() {
		playPauseButton.state = !playPauseButton.state;
	}, true);
	// Process bar process change handler.
    mouseContainer.emplace_handler([=](int mouseFlags, float clampedx, float clampedy) {
		static bool pressed = false;
		clampedx /= SCREEN_WIDTH;
		clampedy /= SCREEN_HEIGHT;
		if ((mouseFlags & MOUSE_LEFTBUTTON_PRESSED) 
		&& clampedy < PROGRESS_BAR_LOWER_BOUND + PROGRESS_BAR_HEIGHT
		&& clampedy > PROGRESS_BAR_LOWER_BOUND
		&& clampedx < PROGRESS_BAR_LEFT_BOUND + PROGRESS_BAR_LENGTH
		&& clampedx > PROGRESS_BAR_LEFT_BOUND) {
			pressed = true;
		}
		if (pressed && (mouseFlags & MOUSE_LEFTBUTTON_HOLD)) {
			setProcess(glm::clamp(clampedx - PROGRESS_BAR_LEFT_BOUND, 0.0f, PROGRESS_BAR_LENGTH) / PROGRESS_BAR_LENGTH);
		}
		if (!(mouseFlags & MOUSE_LEFTBUTTON_HOLD)) {
			pressed = false;
		}
	}); 
}

void ProgressBar::setProcess(float proc) {
	if (editButton.state) {
		process = fmod(proc, 1);
	}
}

void ProgressBar::addProcess(float proc) {
	setProcess(process + proc);
}

void ProgressBar::draw(ShaderProgram& shader) {
    playedProgressBar.setTransformMatrix(glm::mat3{
        {process * PROGRESS_BAR_LENGTH, 0, 0},
        {0, PROGRESS_BAR_HEIGHT ,0},
        {PROGRESS_BAR_LEFT_BOUND, PROGRESS_BAR_LOWER_BOUND, 1}});
	playedProgressBar.draw(shader);
    unplayedProgressBar.setTransformMatrix(glm::mat3{
        {PROGRESS_BAR_LENGTH - process * PROGRESS_BAR_LENGTH, 0, 0}, 
        {0, PROGRESS_BAR_HEIGHT ,0}, 
        { process *  PROGRESS_BAR_LENGTH + PROGRESS_BAR_LEFT_BOUND, PROGRESS_BAR_LOWER_BOUND, 1}});
	unplayedProgressBar.draw(shader);
	playPauseButton.draw(shader);
	editButton.draw(shader);
}
