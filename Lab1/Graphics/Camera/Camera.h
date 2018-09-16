#ifndef GRAPHICS_CAMERA_CAMERA_H
#define GRAPHICS_CAMERA_CAMERA_H

#include <glm/glm.hpp>

class Camera
{
  private:
    const glm::vec3 up = glm::vec3(0, 1, 0);
    bool lock = false;
    glm::vec3 front = glm::vec3(0, 0, 1);
    glm::vec3 target = glm::vec3(0);
  public:
    glm::vec3 position = glm::vec3(0);
    void move(const glm::vec3 &direction);
    void moveForward(float distance);
    void moveBackward(float distance);
    void moveLeft(float distance);
    void moveRight(float distance);
    void moveUp(float distance);
    void moveDown(float distance);
    void turnYaw(float yaw);
    void turnPitch(float pitch);
    void lookAt(const glm::vec3 &position);
    void lockView(const glm::vec3 &position);
    void unlockView();
    bool isViewLocked() { return lock; }
    glm::mat4 getViewMat();
};

#endif // GRAPHICS_CAMERA_CAMERA_H