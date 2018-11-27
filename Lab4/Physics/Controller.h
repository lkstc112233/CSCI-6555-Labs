#ifndef PHYSICS_CONTROLLER_H
#define PHYSICS_CONTROLLER_H

#include <glm/glm.hpp>
#include <memory>

#include "../Graphics/Object/Object.h"

class Controller {
 private:
  std::weak_ptr<Object3D> handlingObject;

 public:
  Controller(std::shared_ptr<Object3D> object);
  glm::vec3 speed;
  glm::vec3 position;
  float mass = 1;
  void applyChange(float timeSpan);
};

#endif  // PHYSICS_CONTROLLER_H