#include "Boid.h"

Boid::Boid(Object3D& objecti, Object3D& targeti)
    : object(objecti), target(targeti) {}

void Boid::update(float time) {}

void Boid::draw(ShaderProgram& shader) {
  object.moveTo(position);
  object.draw(shader);
}