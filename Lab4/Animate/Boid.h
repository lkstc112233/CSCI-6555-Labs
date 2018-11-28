#ifndef ANIMATE_BOID_H
#define ANIMATE_BOID_H

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

#endif  // ANIMATE_BOID_H