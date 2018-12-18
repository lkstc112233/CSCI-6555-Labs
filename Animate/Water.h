#ifndef ANIMATE_WATER_H
#define ANIMATE_WATER_H

#include <memory>
#include <vector>

#include "../Graphics/Models/Model.h"
#include "../Graphics/Object/Object.h"

class Water {
 private:
  class WaterNodes {
   public:
    glm::vec3 position;
  };
  std::vector<std::vector<WaterNodes>> waterNodes;
  const int waterSize;
  const Model& waterTriangle;
  void drawAt(ShaderProgram& shader, glm::vec3 point1, glm::vec3 point2,
              glm::vec3 point3, glm::vec3 point4);

 public:
  Water(int waterSize = 10);
  void update(float time);
  void draw(ShaderProgram&);
};

#endif  // ANIMATE_WATER_H