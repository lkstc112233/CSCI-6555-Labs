#include "World.h"

void World::emplaceControllers(Entity& entity) {
  for (auto e : entity) {
    controllers.emplace_back(e);
  }
}