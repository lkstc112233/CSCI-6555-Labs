#ifndef ANIMATE_WATER_H
#define ANIMATE_WATER_H

#include <memory>
#include <vector>

#include "../Graphics/Shaders/Shader.h"

class Water {
 private:
  ShaderProgram shader;

 public:
  Water();
  void update(float time);
  void draw();
};

#endif  // ANIMATE_WATER_H