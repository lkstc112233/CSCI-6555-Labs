#include "Entity.h"

void Entity::draw(ShaderProgram& shader) {
  for (auto& p : objects) {
    p.second->draw(shader);
  }
}