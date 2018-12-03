#include "Boid.h"

#include <vector>

#include <glm/glm.hpp>

#include "../Math/Quaternion.h"

Boid::Boid(Object3D& objecti, const Object3D& targeti)
    : object(objecti), target(targeti) {}

glm::vec3 Boid::decision(glm::vec3 nearbyCenter,
                         std::vector<glm::vec3>& nearbyBoids) {
  glm::vec3 result(position);
  float weight = 1;
  // Assemble the crowd.
  result += 0.2F * nearbyCenter;
  weight += 0.2;

  // Repeal by nearby boids
  for (auto& pos : nearbyBoids) {
    result += 100.0F * (position - (pos - position));
    weight += 100.0;
  }

  // Fly towards the target
  glm::vec3 targetPosition(target.getTransformX(), target.getTransformY(),
                           target.getTransformZ());
  if (glm::distance(position, targetPosition) > 30) {
    result += 1.0F * targetPosition;
    weight += 1.0;
  }
  // Repeal by the target
  if (glm::distance(position, targetPosition) < 10) {
    result += 1.0F * (position - (targetPosition - position));
    weight += 1.0;
  }

  return result / weight;
}

void Boid::setDirection(glm::vec3 directioni) {
  direction = glm::normalize(directioni);
}

void Boid::update(float time, glm::vec3 nearbyCenter,
                  std::vector<glm::vec3>& nearbyBoids) {
  rotation += ANGULAR_VELOCITY * time;
  glm::vec3 diff =
      glm::normalize(decision(nearbyCenter, nearbyBoids) - position) -
      direction;
  diff *= time;
  direction += diff;

  position += direction * time;
}

void Boid::draw(ShaderProgram& shader) {
  // Rotate the boid itself as rotation indicates
  Quaternion s(cos(rotation), 0, 0, sin(rotation));
  // Rotate the boid to the desired direction:
  glm::vec3 vectorPart = glm::cross(glm::vec3(0, 0, 1), direction);
  float realPart = glm::dot(glm::vec3(0, 0, 1), direction) + 1;
  Quaternion q(realPart, vectorPart.x, vectorPart.y, vectorPart.z);
  // Combine the two rotations
  q.normalize();
  q *= s;
  q.normalize();
  object.setOrientation(q);

  object.moveTo(getPosition());
  object.draw(shader);
}

void Boids::update(float time) {
  for (auto& boid : boids) {
    // TODO: Generate a list of nearby boids.
    std::vector<glm::vec3> nearbyBoids;
    boid->update(time, getCenterNear(boid->getPosition()), nearbyBoids);
  }
}

glm::vec3 Boids::getCenterNear(glm::vec3 center) {
  glm::vec3 result(0);
  int counter = 0;
  for (auto& boid : boids) {
    auto pos = boid->getPosition();
    if (glm::distance(pos, center) < sight) {
      counter += 1;
      result += pos;
    }
  }
  if (counter > 0) {
    result /= counter;
  }
  return result;
}