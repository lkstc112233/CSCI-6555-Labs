#include "Wall.h"

#include <glm/glm.hpp>

Wall::Wall(glm::vec3 positioni, glm::vec3 normali)
    : position(positioni), normal(normali) {}

const static float k = 100.0;

glm::vec3 Wall::applyForce(glm::vec3 pos) {
  auto dpos = position - pos;
  float force = glm::dot(dpos, normal);
  if (force <= 0) {
    return glm::vec3(0);
  }
  // F = kx
  return k * force / glm::length(normal) * normal;
}