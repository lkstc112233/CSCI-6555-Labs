#include "Object.h"

#include <memory>

#include <glm/glm.hpp>

#include "../../Animate/Timeline.hpp"

Object3D::Object3D(const Model& modeli) : model(modeli) {
  if (modeli.getDimensions() != 3) {
    throw "INCAPABLE_DIMENSION";
  }
}

void Object3D::setParent(const std::shared_ptr<Object3D>& parenti) {
  parent = parenti;
}

void Object3D::updateManagers(float time) {
  for (auto& m : managers) {
    m.second->handleDataAt(time);
  }
}

void Object3D::moveTo(glm::vec3 position) {
  transformX = position.x;
  transformY = position.y;
  transformZ = position.z;
}

void Object3D::moveBy(glm::vec3 distance) {
  transformX += distance.x;
  transformY += distance.y;
  transformZ += distance.z;
}

void Object3D::setOpacity(float opacityi) { opacity = opacityi; }

glm::mat4 Object3D::getTransformationMatrix() {
  glm::mat4 center(1.0F);
  center[3][0] = -centerX;
  center[3][1] = -centerY;
  center[3][2] = -centerZ;
  glm::mat4 rotate(orientation.getRotationMatrix());
  rotate[3][3] = 1.0F;
  glm::mat4 transform(1.0F);
  transform[3][0] = transformX;
  transform[3][1] = transformY;
  transform[3][2] = transformZ;
  transform[3][3] = 1.0F;
  glm::mat4 parentTransform(1.0F);
  auto usingParent = parent.lock();
  if (usingParent) {
    parentTransform = usingParent->getTransformationMatrix();
  }
  return parentTransform * transform * rotate * center;
}

void Object3D::draw(ShaderProgram& shader) {
  glm::mat4 scale(1.0F);
  scale[0][0] = scaleX;
  scale[1][1] = scaleY;
  scale[2][2] = scaleZ;
  shader.setMatrix("transform", getTransformationMatrix() * scale);
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
