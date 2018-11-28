#ifndef ANIMATE_BOID_H
#define ANIMATE_BOID_H

#include <vector>

#include <glm/glm.hpp>

#include "../Graphics/Object/Object.h"
#include "../Graphics/Shaders/Shader.h"

class Boid {
 private:
  Object3D& object;
  Object3D& target;

 public:
  Boid(Object3D& object, Object3D& target);
  void update(float time);
  void draw(ShaderProgram& shader);
};

class Boids {
 private:
  std::vector<Boid> boids;

 public:
  template <typename... Args>
  void addBoid(Args... args) {
    boids.emplace_back(args...);
  }
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