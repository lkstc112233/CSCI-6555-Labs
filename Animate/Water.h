#ifndef ANIMATE_WATER_H
#define ANIMATE_WATER_H

#include <memory>
#include <vector>

#include "../Graphics/Models/Model.h"
#include "../Graphics/Object/Object.h"

class Water {
 private:
  const Model& waterTriangle;
  void drawAt(ShaderProgram& shader, glm::vec3 point1, glm::vec3 point2,
              glm::vec3 point3, glm::vec3 point4);

 public:
  Water();
  void update(float time);
  void draw(ShaderProgram&);
};

#endif  // ANIMATE_WATER_H