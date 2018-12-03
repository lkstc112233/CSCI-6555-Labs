#include "Boid.h"

#include <glm/glm.hpp>

#include "../Math/Quaternion.h"

Boid::Boid(Object3D& objecti, const Object3D& targeti)
    : object(objecti), target(targeti) {}

void Boid::setDirection(glm::vec3 directioni) {
  direction = glm::normalize(directioni);
}

void Boid::update(float time) { rotation += ANGULAR_VELOCITY * time; }

void Boid::draw(ShaderProgram& shader) {
  // Rotate the boid itself as rotation indicates
  Quaternion s(cos(rotation), 0, 0, sin(rotation));
  // Rotate the boid to the desired direction:
  glm::vec3 vectorPart = glm::cross(glm::vec3(0, 0, 1), direction);
  float realPart = glm::dot(glm::vec3(0, 0, 1), direction) + 1;
  Quaternion q(realPart, vectorPart.x, vectorPart.y, vectorPart.z);
  q.normalize();
  q *= s;
  q.normalize();
  object.setOrientation(q);

  object.moveTo(getPosition());
  object.draw(shader);
}

glm::vec3 Boids::getCenterNear(glm::vec3 center) {
  glm::vec3 result(0);
  int counter = 0;
  for (auto& boid : boids) {
    auto pos = boid->getPosition();
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