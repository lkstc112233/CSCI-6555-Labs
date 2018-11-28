#include "Boid.h"

Boid::Boid(Object3D& object_operating) : object(object_operating) {}

void Boid::draw(ShaderProgram& shader) {
  // TODO: put the object to correct position.
  object.draw(shader);
}