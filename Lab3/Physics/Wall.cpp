#include "Wall.h"

#include <glm/glm.hpp>

Wall::Wall(glm::vec3 positioni, glm::vec3 normali)
    : position(positioni), normal(normali) {}

glm::vec3 Wall::applyForce(glm::vec3 pos) {
  auto dpos = position - pos;
  float force = -glm::dot(dpos, normal);
  if (force < 0) {
    return glm::vec3(0);
  }
  return -force / glm::length(normal) * normal;
}