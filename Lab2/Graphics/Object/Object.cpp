#include "Object.h"

#include <glm/glm.hpp>

Object3D::Object3D(const Model& modeli) : model(modeli), transform(1.0F) {
  if (modeli.getDimensions() != 3) {
    throw "INCAPABLE_DIMENSION";
  }
}

void Object3D::setTransformMatrix(const glm::mat4& transformi) {
  transform = transformi;
}

void Object3D::setOpacity(float opacityi) { opacity = opacityi; }

void Object3D::draw(ShaderProgram& shader) {
  shader.setMatrix("transform", transform);
  shader.setValue("opacity", opacity);
  model.draw(shader);
}

Object2D::Object2D(const Model& modeli)
    : model(modeli), transform(1.0F), color(1.0F), z(0) {
  if (modeli.getDimensions() != 2) {
    throw "INCAPABLE_DIMENSION";
  }
}

void Object2D::setTransformMatrix(const glm::mat3& transformi) {
  transform = transformi;
}

void Object2D::setColor(const glm::vec3& colori) { color = colori; }

void Object2D::setR(float r) { color.r = r; }

void Object2D::setG(float g) { color.g = g; }

void Object2D::setB(float b) { color.b = b; }

void Object2D::setZ(float zi) { z = zi; }

void Object2D::setOpacity(float opacityi) { opacity = opacityi; }

void Object2D::draw(ShaderProgram& shader) {
  shader.setMatrix("transform", transform);
  shader.setVector("color", color);
  shader.setValue("z", z);
  shader.setValue("opacity", opacity);
  model.draw(shader);
}
