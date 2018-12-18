#include "Character.h"

Character::Character()
    : time(0),
      waving(false),
      waveRate(0),
      script(Scripts::loadScript("res/scripts/lab2.keys")),
      model(ModelLoader::loadOffFile("res/models/cube.off")) {
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
  {
    auto pelvis = entity.getObject("pelvis");
    pelvis->setScaleY(6);
    pelvis->setCenterY(-2);
    pelvis->setTransformXManager(script.getFloatTimeline("xPos"));
    pelvis->setTransformYManager(script.getFloatTimeline("yPos"));
    pelvis->setTransformZManager(script.getFloatTimeline("zPos"));
    pelvis->setOrientationManager(script.getQuaternionTimeline("orientation"));
  }
  {
    auto thigh = entity.getObject("thigh-left");
    thigh->setScaleY(5);
    thigh->setCenterY(5);
    thigh->setTransformX(2);
    thigh->setTransformY(-4);
    thigh->setOrientationManager(script.getQuaternionTimeline("thigh-walking"),
                                 2);
  }
  {
    auto thigh = entity.getObject("thigh-right");
    thigh->setScaleY(5);
    thigh->setCenterY(5);
    thigh->setTransformX(-2);
    thigh->setTransformY(-4);
    thigh->setOrientationManager(script.getQuaternionTimeline("thigh-walking"));
  }
  {
    auto calf = entity.getObject("calf-left");
    calf->setScaleY(5);
    calf->setCenterY(5);
    calf->setTransformY(-6);
    calf->setOrientationManager(script.getQuaternionTimeline("calf-walking"),
                                2);
  }
  {
    auto calf = entity.getObject("calf-right");
    calf->setScaleY(5);
    calf->setCenterY(5);
    calf->setTransformY(-6);
    calf->setOrientationManager(script.getQuaternionTimeline("calf-walking"));
  }
  {
    auto chest = entity.getObject("chest");
    chest->setScaleY(9);
    chest->setCenterY(-9);
    chest->setTransformY(8);
  }
  {
    auto arm = entity.getObject("upper-arm-left");
    arm->setScaleY(5);
    arm->setCenterY(5);
    arm->setTransformX(2);
    arm->setTransformY(4);
    arm->setOrientationManager(
        script.getQuaternionTimeline("upper-arm-walking"), 2);
  }
  {
    auto arm = entity.getObject("upper-arm-right");
    arm->setScaleY(5);
    arm->setCenterY(5);
    arm->setTransformX(-2);
    arm->setTransformY(4);
    arm->setOrientationManager(
        {script.getQuaternionTimeline("upper-arm-walking"),
         script.getQuaternionTimeline("upper-arm-waving")});
  }
  {
    auto arm = entity.getObject("fore-arm-left");
    arm->setScaleY(5);
    arm->setCenterY(5);
    arm->setTransformY(-6);
    arm->setOrientationManager(script.getQuaternionTimeline("fore-arm-walking"),
                               2);
  }
  {
    auto arm = entity.getObject("fore-arm-right");
    arm->setScaleY(5);
    arm->setCenterY(5);
    arm->setTransformY(-6);
    arm->setOrientationManager(
        {script.getQuaternionTimeline("fore-arm-walking"),
         script.getQuaternionTimeline("fore-arm-waving")});
  }
}
void Character::update(float dtime) {
  auto upperArm = entity.getObject("upper-arm-right");
  auto foreArm = entity.getObject("fore-arm-right");
  if (waving) {
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