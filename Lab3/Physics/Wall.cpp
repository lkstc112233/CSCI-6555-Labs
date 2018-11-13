#include "Wall.h"

#include <glm/glm.hpp>

Wall::Wall(glm::vec3 positioni, glm::vec3 normali)
    : position(positioni), normal(normali) {}

glm::vec3 Wall::applyForce(glm::vec3 pos) {}