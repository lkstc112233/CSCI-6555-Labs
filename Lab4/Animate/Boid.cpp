#include "Boid.h"

Boid::Boid(Object3D& objecti, const Object3D& targeti)
    : object(objecti), target(targeti) {}

void Boid::setDirection(glm::vec3 directioni) {
  direction = glm::normalize(directioni);
}
void Boid::update(float time) {}

void Boid::draw(ShaderProgram& shader) {
  object.moveTo(position);
  object.draw(shader);
}

glm::vec3 Boids::getCenterNear(glm::vec3 center) {
  glm::vec3 result(0);
  int counter = 0;
  for (auto& boid : boids) {
    auto pos = boid.getPosition();
    if (glm::distance(pos, center) < sight) {
      counter += 1;
      result += pos;
    }
  }
  if (counter > 0) {
    result /= counter;
  }
  return result;
}