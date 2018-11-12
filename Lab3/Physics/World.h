#ifndef PHYSICS_WORLD_H
#define PHYSICS_WORLD_H

#include <vector>

#include "../Graphics/Entity/Entity.h"

#include "Controller.h"

class World {
 private:
  std::vector<Controller> controllers;
  static const glm::vec3 g;

 public:
  void emplaceControllers(Entity& entity);
};

#endif  // PHYSICS_WORLD_H