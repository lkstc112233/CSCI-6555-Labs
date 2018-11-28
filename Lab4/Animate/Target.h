#ifndef ANIMATE_TARGET_H
#define ANIMATE_TARGET_H

#include <glm/glm.hpp>

#include "../Graphics/Object/Object.h"
#include "../Graphics/Shaders/Shader.h"

class Target {
 private:
  Object3D object;

 public:
  Target(Object3D&& object);
  void draw(ShaderProgram& shader);
};

#endif  // ANIMATE_TARGET_H