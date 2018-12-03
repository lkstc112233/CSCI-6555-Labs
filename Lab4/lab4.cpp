// standard
#include <cassert>
#include <cmath>
#include <iostream>

#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Graphics/Camera/Camera.h"
#include "Graphics/Entity/Entity.h"
#include "Graphics/Models/Model.h"
#include "Graphics/Object/Object.h"
#include "Graphics/Shaders/Shader.h"
#include "Graphics/Texture/Texture.h"
#include "Graphics/WindowInitializer.h"
#include "Physics/World.h"

#include "Interface/InterfaceRelatedStuffHolder.hpp"
#include "Interface/KeyHandler.h"
#include "Interface/MouseHandler.h"
#include "Interface/ProgressBar.h"
#include "Interface/Utilities/Button.h"

#include "Animate/Boid.h"
#include "Animate/Keyframe.hpp"
#include "Animate/Scripts.h"
#include "Animate/Timeline.hpp"
#include "Math/Quaternion.h"

const float PROJECTION_RATIO = float(SCREEN_WIDTH) / SCREEN_HEIGHT;

int main(int argc, char** argv) {
  GLFWwindow* window = initializeWindow("Lab4");
  if (!window) {
    return -1;
  }

  double mouseX, mouseY;
  int windowWidth, windowHeight;
  glfwGetCursorPos(window, &mouseX, &mouseY);
  glfwGetWindowSize(window, &windowWidth, &windowHeight);
  MouseHandlerContainer mouseHandlers(mouseX, mouseY, windowWidth,
                                      windowHeight);
  MouseCallbackWrapper::registerHandlerCallbacks(window, &mouseHandlers);

  Boids boids;
  Object3D target(ModelLoader::loadOffFile("res/models/ball.off"));
  Object3D boid(ModelLoader::loadOffFile("res/models/arrow.off"));
  boid.setScale(0.2);
  boid.setScaleZ(0.5);

  ShaderProgram shaderProgram{
      Shader::createVertexShader("res/shaders/simpleShader.vert"),
      Shader::createFragmentShader("res/shaders/simpleShader.frag")};
  if (!shaderProgram.isValid()) {
    return -4;
  }
  glm::mat4 projection(1.0f);
  projection =
      glm::perspective(glm::radians(45.0f), PROJECTION_RATIO, 0.1f, 1000.0f);
  shaderProgram.setMatrix("projection", projection);

  Camera camera;

  mouseHandlers.emplace_handler([&target, &camera](int mouseFlags, float, float,
                                                   float diffx, float diffy) {
    if (mouseFlags & MOUSE_LEFTBUTTON_HOLD) {
      float sensitivity = -0.005F;
      diffx *= sensitivity;
      diffy *= sensitivity;
      float diffz = 0;
      if (mouseFlags & MOUSE_SCROLLED_UP_PRESSED) {
        diffz += 100 * sensitivity;
      }
      if (mouseFlags & MOUSE_SCROLLED_DOWN_PRESSED) {
        diffz -= 100 * sensitivity;
      }

      target.moveBy(diffx * camera.getLeftVec() + diffy * camera.getUpVec() +
                    diffz * camera.getFrontVec());
    }
  });
  KeyHandlerContainer keyHandlers(window);
  camera.position = glm::vec3(0, 0, -10);

  attachCameraControls(keyHandlers, mouseHandlers, camera);

  keyHandlers.emplace_handler(
      GLFW_KEY_ESCAPE, [window]() { glfwSetWindowShouldClose(window, true); });

  ShaderProgram hudShader{
      Shader::createVertexShader("res/shaders/2DShader.vert"),
      Shader::createFragmentShader("res/shaders/2DShader.frag")};
  bool playing = true;

  Object2D cursor(ModelLoader::loadShpFile("res/shapes/cursor.shp"));
  if (!hudShader.isValid()) {
    return -4;
  }
  mouseHandlers.emplace_handler(
      [&cursor](int mouseFlags, float clampedx, float clampedy) {
        glm::mat3 cursorTransform(1.0f);
        cursorTransform[2] = {clampedx / SCREEN_WIDTH, clampedy / SCREEN_HEIGHT,
                              1.0};
        cursor.setTransformMatrix(cursorTransform);
        if (mouseFlags & MOUSE_RIGHTBUTTON_HOLD) {
          cursor.setOpacity(0.0f);
        } else {
          cursor.setOpacity(1.0f);
        }
      });

  double lastTime = glfwGetTime();

  bool play = false;
  keyHandlers.emplace_handler(GLFW_KEY_SPACE, [&play]() { play = true; }, true);

  while (!glfwWindowShouldClose(window)) {
    keyHandlers.handle();
    mouseHandlers.handle();

    double thisTime = glfwGetTime();
    float dtime = thisTime - lastTime;
    lastTime = thisTime;
    if (!play) {
      dtime = 0;
    }

    boids.update(dtime);

    // render
    // ------
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shaderProgram.setMatrix("view", camera.getViewMat());
    target.draw(shaderProgram);

    // Draw cursor
    glClear(GL_DEPTH_BUFFER_BIT);
    cursor.draw(hudShader);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();

  return 0;
}