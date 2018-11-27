#include "Controller.h"

#include <memory>

#include "../Graphics/Object/Object.h"

Controller::Controller(std::shared_ptr<Object3D> object)
    : handlingObject(object) {
  position.x = object->getTransformX();
  position.y = object->getTransformY();
  position.z = object->getTransformZ();
}

void Controller::applyChange(float timeSpan) {
  auto object = handlingObject.lock();
  position += speed * timeSpan;
  if (object) {
    object->moveTo(position);
  }
}