#ifndef ANIMATE_CHARACTER_H
#define ANIMATE_CHARACTER_H

#include "../Graphics/Entity/Entity.h"
#include "../Graphics/Shaders/Shader.h"
#include "Scripts.h"

class Character {
 private:
  Scripts script;
  Model model;
  Model modelArrow;
  Entity entity;
  float time;
  bool throwing;
  float waveRate;

 public:
  constexpr static const float ARM_WAVE_CHANGE_RATE = 0.05;
  Character();
  void throwStone(float angle, float power);
  std::pair<std::pair<glm::vec3, glm::vec3>, bool> throwStone();
  void update(float time);
  void draw(ShaderProgram& shader);
};

#endif  // ANIMATE_CHARACTER_H