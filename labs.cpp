// standard
#include <cassert>
#include <cmath>
#include <iostream>
#include <random>

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
#include "Animate/Character.h"
#include "Animate/Keyframe.hpp"
#include "Animate/Scripts.h"
#include "Animate/Timeline.hpp"
#include "Animate/Water.h"
#include "Math/Quaternion.h"

const static int BOIDS_COUNT = 10;

const float PROJECTION_RATIO = float(SCREEN_WIDTH) / SCREEN_HEIGHT;

int main(int argc, char** argv) {
  GLFWwindow* window = initializeWindow("Labs");
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

  Character character;

  Boids boids;
  Object3D target(ModelLoader::loadOffFile("res/models/ball.off"));
  Object3D boid(ModelLoader::loadOffFile("res/models/arrow.off"));
  boid.setScale(0.2);
  boid.setScaleZ(0.5);

  Entity stones;
  World world;

  // Add random generated boids.
  std::default_random_engine randomGenerator;
  std::normal_distribution boidPositionDistributor(0.F, 10.F);
  std::uniform_real_distribution boidDirectionDistributor(-1.F, 1.F);
  for (int i = 0; i < BOIDS_COUNT; ++i) {
    auto& managingBoid = boids.addBoid(boid, target);
    managingBoid.setPosition(
        glm::vec3(boidPositionDistributor(randomGenerator),
                  boidPositionDistributor(randomGenerator),
                  boidPositionDistributor(randomGenerator)));
    managingBoid.setDirection(
        glm::normalize(glm::vec3(boidDirectionDistributor(randomGenerator),
                                 boidDirectionDistributor(randomGenerator),
                                 boidDirectionDistributor(randomGenerator))));
  }

  ShaderProgram shaderProgram{
      Shader::createVertexShader("res/shaders/simpleShader.vert"),
      Shader::createFragmentShader("res/shaders/simpleShader.frag")};
  if (!shaderProgram.isValid()) {
    return -4;
  }
  ShaderProgram boidShaderProgram{
      Shader::createVertexShader("res/shaders/simpleLightShader.vert"),
      Shader::createFragmentShader("res/shaders/simpleLightShader.frag")};
  if (!boidShaderProgram.isValid()) {
    return -4;
  }
  ShaderProgram waterShaderProgram{
      Shader::createVertexShader("res/shaders/waterShader.vert"),
      Shader::createFragmentShader("res/shaders/waterShader.frag")};
  if (!waterShaderProgram.isValid()) {
    return -4;
  }
  glm::mat4 projection(1.0f);
  projection =
      glm::perspective(glm::radians(45.0f), PROJECTION_RATIO, 0.1f, 1000.0f);
  shaderProgram.setMatrix("projection", projection);
  boidShaderProgram.setMatrix("projection", projection);
  waterShaderProgram.setMatrix("projection", projection);

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
  keyHandlers.emplace_handler(
      GLFW_KEY_B,
      [&]() {
        auto& managingBoid = boids.addBoid(boid, target);
        managingBoid.setPosition(
            glm::vec3(boidPositionDistributor(randomGenerator),
                      boidPositionDistributor(randomGenerator),
                      boidPositionDistributor(randomGenerator)));
        managingBoid.setDirection(glm::normalize(
            glm::vec3(boidDirectionDistributor(randomGenerator),
                      boidDirectionDistributor(randomGenerator),
                      boidDirectionDistributor(randomGenerator))));
      },
      true);
  // character
  auto stoneModel = ModelLoader::loadOffFile("res/models/ball.off");
  {
    keyHandlers.emplace_handler(
        GLFW_KEY_1, [&character]() { character.throwStone(0.1); }, false,
        [&]() {
          static int id = 0;
          auto throwResult = character.throwStone();
          if (throwResult.second) {
            auto stone = throwResult.first;
            std::string name = std::to_string(id++);
            stones.addObject(name, stoneModel);
            auto stoneAdded = stones.getObject(name);
            stoneAdded->setTransformX(stone.first.x);
            stoneAdded->setTransformY(stone.first.y);
            stoneAdded->setTransformZ(stone.first.z);
            world.addController(stoneAdded, stone.second);
          }
        });
  }

  constexpr static const int WATER_SIZE = 20;
  Water water(WATER_SIZE);

  std::uniform_int_distribution waterPokingDistributor(-9, 9);
  keyHandlers.emplace_handler(GLFW_KEY_O,
                              [&]() {
                                water.poke(
                                    waterPokingDistributor(randomGenerator),
                                    waterPokingDistributor(randomGenerator));
                              },
                              true);

  ShaderProgram hudShader{
      Shader::createVertexShader("res/shaders/2DShader.vert"),
      Shader::createFragmentShader("res/shaders/2DShader.frag")};

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

  ProgressBar progressBar;
  progressBar.setProcess(0.5);
  progressBar.attachControls(keyHandlers, mouseHandlers);

  while (!glfwWindowShouldClose(window)) {
    keyHandlers.handle();
    mouseHandlers.handle();

    double thisTime = glfwGetTime();
    float dtime = thisTime - lastTime;
    if (progressBar.isPlaying()) {
      dtime *= progressBar.getProcess() / 0.5;
    } else {
      dtime = 0;
    }
    lastTime = thisTime;

    boids.update(dtime);
    for (auto& p : boids.pokes) {
      if (abs(p.first) < WATER_SIZE && abs(p.second) < WATER_SIZE) {
        water.poke(p.first, p.second, 1, 0.3);
      }
    }
    world.timePass(dtime);
    water.update(dtime);
    character.update(dtime);

    // render
    // ------
    glClearColor(0.1f, 0.15f, 0.15f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shaderProgram.setMatrix("view", camera.getViewMat());
    target.draw(shaderProgram);

    boidShaderProgram.setMatrix("view", camera.getViewMat());
    boidShaderProgram.setVector("viewPos", camera.position);
    boidShaderProgram.setVector(
        "lightPos", glm::vec3(target.getTransformX(), target.getTransformY(),
                              target.getTransformZ()));
    boids.draw(boidShaderProgram);

    character.draw(shaderProgram);

    stones.draw(shaderProgram);

    waterShaderProgram.setMatrix("view", camera.getViewMat());
    water.draw(waterShaderProgram);

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