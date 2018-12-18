#include "Water.h"

Water::Water(int waterSizeIn)
    : waterTriangle(ModelLoader::getUnitTriangleShape()),
      waterSize(waterSizeIn) {
  for (int i = -waterSize; i <= waterSize; ++i) {
    auto& row = waterNodes.emplace_back();
    for (int j = -waterSize; j <= waterSize; ++j) {
      row.emplace_back().position = glm::vec3(i, 0, j);
    }
  }
}

void Water::drawAt(ShaderProgram& shader, glm::vec3 point1, glm::vec3 point2,
                   glm::vec3 point3, glm::vec3 point4) {
  glm::vec3 center = (point1 + point2 + point3 + point4) / 4.0F;
  glm::mat4 points(1.0F);
  points[0] = glm::vec4(point1, 0);
  points[1] = glm::vec4(point2, 0);
  points[2] = glm::vec4(center, 0);
  shader.setMatrix("transform", points);
  waterTriangle.draw(shader);
  points[0] = glm::vec4(point2, 0);
  points[1] = glm::vec4(point3, 0);
  points[2] = glm::vec4(center, 0);
  shader.setMatrix("transform", points);
  waterTriangle.draw(shader);
  points[0] = glm::vec4(point3, 0);
  points[1] = glm::vec4(point4, 0);
  points[2] = glm::vec4(center, 0);
  shader.setMatrix("transform", points);
  waterTriangle.draw(shader);
  points[0] = glm::vec4(point4, 0);
  points[1] = glm::vec4(point1, 0);
  points[2] = glm::vec4(center, 0);
  shader.setMatrix("transform", points);
  waterTriangle.draw(shader);
}

void Water::poke(int x, int y) {
  toPoke[std::make_pair(x, y)] += POKE_INCRAESEMENT;
}

void Water::update(float time) {
  for (auto iter = toPoke.begin(); iter != toPoke.end(); ++iter) {
    waterNodes[iter->first.first + waterSize][iter->first.second + waterSize]
        .position.y -= POKE_FORCE;
    iter->second -= 1;
  }

  // handle the springs
  for (int i = 1; i < waterNodes.size() - 1; ++i) {
    for (int j = 1; j < waterNodes[0].size() - 1; ++j) {
      // determine force to apply
      auto force1 = waterNodes[i][j].position - waterNodes[i + 1][j].position;
      auto force2 = waterNodes[i][j].position - waterNodes[i - 1][j].position;
      auto force3 = waterNodes[i][j].position - waterNodes[i][j + 1].position;
      auto force4 = waterNodes[i][j].position - waterNodes[i][j - 1].position;
      // Apply folk's law
      static auto folkLaw = [](auto& force) {
        constexpr static const float K = -5.0F;
        return K * (glm::length(force) - 1) / glm::length(force);
      };
      force1 *= folkLaw(force1);
      force2 *= folkLaw(force2);
      force3 *= folkLaw(force3);
      force4 *= folkLaw(force4);
      // Cheat: make the nodes "want" to get back.
      auto forceC = glm::vec3(i - waterSize, 0, j - waterSize) -
                    waterNodes[i][j].position;
      forceC *= 0.01F;
      waterNodes[i][j].speed +=
          (force1 + force2 + force3 + force4 + forceC) * time;
      // LP filter.
      waterNodes[i][j].speed *= 0.99F;
    }
  }
  for (int i = 1; i < waterNodes.size() - 1; ++i) {
    for (int j = 1; j < waterNodes[0].size() - 1; ++j) {
      waterNodes[i][j].position += waterNodes[i][j].speed * time;
    }
  }
  // remove if value = 0.
  for (auto iter = toPoke.begin(); iter != toPoke.end();) {
    if (iter->second <= 0) {
      toPoke.erase(iter++);
    } else {
      ++iter;
    }
  }
}

void Water::draw(ShaderProgram& shader) {
  for (int i = 0; i < waterNodes.size() - 1; ++i) {
    for (int j = 0; j < waterNodes[0].size() - 1; ++j) {
      drawAt(shader, waterNodes[i][j].position, waterNodes[i + 1][j].position,
             waterNodes[i + 1][j + 1].position, waterNodes[i][j + 1].position);
    }
  }
}
