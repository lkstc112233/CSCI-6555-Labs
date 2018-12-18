#include "Water.h"

Water::Water() : waterTriangle(ModelLoader::getUnitTriangleShape()) {}

void Water::drawAt(ShaderProgram& shader, int x, int z) {
  glm::vec4 point1(x, 0, z, 0);
  glm::vec4 point2(x + 1, 0, z, 0);
  glm::vec4 point3(x + 1, 0, z + 1, 0);
  glm::mat4 points(1.0F);
  points[0] = point1;
  points[1] = point2;
  points[2] = point3;
  shader.setMatrix("transform", points);
  //   shader.setValue("opacity", 0.3F);
  waterTriangle.draw(shader);
}

void Water::draw(ShaderProgram& shader) { drawAt(shader, 1, 2); }
