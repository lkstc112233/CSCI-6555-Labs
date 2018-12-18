#include "Water.h"

Water::Water(int waterSizeIn)
    : waterTriangle(ModelLoader::getUnitTriangleShape()),
      waterSize(waterSizeIn) {
  for (int i = -waterSize; i <= waterSize; ++i) {
    auto& row = waterNodes.emplace_back();
    for (int j = -waterSize; j <= waterSize; ++j) {
      row.emplace_back().position = glm::vec3(i, 0, j);
    }
  }
}

void Water::drawAt(ShaderProgram& shader, glm::vec3 point1, glm::vec3 point2,
                   glm::vec3 point3, glm::vec3 point4) {
  glm::vec3 center = (point1 + point2 + point3 + point4) / 4.0F;
  glm::mat4 points(1.0F);
  points[0] = glm::vec4(point1, 0);
  points[1] = glm::vec4(point2, 0);
  points[2] = glm::vec4(center, 0);
  shader.setMatrix("transform", points);
  waterTriangle.draw(shader);
  points[0] = glm::vec4(point2, 0);
  points[1] = glm::vec4(point3, 0);
  points[2] = glm::vec4(center, 0);
  shader.setMatrix("transform", points);
  waterTriangle.draw(shader);
  points[0] = glm::vec4(point3, 0);
  points[1] = glm::vec4(point4, 0);
  points[2] = glm::vec4(center, 0);
  shader.setMatrix("transform", points);
  waterTriangle.draw(shader);
  points[0] = glm::vec4(point4, 0);
  points[1] = glm::vec4(point1, 0);
  points[2] = glm::vec4(center, 0);
  shader.setMatrix("transform", points);
  waterTriangle.draw(shader);
}

void Water::draw(ShaderProgram& shader) {
  for (int i = 0; i < waterNodes.size() - 1; ++i) {
    for (int j = 0; j < waterNodes[0].size() - 1; ++j) {
      drawAt(shader, waterNodes[i][j].position, waterNodes[i + 1][j].position,
             waterNodes[i + 1][j + 1].position, waterNodes[i][j + 1].position);
    }
  }
}
