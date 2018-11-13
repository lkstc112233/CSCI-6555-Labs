#include "Controller.h"

#include <memory>

#include "../Graphics/Object/Object.h"

Controller::Controller(std::shared_ptr<Object3D> object)
    : handlingObject(object) {}

void Controller::applyChange(float timeSpan) {
  auto object = handlingObject.lock();
  position += speed * timeSpan;
  if (object) {
    object->moveTo(position);
  }
}