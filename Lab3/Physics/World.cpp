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
  // Check wall colliding
  for (auto& w : walls) {
    for (auto& c : controllers) {
      c.speed += t * w.applyForce(c.position) / c.mass;
    }
  }
  // Check balls colliding
  for (auto i = controllers.begin(); i != controllers.end(); ++i) {
    for (auto j = i + 1; j != controllers.end(); ++j) {
      if (glm::distance(i->position, j->position) < 2) {
        // Bounce
        auto norm = i->position - j->position;
        auto projection = [norm](glm::vec3 vec) {
          float mag = glm::dot(vec, norm);
          return mag / glm::length(norm) * norm;
        };
        auto differ_i_v = projection(i->speed);
        auto differ_j_v = projection(j->speed);
        i->speed += differ_j_v;
        i->speed -= differ_i_v;
        j->speed += differ_i_v;
        j->speed -= differ_j_v;
      }
    }
  }
  for (auto& c : controllers) {
    c.applyChange(t);
  }
}