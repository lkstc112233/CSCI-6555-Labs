#ifndef ANIMATE_BOID_H
#define ANIMATE_BOID_H

#include <vector>

#include <glm/glm.hpp>

#include "../Graphics/Object/Object.h"
#include "../Graphics/Shaders/Shader.h"

class Boid {
 private:
  Object3D& object;
  const Object3D& target;
  glm::vec3 position;

 public:
  Boid(Object3D& object, const Object3D& target);
  void update(float time);
  void draw(ShaderProgram& shader);
  glm::vec3 getPosition() { return position; }
};

class Boids {
 private:
  std::vector<Boid> boids;
  constexpr static const float sight = 50;

 public:
  template <typename... Args>
  Boid& addBoid(Args&... args) {
    return boids.emplace_back(args...);
  }
  glm::vec3 getCenterNear(glm::vec3 center);
  void update(float time) {
    for (auto& boid : boids) {
      boid.update(time);
    }
  }
  void draw(ShaderProgram& shader) {
    for (auto& boid : boids) {
      boid.draw(shader);
    }
  }
};

#endif  // ANIMATE_BOID_H