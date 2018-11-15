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

#include "Animate/Keyframe.hpp"
#include "Animate/Scripts.h"
#include "Animate/Timeline.hpp"
#include "Math/Quaternion.h"

const float PROJECTION_RATIO = float(SCREEN_WIDTH) / SCREEN_HEIGHT;

int main(int argc, char** argv) {
  GLFWwindow* window = initializeWindow("Lab3");
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
  // Build some ball-models
  entity.addObject("ball1", ModelLoader::loadOffFile("res/models/ball.off"));
  entity.addObject("ball2", ModelLoader::loadOffFile("res/models/ball.off"));
  entity.addObject("ball3", ModelLoader::loadOffFile("res/models/ball.off"));
  entity.addObject("ball4", ModelLoader::loadOffFile("res/models/ball.off"));
  entity.addObject("ball5", ModelLoader::loadOffFile("res/models/ball.off"));
  entity.addObject("ball6", ModelLoader::loadOffFile("res/models/ball.off"));

  {
    auto b = entity.getObject("ball1");
    b->setTransformX(0);
    b->setTransformY(0);
    b->setTransformZ(10);
  }
  {
    auto b = entity.getObject("ball2");
    b->setTransformX(4);
    b->setTransformY(2);
    b->setTransformZ(10);
  }
  {
    auto b = entity.getObject("ball3");
    b->setTransformX(1);
    b->setTransformY(2);
    b->setTransformZ(10);
  }
  {
    auto b = entity.getObject("ball4");
    b->setTransformX(7);
    b->setTransformY(15);
    b->setTransformZ(3);
  }
  {
    auto b = entity.getObject("ball5");
    b->setTransformX(-5);
    b->setTransformY(4);
    b->setTransformZ(6);
  }
  {
    auto b = entity.getObject("ball6");
    b->setTransformX(2);
    b->setTransformY(0);
    b->setTransformZ(7);
  }

  Entity environment;
  environment.addObject("floor",
                        ModelLoader::loadOffFile("res/models/surface.off"));
  environment.addObject("floor2",
                        ModelLoader::loadOffFile("res/models/surface.off"));
  {
    auto f = environment.getObject("floor2");
    f->setScale(100);
    f->setTransformY(-35);
    f->setOrientation(Quaternion(0.653282, 0.653282, 0.2705967, 0.2705967));
  }
  {
    auto f = environment.getObject("floor");
    f->setScale(100);
    f->setTransformY(-35);
    f->setOrientation(Quaternion(0.7071068, 0.7071068, 0, 0));
  }

  World world;
  world.emplaceControllers(entity);
  world.addWall(glm::vec3(0, -30, 0), glm::vec3(0, 1, 0));
  world.addWall(glm::vec3(0, -30, 0), glm::vec3(-1, 1, 0));

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

  KeyHandlerContainer keyHandlers(window);
  camera.position = glm::vec3(0, 0, -100);

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

    world.timePass(dtime);

    // render
    // ------
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shaderProgram.setMatrix("view", camera.getViewMat());
    entity.draw(shaderProgram);
    environment.draw(shaderProgram);

    // Draw cursor
    glClear(GL_DEPTH_BUFFER_BIT);
    cursor.draw(hudShader);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();

  return 0;
}