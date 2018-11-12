#include "World.h"

#include <glm/glm.hpp>

const glm::vec3 World::g = glm::vec3(0.0F, 0.1F, 0.0F);

void World::emplaceControllers(Entity& entity) {
  for (auto e : entity) {
    controllers.emplace_back(e);
  }
}