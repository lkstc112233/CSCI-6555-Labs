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

#include "Interface/InterfaceRelatedStuffHolder.hpp"
#include "Interface/KeyHandler.h"
#include "Interface/MouseHandler.h"
#include "Interface/ProgressBar.h"
#include "Interface/Utilities/Button.h"

#include "Animate/Keyframe.hpp"
#include "Animate/Scripts.h"
#include "Math/Quaternion.h"

const float PROJECTION_RATIO = float(SCREEN_WIDTH) / SCREEN_HEIGHT;

int main(int argc, char** argv) {
  if (argc < 3) {
    std::cerr << "Usage:\n\t" << argv[0] << " *model.off* *script.keys*"
              << std::endl;
    return -1;
  }

  auto script = Scripts::loadScript(argv[2]);

  GLFWwindow* window = initializeWindow("Lab1");
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

  Entity entity;
  entity.addObject("object", ModelLoader::loadOffFile(argv[1]));

  ShaderProgram shaderProgram{
      Shader::createVertexShader("res/shaders/simpleShader.vert"),
      Shader::createFragmentShader("res/shaders/simpleShader.frag")};
  if (!shaderProgram.isValid()) {
    return -4;
  }
  ShaderProgram keyframeShader{
      Shader::createVertexShader("res/shaders/keyShader.vert"),
      Shader::createFragmentShader("res/shaders/simpleShader.frag")};
  glm::mat4 projection(1.0f);
  projection =
      glm::perspective(glm::radians(45.0f), PROJECTION_RATIO, 0.1f, 100.0f);
  shaderProgram.setMatrix("projection", projection);
  keyframeShader.setMatrix("projection", projection);

  Camera camera;

  KeyHandlerContainer keyHandlers(window);

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
  ProgressBar progressBar(script);
  progressBar.attachControls(keyHandlers, mouseHandlers);

  while (!glfwWindowShouldClose(window)) {
    keyHandlers.handle();
    mouseHandlers.handle();

    double thisTime = glfwGetTime();
    if (progressBar.isPlaying()) {
      progressBar.addProcess((thisTime - lastTime) / script.getMaximumTime());
    }
    lastTime = thisTime;

    // render
    // ------
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shaderProgram.setMatrix("view", camera.getViewMat());
    float currentTime = progressBar.getProcess() * script.getMaximumTime();
    auto& obj = *entity.getObject("object");
    obj.setTransformX(script.getXAt(currentTime));
    obj.setTransformY(script.getYAt(currentTime));
    obj.setTransformZ(script.getZAt(currentTime));
    obj.setOrientation(script.getOrientationAt(currentTime));

    entity.draw(shaderProgram);

    // Draw HDR
    glClear(GL_DEPTH_BUFFER_BIT);
    progressBar.draw(hudShader);

    // Draw cursor
    glClear(GL_DEPTH_BUFFER_BIT);
    cursor.draw(hudShader);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();

  return 0;
}