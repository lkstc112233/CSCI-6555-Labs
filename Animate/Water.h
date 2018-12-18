#ifndef ANIMATE_WATER_H
#define ANIMATE_WATER_H

#include <map>
#include <memory>
#include <utility>
#include <vector>

#include "../Graphics/Models/Model.h"
#include "../Graphics/Object/Object.h"

class Water {
 private:
  class WaterNodes {
   public:
    glm::vec3 position;
    glm::vec3 speed;
  };
  std::vector<std::vector<WaterNodes>> waterNodes;
  const int waterSize;
  std::map<std::pair<int, int>, std::pair<int, float>> toPoke;
  const Model& waterTriangle;
  void drawAt(ShaderProgram& shader, glm::vec3 point1, glm::vec3 point2,
              glm::vec3 point3, glm::vec3 point4);

 public:
  constexpr static const int POKE_INCRAESEMENT = 5;
  constexpr static const float POKE_FORCE = 1;
  Water(int waterSize = 10);
  void poke(int x, int y, int increasement = POKE_INCRAESEMENT,
            float force = POKE_FORCE);
  void update(float time);
  void draw(ShaderProgram&);
};

#endif  // ANIMATE_WATER_H