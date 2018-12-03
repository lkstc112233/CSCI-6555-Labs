#ifndef GRAPHICS_CAMERA_CAMERA_H
#define GRAPHICS_CAMERA_CAMERA_H

#include <glm/glm.hpp>

const float DEFAULT_CAMERA_SPEED = 0.05;

class Camera {
 private:
  const glm::vec3 up = glm::vec3(0, 1, 0);
  bool lock = false;
  glm::vec3 front = glm::vec3(0, 0, 1);
  glm::vec3 target = glm::vec3(0);

 public:
  glm::vec3 position = glm::vec3(0);
  void move(const glm::vec3 &direction);
  void moveForward(float distance = DEFAULT_CAMERA_SPEED);
  void moveBackward(float distance = DEFAULT_CAMERA_SPEED);
  void moveLeft(float distance = DEFAULT_CAMERA_SPEED);
  void moveRight(float distance = DEFAULT_CAMERA_SPEED);
  void moveUp(float distance = DEFAULT_CAMERA_SPEED);
  void moveDown(float distance = DEFAULT_CAMERA_SPEED);
  void turnYaw(float yaw);
  void turnPitch(float pitch);
  void lookAt(const glm::vec3 &position);
  void lockView(const glm::vec3 &position);
  void unlockView();
  bool isViewLocked() { return lock; }
  glm::mat4 getViewMat();
  glm::vec3 getFrontVec();
  glm::vec3 getLeftVec();
  glm::vec3 getUpVec();
};

#endif  // GRAPHICS_CAMERA_CAMERA_H