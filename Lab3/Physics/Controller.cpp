#include "Controller.h"

#include <memory>

#include "../Graphics/Object/Object.h"

Controller::Controller(std::shared_ptr<Object3D> object)
    : handlingObject(object) {}

void Controller::applyChange(float timeSpan) {
  handlingObject.lock()->moveBy(speed * timeSpan);
}