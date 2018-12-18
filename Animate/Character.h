#ifndef ANIMATE_CHARACTER_H
#define ANIMATE_CHARACTER_H

#include "../Graphics/Entity/Entity.h"
#include "../Graphics/Shaders/Shader.h"
#include "Scripts.h"

class Character {
 private:
  Scripts script;
  Model model;
  Entity entity;
  float time;
  bool waving;
  float waveRate;

 public:
  constexpr static const float ARM_WAVE_CHANGE_RATE = 0.05;
  Character();
  void wave() { waving = true; }
  void unwave() { waving = false; }
  void update(float time);
  void draw(ShaderProgram& shader);
};

#endif  // ANIMATE_CHARACTER_H