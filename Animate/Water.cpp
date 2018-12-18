#include "Water.h"

Water::Water() : waterTriangle(ModelLoader::getUnitTriangleShape()) {}

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
  drawAt(shader, glm::vec3(1, 0, 1), glm::vec3(2, 0, 1), glm::vec3(2, 0, 2),
         glm::vec3(1, 0, 2));
}
