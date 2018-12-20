#ifndef ANIMATE_BOID_H
#define ANIMATE_BOID_H

#include <memory>
#include <vector>

#include <glm/glm.hpp>

#include "../Graphics/Entity/Entity.h"
#include "../Graphics/Object/Object.h"
#include "../Graphics/Shaders/Shader.h"

class Boid {
 private:
  constexpr static const float ANGULAR_VELOCITY = 0.35;
  constexpr static const float VELOCITY = 5;
  constexpr static const float FEED_ONCE = 500;

  Object3D& object;
  Entity& baits;
  glm::vec3 position;
  glm::vec3 direction;
  float rotation = 0;
  int feed;

  glm::vec3 decision(glm::vec3 nearbyCenter,
                     std::vector<glm::vec3>& nearbyBoids);

 public:
  Boid(Object3D& object, Entity& target);
  void update(float time, glm::vec3 nearbyCenter,
              std::vector<glm::vec3>& nearbyBoids);
  void draw(ShaderProgram& shader);
  glm::vec3 getPosition() { return position; }
  void setPosition(glm::vec3 positioni) { position = positioni; }
  void setDirection(glm::vec3 direction);
};

class Boids {
 private:
  std::vector<std::unique_ptr<Boid>> boids;
  constexpr static const float sight = 20;
  constexpr static const float NEARBY_BOID_DISTANCE_THRESHOLD = 1;

 public:
  std::vector<std::pair<int, int>> pokes;
  template <typename... Args>
  Boid& addBoid(Args&... args) {
    return *boids.emplace_back(std::make_unique<Boid>(args...));
  }
  glm::vec3 getCenterNear(glm::vec3 center);
  void update(float time);
  void draw(ShaderProgram& shader) {
    for (auto& boid : boids) {
      boid->draw(shader);
    }
  }
};

#endif  // ANIMATE_BOID_H