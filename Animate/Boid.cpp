#include "Boid.h"

#include <algorithm>
#include <vector>

#include <glm/glm.hpp>

#include "../Math/Quaternion.h"

Boid::Boid(Object3D& objecti, Entity& targetsi)
    : object(objecti), baits(targetsi), feed(0) {}

glm::vec3 Boid::decision(glm::vec3 nearbyCenter,
                         std::vector<glm::vec3>& nearbyBoids) {
  glm::vec3 result(position);
  float weight = 1;
  // Assemble the crowd.
  result += 0.05F * nearbyCenter;
  weight += 0.05;

  // Repeal by nearby boids
  for (auto& pos : nearbyBoids) {
    result += 100.0F * (position - (pos - position));
    weight += 100.0;
  }

  if (feed < 0 && baits.size()) {
    // Chase with the nearest bait
    glm::vec3 targetPosition(
        (*std::min_element(
             baits.begin(), baits.end(),
             [this](auto elem1, auto elem2) {
               return glm::distance(position, elem1->getAbsolutePosition()) <
                      glm::distance(position, elem2->getAbsolutePosition());
             }))
            ->getAbsolutePosition());
    if (glm::distance(position, targetPosition) > 2) {
      // Fly towards the target
      result += 5.0F * targetPosition;
      weight += 5.0;
    } else {
      // feed. Stops chasing the baits.
      // Resets the feed counter.
      feed = FEED_ONCE;
    }
  } else {
    feed -= 1;
  }

  // Repeal by walls
  if (position.x > 20) {
    result += (position.x - 20) * glm::vec3(-1, 0, 0);
    weight += (position.x - 20);
  }
  if (position.x < -20) {
    result += (position.x + 20) * glm::vec3(1, 0, 0);
    weight += (position.x + 20);
  }
  if (position.z > 20) {
    result += (position.z - 20) * glm::vec3(0, 0, -1);
    weight += (position.z - 20);
  }
  if (position.z < -20) {
    result += (position.z + 20) * glm::vec3(0, 0, 1);
    weight += (position.z + 20);
  }

  // Repeal by gravity
  if (position.y > 0) {
    result += 100.0F * glm::vec3(0, -1, 0);
    weight += 100.0F;
  }
  // Repeal by water level
  if (position.y > -5) {
    result += 1.0F * glm::vec3(0, -1, 0);
    weight += 1.0F;
  }
  // Repeal by abyss
  if (position.y < -50) {
    result += 1.0F * glm::vec3(0, 1, 0);
    weight += 1.0F;
  }

  return result / weight;
}

void Boid::setDirection(glm::vec3 directioni) {
  direction = glm::normalize(directioni);
}

void Boid::update(float time, glm::vec3 nearbyCenter,
                  std::vector<glm::vec3>& nearbyBoids) {
  rotation += ANGULAR_VELOCITY * time;
  glm::vec3 desiredPosition = decision(nearbyCenter, nearbyBoids);
  if (desiredPosition != position) {
    glm::vec3 diff = glm::normalize(desiredPosition - position) - direction;
    diff *= time;
    direction += diff;
    direction = normalize(direction);
  }

  position += direction * time * VELOCITY;
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
  pokes.clear();
  antiPokes.clear();
  for (auto& boid : boids) {
    // Generate a list of nearby boids.
    std::vector<glm::vec3> nearbyBoids;
    for (auto& boidTesting : boids) {
      if (glm::distance(boid->getPosition(), boidTesting->getPosition()) <
              NEARBY_BOID_DISTANCE_THRESHOLD &&
          boid->getPosition() != boidTesting->getPosition()) {
        nearbyBoids.emplace_back(boidTesting->getPosition());
      }
    }
    bool boidIsPositive = boid->getPosition().y > 0;
    boid->update(time, getCenterNear(boid->getPosition()), nearbyBoids);
    if (boidIsPositive ^ (boid->getPosition().y > 0)) {
      (boidIsPositive ? pokes : antiPokes)
          .emplace_back(boid->getPosition().x, boid->getPosition().z);
    }
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