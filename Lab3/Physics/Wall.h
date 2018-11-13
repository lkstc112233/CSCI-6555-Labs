#ifndef PHYSICS_WALL_H
#define PHYSICS_WALL_H

#include <glm/glm.hpp>

class Wall {
 private:
  glm::vec3 position;
  glm::vec3 normal;

 public:
  Wall(glm::vec3 position, glm::vec3 normal);
  glm::vec3 applyForce(glm::vec3 position);
};

#endif  // PHYSICS_WALL_H