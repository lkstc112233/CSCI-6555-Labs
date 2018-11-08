#include "Entity.h"

#include "../Object/Object.h"

void Entity::updateManagers(float time) {
  for (auto& p : objects) {
    p.second->updateManagers(time);
  }
}

void Entity::draw(ShaderProgram& shader) {
  for (auto& p : objects) {
    p.second->draw(shader);
  }
}