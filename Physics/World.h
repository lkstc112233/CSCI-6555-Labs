#ifndef PHYSICS_WORLD_H
#define PHYSICS_WORLD_H

#include <vector>

#include "../Graphics/Entity/Entity.h"

#include "Controller.h"
#include "Wall.h"

class World {
 private:
  std::vector<Controller> controllers;
  std::vector<Wall> walls;
  static const glm::vec3 g;

 public:
  template <typename... Args>
  void addWall(Args... args) {
    walls.emplace_back(args...);
  }
  void emplaceControllers(Entity& entity);
  void addControllers(std::shared_ptr<Object3D>, glm::vec3 speed);
  void timePass(float t);
};

#endif  // PHYSICS_WORLD_H