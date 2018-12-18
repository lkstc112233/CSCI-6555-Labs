#include "ProgressBar.h"

#include <algorithm>

#include <glad/glad.h>

#include "../Graphics/WindowInitializer.h"

#include "../Graphics/Models/Model.h"
#include "../Graphics/Object/Object.h"
#include "../Graphics/Shaders/Shader.h"

#include "Utilities/TwoStateButton.h"

#include <glm/glm.hpp>

ProgressBar::ProgressBar()
    : playedProgressBar(ModelLoader::getUnitSquareShape()),
      unplayedProgressBar(ModelLoader::getUnitSquareShape()),
      keyframeObject(ModelLoader::getUnitSquareShape()),
      playPauseButton("res/shapes/pause.shp", "res/shapes/play.shp", -1, -1,
                      0.1) {
  playedProgressBar.setColor(glm::vec3(0.0, 0.5, 0.5));
  unplayedProgressBar.setColor(glm::vec3(0.9));
  keyframeObject.setColor(glm::vec3(0.8, 0.8, 0));
  keyframeObject.setZ(-0.1);
}

void ProgressBar::attachControls(KeyHandlerContainer &keyContainer,
                                 MouseHandlerContainer &mouseContainer) {
  playPauseButton.attachControls(mouseContainer);
  keyContainer.emplace_handler(
      GLFW_KEY_SPACE,
      [this]() { playPauseButton.state = !playPauseButton.state; }, true);
  // Process bar process change handler.
  mouseContainer.emplace_handler(
      [this](int mouseFlags, float clampedx, float clampedy) {
        static bool pressed = false;
        clampedx /= SCREEN_WIDTH;
        clampedy /= SCREEN_HEIGHT;
        if ((mouseFlags & MOUSE_LEFTBUTTON_PRESSED) &&
            clampedy < PROGRESS_BAR_LOWER_BOUND + PROGRESS_BAR_HEIGHT &&
            clampedy > PROGRESS_BAR_LOWER_BOUND &&
            clampedx < PROGRESS_BAR_LEFT_BOUND + PROGRESS_BAR_LENGTH &&
            clampedx > PROGRESS_BAR_LEFT_BOUND) {
          pressed = true;
        }
        if (pressed && (mouseFlags & MOUSE_LEFTBUTTON_HOLD)) {
          setProcess(glm::clamp(clampedx - PROGRESS_BAR_LEFT_BOUND, 0.0F,
                                PROGRESS_BAR_LENGTH) /
                     PROGRESS_BAR_LENGTH);
        }
        if (!(mouseFlags & MOUSE_LEFTBUTTON_HOLD)) {
          pressed = false;
        }
      });
}

void ProgressBar::setProcess(float proc) { process = fmod(proc, 1); }

void ProgressBar::addProcess(float proc) { setProcess(process + proc); }

void ProgressBar::draw(ShaderProgram &shader) {
  // Draws a progress bar
  playedProgressBar.setTransformMatrix(
      glm::mat3{{process * PROGRESS_BAR_LENGTH, 0, 0},
                {0, PROGRESS_BAR_HEIGHT, 0},
                {PROGRESS_BAR_LEFT_BOUND, PROGRESS_BAR_LOWER_BOUND, 1}});
  playedProgressBar.draw(shader);
  unplayedProgressBar.setTransformMatrix(
      glm::mat3{{PROGRESS_BAR_LENGTH - process * PROGRESS_BAR_LENGTH, 0, 0},
                {0, PROGRESS_BAR_HEIGHT, 0},
                {process * PROGRESS_BAR_LENGTH + PROGRESS_BAR_LEFT_BOUND,
                 PROGRESS_BAR_LOWER_BOUND, 1}});
  unplayedProgressBar.draw(shader);
  playPauseButton.draw(shader);
}
