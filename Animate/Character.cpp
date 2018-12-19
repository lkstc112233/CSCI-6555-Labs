#include "Character.h"

Character::Character()
    : time(0),
      throwing(false),
      waveRate(0),
      script(Scripts::loadScript("res/scripts/lab2.keys")),
      model(ModelLoader::loadOffFile("res/models/cube.off")),
      modelArrow(ModelLoader::loadOffFile("res/models/arrow.off")) {
  // Build a human-model
  entity.addObject("pelvis", model);
  entity.addChild("pelvis", "chest", model);
  entity.addChild("pelvis", "thigh-left", model);
  entity.addChild("pelvis", "thigh-right", model);
  entity.addChild("thigh-left", "calf-left", model);
  entity.addChild("thigh-right", "calf-right", model);
  entity.addChild("chest", "upper-arm-left", model);
  entity.addChild("chest", "upper-arm-right", model);
  entity.addChild("upper-arm-left", "fore-arm-left", model);
  entity.addChild("upper-arm-right", "fore-arm-right", model);
  entity.addChild("chest", "arrow", modelArrow);
  {
    auto pelvis = entity.getObject("pelvis");
    pelvis->setScale(0.2);
    pelvis->setScaleY(1.2);
    pelvis->setCenterY(-0.4);
    pelvis->setTransformXManager(script.getFloatTimeline("xPos"));
    pelvis->setTransformYManager(script.getFloatTimeline("yPos"));
    pelvis->setTransformZManager(script.getFloatTimeline("zPos"));
    pelvis->setOrientationManager(script.getQuaternionTimeline("orientation"));
  }
  {
    auto thigh = entity.getObject("thigh-left");
    thigh->setScale(0.2);
    thigh->setScaleY(1);
    thigh->setCenterY(1);
    thigh->setTransformX(0.4);
    thigh->setTransformY(-0.8);
    thigh->setOrientationManager(script.getQuaternionTimeline("thigh-walking"),
                                 2);
  }
  {
    auto thigh = entity.getObject("thigh-right");
    thigh->setScale(0.2);
    thigh->setScaleY(1);
    thigh->setCenterY(1);
    thigh->setTransformX(-0.4);
    thigh->setTransformY(-0.8);
    thigh->setOrientationManager(script.getQuaternionTimeline("thigh-walking"));
  }
  {
    auto calf = entity.getObject("calf-left");
    calf->setScale(0.2);
    calf->setScaleY(1);
    calf->setCenterY(1);
    calf->setTransformY(-1.2);
    calf->setOrientationManager(script.getQuaternionTimeline("calf-walking"),
                                2);
  }
  {
    auto calf = entity.getObject("calf-right");
    calf->setScale(0.2);
    calf->setScaleY(1);
    calf->setCenterY(1);
    calf->setTransformY(-1.2);
    calf->setOrientationManager(script.getQuaternionTimeline("calf-walking"));
  }
  {
    auto chest = entity.getObject("chest");
    chest->setScale(0.2);
    chest->setScaleY(1.8);
    chest->setCenterY(-1.8);
    chest->setTransformY(1.6);
  }
  {
    auto arm = entity.getObject("upper-arm-left");
    arm->setScale(0.2);
    arm->setScaleY(1);
    arm->setCenterY(1);
    arm->setTransformX(0.4);
    arm->setTransformY(0.8);
    arm->setOrientationManager(
        script.getQuaternionTimeline("upper-arm-walking"), 2);
  }
  {
    auto arm = entity.getObject("upper-arm-right");
    arm->setScale(0.2);
    arm->setScaleY(1);
    arm->setCenterY(1);
    arm->setTransformX(-0.4);
    arm->setTransformY(0.8);
    arm->setOrientationManager(
        {script.getQuaternionTimeline("upper-arm-walking"),
         script.getQuaternionTimeline("upper-arm-waving")});
  }
  {
    auto arm = entity.getObject("fore-arm-left");
    arm->setScale(0.2);
    arm->setScaleY(1);
    arm->setCenterY(1);
    arm->setTransformY(-1.2);
    arm->setOrientationManager(script.getQuaternionTimeline("fore-arm-walking"),
                               2);
  }
  {
    auto arm = entity.getObject("fore-arm-right");
    arm->setScale(0.2);
    arm->setScaleY(1);
    arm->setCenterY(1);
    arm->setTransformY(-1.2);
    arm->setOrientationManager(
        {script.getQuaternionTimeline("fore-arm-walking"),
         script.getQuaternionTimeline("fore-arm-waving")});
  }
  {
    auto arrow = entity.getObject("arrow");
    arrow->setScale(0.2);
    arrow->setScaleZ(1);
    arrow->setAbsoluteOrientation();
    arrow->setTransformY(2.4);
    arrow->setOpacity(0);
  }
}
void Character::throwStone(float angle, float power) {
  throwing = true;
  auto arrow = entity.getObject("arrow");
  arrow->setOpacity(1);
}
std::pair<std::pair<glm::vec3, glm::vec3>, bool> Character::throwStone() {
  auto successfullyThrown = throwing;
  throwing = false;
  auto arrow = entity.getObject("arrow");
  arrow->setOpacity(0);
  return std::make_pair(
      std::make_pair(
          arrow->getAbsolutePosition(),
          arrow->getOrientation().getRotationMatrix() * glm::vec3(0, 0, 1)),
      successfullyThrown);
}
void Character::update(float dtime) {
  auto upperArm = entity.getObject("upper-arm-right");
  auto foreArm = entity.getObject("fore-arm-right");
  if (throwing) {
    if (waveRate < 1) {
      waveRate += ARM_WAVE_CHANGE_RATE;
    } else {
      waveRate = 1;
    }
  } else {
    if (waveRate > 0) {
      waveRate -= ARM_WAVE_CHANGE_RATE;
    } else {
      waveRate = 0;
    }
  }
  upperArm->setOrientationManagerRate(waveRate);
  foreArm->setOrientationManagerRate(waveRate);
  time += dtime;
  entity.updateManagers(time);
}

void Character::draw(ShaderProgram& shader) { entity.draw(shader); }