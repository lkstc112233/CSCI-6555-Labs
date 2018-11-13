#include "World.h"

#include <glm/glm.hpp>

const glm::vec3 World::g = glm::vec3(0.0F, -5.0F, 0.0F);

void World::emplaceControllers(Entity& entity) {
  for (auto e : entity) {
    controllers.emplace_back(e);
  }
}

void World::timePass(float t) {
  for (auto& c : controllers) {
    c.speed += t * g;
  }
  for (auto& w : walls) {
    for (auto& c : controllers) {
      c.speed += t * w.applyForce(c.position) / c.mass;
    }
  }
  for (auto& c : controllers) {
    c.applyChange(t);
  }
}