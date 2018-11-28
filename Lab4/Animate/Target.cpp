#include "Target.h"

#include <utility>

Target::Target(Object3D&& object_operating)
    : object(std::move(object_operating)) {}

void Target::draw(ShaderProgram& shader) {
  // TODO: put the object to correct position.
  object.draw(shader);
}