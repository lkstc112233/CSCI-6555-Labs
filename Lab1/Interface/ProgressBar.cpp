#include "ProgressBar.h"

#include <algorithm>

#include <glad/glad.h>

#include "../Graphics/WindowInitializer.h"

#include "../Graphics/Models/Model.h"
#include "../Graphics/Object/Object.h"
#include "../Graphics/Shaders/Shader.h"

#include "Utilities/TwoStateButton.h"

#include <glm/glm.hpp>

ProgressBar::ProgressBar(const std::unique_ptr<Scripts> &scripti) 
    : playedProgressBar(ModelLoader::getUnitSquareShape())
    , unplayedProgressBar(ModelLoader::getUnitSquareShape())
    , keyframeObject(ModelLoader::getUnitSquareShape())
    , playPauseButton("res/shapes/pause.shp", "res/shapes/play.shp", -1, -1, 0.1)
    , editButton("res/shapes/hammer.shp", "res/shapes/confirm.shp", -1, 0.9, 0.1, glm::radians(45.0f))
	, script(scripti)
{
    playedProgressBar.setColor(glm::vec3(1.0, 0, 0));
    unplayedProgressBar.setColor(glm::vec3(0.9));
	keyframeObject.setColor(glm::vec3(0.8, 0.8, 0));
	keyframeObject.setZ(-0.1);
}

void ProgressBar::attachControls(KeyHandlerContainer& keyContainer, MouseHandlerContainer& mouseContainer) {
	playPauseButton.attachControls(mouseContainer);
	editButton.attachControls(mouseContainer);
	keyContainer.emplace_handler(GLFW_KEY_SPACE, [this]() {
		playPauseButton.state = !playPauseButton.state;
	}, true);
	keyContainer.emplace_handler(GLFW_KEY_Y, [this]() {
		script->rotateActiveKeyframeBy(0.005, 0, 0);
	});
	keyContainer.emplace_handler(GLFW_KEY_H, [this]() {
		script->rotateActiveKeyframeBy(-0.005, 0, 0);
	});
	keyContainer.emplace_handler(GLFW_KEY_U, [this]() {
		script->rotateActiveKeyframeBy(0, 0.005, 0);
	});
	keyContainer.emplace_handler(GLFW_KEY_J, [this]() {
		script->rotateActiveKeyframeBy(0, -0.005, 0);
	});
	keyContainer.emplace_handler(GLFW_KEY_I, [this]() {
		script->rotateActiveKeyframeBy(0, 0, 0.005);
	});
	keyContainer.emplace_handler(GLFW_KEY_K, [this]() {
		script->rotateActiveKeyframeBy(0, 0, -0.005);
	});
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
    mouseContainer.emplace_handler([=](int mouseFlags, float clampedx, float clampedy, float diffx, float diffy) {
		clampedx /= SCREEN_WIDTH;
		clampedy /= SCREEN_HEIGHT;
		if (editButton.state) {
			return;
		}
		static int draggingKeyframeDefinitionCountdown = -1;
		const std::vector<float>& frames = script->getTimestamps();
		auto inKeyframeHeightRange = [] (auto clampedy) {
			return clampedy < PROGRESS_BAR_LOWER_BOUND + KEYFRAME_INDICATOR_HEIGHT_DIFF + PROGRESS_BAR_HEIGHT
				&& clampedy > PROGRESS_BAR_LOWER_BOUND - KEYFRAME_INDICATOR_HEIGHT_DIFF;
		};
		auto inKeyframeWidthRange = [this, clampedx] (auto keyframeTimestamp) {
			return clampedx < PROGRESS_BAR_LEFT_BOUND + KEYFRAME_INDICATOR_WIDTH_HALFED + keyframeTimestamp * PROGRESS_BAR_LENGTH / script->getMaximumTime()
				&& clampedx > PROGRESS_BAR_LEFT_BOUND - KEYFRAME_INDICATOR_WIDTH_HALFED + keyframeTimestamp * PROGRESS_BAR_LENGTH / script->getMaximumTime();
		};
		if ((mouseFlags & MOUSE_LEFTBUTTON_PRESSED) && inKeyframeHeightRange(clampedy)) {
			auto target = std::find_if(frames.cbegin() + 1, frames.cend() - 1, inKeyframeWidthRange);
			if (target != frames.cend() - 1) {
				draggingKeyframeDefinitionCountdown = 10;
				script->activeKeyframe(target - frames.cbegin());
			}
			if (draggingKeyframeDefinitionCountdown != 10) {
				if (clampedx < PROGRESS_BAR_LEFT_BOUND + PROGRESS_BAR_LENGTH && clampedx > PROGRESS_BAR_LEFT_BOUND) {
					draggingKeyframeDefinitionCountdown = 10;
					script->addKeyframeAt((clampedx - PROGRESS_BAR_LEFT_BOUND) / PROGRESS_BAR_LENGTH * script->getMaximumTime());
				}
			}
		}
		if ((mouseFlags & MOUSE_RIGHTBUTTON_PRESSED) && inKeyframeHeightRange(clampedy)) {
			auto target = std::find_if(frames.cbegin() + 1, frames.cend() - 1, inKeyframeWidthRange);
			if (target != frames.cend() - 1) {
				draggingKeyframeDefinitionCountdown = 10;
				script->removeKeyframeOf(target - frames.cbegin());
			}
		}
		if (mouseFlags & MOUSE_LEFTBUTTON_HOLD) {
			if (draggingKeyframeDefinitionCountdown) {
				if (draggingKeyframeDefinitionCountdown > 0) {
					if (inKeyframeWidthRange(script->getActivedTimestamp())) {
						draggingKeyframeDefinitionCountdown -= 1;
					}
				} else {
					// Move keyframe
					const float rate = 0.005;
					const float zrate = 50;
					float zmovement = 0;
					if (mouseFlags & MOUSE_SCROLLED_UP_PRESSED) {
						zmovement += rate * zrate;
					} 
					if (mouseFlags & MOUSE_SCROLLED_DOWN_PRESSED) {
						zmovement -= rate * zrate;
					}
					script->moveActiveKeyframeBy(glm::vec3(-diffx * rate, -diffy * rate, zmovement));
				}
			} else {
				script->setActiveTimestamp(std::max(0.0f, (clampedx - PROGRESS_BAR_LEFT_BOUND) / PROGRESS_BAR_LENGTH) * script->getMaximumTime());
			}
		}
		if (!(mouseFlags & MOUSE_LEFTBUTTON_HOLD)) {
			draggingKeyframeDefinitionCountdown = -1;
			script->rearrangeKeyframes();
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
	if (editButton.state) {
		// Draws a progress bar
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
	} else {
		// Draws an edit bar, with the unplayed part's color aka light grey.
		unplayedProgressBar.setTransformMatrix(glm::mat3{
			{PROGRESS_BAR_LENGTH, 0, 0}, 
			{0, PROGRESS_BAR_HEIGHT ,0}, 
			{ PROGRESS_BAR_LEFT_BOUND, PROGRESS_BAR_LOWER_BOUND, 1}});
		unplayedProgressBar.draw(shader);
		// Draws keyframes.
		const std::vector<float>& frames = script->getTimestamps();
		for (auto iter = frames.cbegin() + 1; iter < frames.cend() - 1; ++iter) {
			keyframeObject.setTransformMatrix(glm::mat3{
				{0.01, 0, 0}, 
				{0, PROGRESS_BAR_HEIGHT + 0.04 ,0}, 
				{ PROGRESS_BAR_LEFT_BOUND - 0.005 + *iter * PROGRESS_BAR_LENGTH / script->getMaximumTime(), PROGRESS_BAR_LOWER_BOUND - 0.02, 1}});
			keyframeObject.draw(shader);
		}
	}
	playPauseButton.draw(shader);
	editButton.draw(shader);
}
